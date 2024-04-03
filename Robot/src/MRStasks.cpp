#include "MRSTasks.h"

struct mrsTask_h taskVals;
struct mrsTaskHandle_h  mrsHandle;
struct mrsData_h robotData;
struct mrsAlgo_h algoVals;

void peripheralTask(void * pvParameters) {
	for (;;) {
		//Serial.println("Peripheral Task");
		// check if a peripheral has been discovered
		taskVals.peripheral = BLE.available();

		if (taskVals.peripheral) {
			// discovered a peripheral
			Serial.println("Discovered a peripheral");
			Serial.println("-----------------------");

			// print address
			Serial.print("Address: ");
			Serial.println(taskVals.peripheral.address());
			if (taskVals.peripheral.localName() == "beacon1")
				taskVals.beacon1 = taskVals.peripheral.address();
			else if (taskVals.peripheral.localName() == "beacon2")
				taskVals.beacon2 = taskVals.peripheral.address();
			else
				Serial.println("Beacon name not found");

			BLE.stopScan();

			// print the local name, if present
			if (taskVals.peripheral.hasLocalName()) {
				Serial.print("Local Name: ");
				Serial.println(taskVals.peripheral.localName());
			}
			xSemaphoreGive(mrsHandle.BeaconfoundSemaphore);
			vTaskSuspend(mrsHandle.peripheral);
		}
		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
}

void distanceTask(void * pvParameters) {
	// print the RSSI
	for(;;) {
		//Serial.println("Distance Task");
		if(xSemaphoreTake(mrsHandle.BeaconfoundSemaphore, 100) == pdTRUE) {
			Serial.print("RSSI: ");
			Serial.println(taskVals.peripheral.rssi());
			if (taskVals.currentBeacon == 1) {
				algoVals.rssiValuesB1[algoVals.rssiCountB1] = taskVals.peripheral.rssi();
      			algoVals.rssiCountB1++;
				if (algoVals.rssiCountB1 == RSSIAVGSIZE)
        			algoVals.rssiCountB1 = 0;
			
				for (int i = 0; i < RSSIAVGSIZE; i++) {
					if (algoVals.rssiValuesB1[i] == 0) {
						algoVals.calcDistaceFlag = 0;
						break;
					} else
						algoVals.calcDistaceFlag = 1;
				}
			} else {
				algoVals.rssiValuesB2[algoVals.rssiCountB2] = taskVals.peripheral.rssi();
      			algoVals.rssiCountB2++;
				if (algoVals.rssiCountB2 == RSSIAVGSIZE)
        			algoVals.rssiCountB2 = 0;
			
				for (int i = 0; i < RSSIAVGSIZE; i++) {
					if (algoVals.rssiValuesB2[i] == 0) {
						algoVals.calcDistaceFlag = 0;
						break;
					} else
						algoVals.calcDistaceFlag = 1;
				}
			}

			if (algoVals.calcDistaceFlag) {
				float avg = 0;
				Serial.printf("RSSI Values: ");
				if (taskVals.currentBeacon == 1) {
					for (int i = 0; i < RSSIAVGSIZE; i++) {
						avg += algoVals.rssiValuesB1[i];
						Serial.printf("%d, ", algoVals.rssiValuesB1[i]);
					}
				} else {
					for (int i = 0; i < RSSIAVGSIZE; i++) {
						avg += algoVals.rssiValuesB2[i];
						Serial.printf("%d, ", algoVals.rssiValuesB2[i]);
					}
				}
				
				avg = avg / RSSIAVGSIZE;
				Serial.printf("\r\nAverage: %.0f\r\n", avg);

				float distance = rssiToDistance(-44.6, avg, 4);
				Serial.printf("Distance: %.2fm\r\n", distance);
				if (taskVals.currentBeacon == 1)
					taskVals.b1Distance = distance;
				else
					taskVals.b2Distance = distance;
				distance = 0;
				if (xSemaphoreTake(mrsHandle.getDistanceSemaphore, 0) == pdTRUE) {
					//Get the Co-ordinates of the Robot
					if (taskVals.b1Distance != 0 && taskVals.b2Distance != 0) {
						getRobotCoords(taskVals.coords, taskVals.b1Distance, taskVals.b2Distance, taskVals.bbDistance);
						Serial.printf("co-ordinates: %.2f, %.2f\r\n", taskVals.coords[0], taskVals.coords[1]);
					}
				}
			}
			//switch beacon to scan
				Serial.println();
				if (taskVals.currentBeacon == 1) {
					taskVals.currentBeacon = 2;
					BLE.stopScan();
					BLE.scanForName("beacon2"); 
				} else {
					taskVals.currentBeacon = 1;
					BLE.stopScan();
					BLE.scanForName("beacon1"); 
				}
		} 
		vTaskResume(mrsHandle.peripheral);
	}
}

void findCellTask(void * pvParameters) {
	for(;;) {
		Serial.println("FindCellTask");
		// {"name": "robotData.name", "coords":[coordStr0, coordStr1]}
		std::string jsonPost = "{\"name\":\"" + robotData.name + "\", \"coords\":[" + std::to_string(taskVals.coords[0]) + ", " +  std::to_string(taskVals.coords[1]) + "]}";
		if (MRS_wifiPostJson("/findCell", jsonPost) == 200) {
			// {"name" : "robotData.name"}
			int tempResult = MRS_wifiGetJson("/getId", "{\"name\":\"" + robotData.name + "\"}").toInt();
			if ((tempResult != 404) || (tempResult != 500)){
				robotData.id = tempResult;
				vTaskSuspend(NULL);
			} else {
				vTaskDelay(100 / portTICK_PERIOD_MS);
			}
		} else {
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
	}
}

void testConnectionTask(void * pvParameters) {
	for(;;) {
		Serial.printf("Test Connection Task\r\n");
		if (xSemaphoreTake(mrsHandle.testConnectionSemaphore, 50) == pdTRUE) {
			// {"id": robotData.id}
			if ((MRS_wifiGetJson("/TestConnection", "{\"id\": " + std::to_string(robotData.id) + "}").toInt())) {
				vTaskDelay(5000 / portTICK_PERIOD_MS);
			} else {
				vTaskResume(mrsHandle.findCell);
			}
		}
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

/*void getOthersTask(void * pvParameters) {
	for(;;) {
		//JsonDocument robots;
		//{"id":robotData.id}
		int tempResult = MRS_wifiGetJson("/findOthers", "{\"id\": " + std::to_string(robotData.id) + "}").toInt();
		if ((tempResult != 404) || (tempResult != 500))
			Serial.println(tempResult);
		else
			xSemaphoreGive(mrsHandle.testConnectionSemaphore);
		vTaskDelay(50 / portTICK_PERIOD_MS);
	}
} */

void updateLocationTask(void * pvParameters) {
	for(;;) {
		Serial.printf("Update Location Task\r\n");
		//{"id":robotData.id, "coords": [taskVals.coords[0], taskVals.coords[1]]}
		if (MRS_wifiPostJson("/updateLocation", "{\"id\":" + std::to_string(robotData.id)  + ","
												+ "\"coords\": [" + std::to_string(taskVals.coords[0]) + ", " + std::to_string(taskVals.coords[1]) +"]}") != 200)
			xSemaphoreGive(mrsHandle.testConnectionSemaphore);
		vTaskDelay(250 / portTICK_PERIOD_MS);
	}
}

void getBeaconDistanceTask(void * pvParameters) {
	for(;;) {
		Serial.printf("getBeaconDistance Task\r\n");
		// {"id": robotData.id}
		float response = MRS_wifiGetJson("/getDistance").toFloat();
		if ((response != 404) || (response != 500)) {
			taskVals.bbDistance = response;
			xSemaphoreGive(mrsHandle.getDistanceSemaphore);
			vTaskDelay(10000 / portTICK_PERIOD_MS);
		} else {
			xSemaphoreGive(mrsHandle.testConnectionSemaphore);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
	}
}
