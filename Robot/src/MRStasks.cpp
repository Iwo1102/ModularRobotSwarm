#include "MRSTasks.h"

struct mrsTask_h taskVals;
struct mrsTaskHandle_h  mrsHandle;
struct mrsData_h thisRobot;
struct mrsAlgo_h algoVals;

mrsData_h otherRobots[4];

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
				EventBits_t eventBits;
				eventBits = xEventGroupWaitBits(mrsHandle.distanceEvent, calcCoordsBit, pdFALSE, pdTRUE, pdMS_TO_TICKS(50));
				if (eventBits & calcCoordsBit) {
					//Get the Co-ordinates of the Robot
					if (taskVals.b1Distance != 0 && taskVals.b2Distance != 0) {
						getRobotCoords(thisRobot.coords, taskVals.b1Distance, taskVals.b2Distance, taskVals.bbDistance);
						Serial.printf("co-ordinates: %.2f, %.2f\r\n", thisRobot.coords[0], thisRobot.coords[1]);
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
		// {"name": "thisRobot.name", "coords":[thisRobot.coords[0], thisRobot.coords[1]]}
		std::string jsonPost = "{\"name\":\"" + thisRobot.name + "\", \"coords\":[" + std::to_string(thisRobot.coords[0]) + ", " +  std::to_string(thisRobot.coords[1]) + "]}";
		if (MRS_wifiPostJson("/findCell", jsonPost) == 200) {
			// {"name" : "thisRobot.name"}
			int tempResult = MRS_wifiGetJson("/getId", "{\"name\":\"" + thisRobot.name + "\"}").toInt();
			if ((tempResult != 404) || (tempResult != 500)){
				thisRobot.id = tempResult;
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
			// {"id": thisRobot.id}
			if ((MRS_wifiGetJson("/TestConnection", "{\"id\": " + std::to_string(thisRobot.id) + "}").toInt())) {
				vTaskDelay(5000 / portTICK_PERIOD_MS);
			} else {
				vTaskResume(mrsHandle.findCell);
			}
		}
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	}
}

void getOthersTask(void * pvParameters) {
	for(;;) {
		Serial.printf("Get other Robots Task\r\n");
		//{"id":thisRobot.id}
		String tempResult = MRS_wifiGetJson("/findOthers", "{\"id\": " + std::to_string(thisRobot.id) + "}");
		if ((tempResult.toInt() != 404) || (tempResult.toInt() != 500)) {

			JsonDocument doc;
			DeserializationError error = deserializeJson(doc, tempResult);

			// Test if parsing fails.
			if (error) {
				Serial.print(F("deserializeJson() failed: "));
				Serial.println(error.f_str());
			} else {
				JsonArray robots = doc.as<JsonArray>();
				int i = 0;
				for(JsonObject robot : robots) {
					std::string robotName = robot["name"].as<std::string>();
					otherRobots[i].name = robotName;

					int robotId = robot["id"].as<int>();
					otherRobots[i].id = robotId;

					JsonArray coords = robot["coords"];
					otherRobots[i].coords[0] = coords[0].as<float>();
					otherRobots[i].coords[1] = coords[1].as<float>();

					Serial.printf("Robot:\r\nname: %s\r\nid: %d\r\ncoords: [%.2f, %.2f]\r\n",
									otherRobots[i].name.c_str(), otherRobots[i].id, otherRobots[i].coords[0], otherRobots[i].coords[1]);
					i++;
				}
				xSemaphoreGive(mrsHandle.checkProximitySemaphore);
			}
		}
		else
			xSemaphoreGive(mrsHandle.testConnectionSemaphore);
		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
} 

void updateLocationTask(void * pvParameters) {
	for(;;) {
		Serial.printf("Update Location Task\r\n");
		//{"id":thisRobot.id, "coords": [thisRobot.coords[0], thisRobot.coords[1]]}
		if (MRS_wifiPostJson("/updateLocation", "{\"id\":" + std::to_string(thisRobot.id)  + ","
												+ "\"coords\": [" + std::to_string(thisRobot.coords[0]) + ", " + std::to_string(thisRobot.coords[1]) +"]}") != 200)
			xSemaphoreGive(mrsHandle.testConnectionSemaphore);
		vTaskDelay(250 / portTICK_PERIOD_MS);
	}
}

void getBeaconDistanceTask(void * pvParameters) {
	for(;;) {
		Serial.printf("getBeaconDistance Task\r\n");
		// {"id": thisRobot.id}
		float response = MRS_wifiGetJson("/getDistance").toFloat();
		if ((response != 404) || (response != 500)) {
			taskVals.bbDistance = response;
			xEventGroupSetBits(mrsHandle.distanceEvent, calcCoordsBit);
			vTaskDelay(10000 / portTICK_PERIOD_MS);
		} else {
			xSemaphoreGive(mrsHandle.testConnectionSemaphore);
			vTaskDelay(100 / portTICK_PERIOD_MS);
		}
	}
}

void checkProximityTask(void * pvParameters) {
	for(;;) {
		if (xSemaphoreTake(mrsHandle.checkProximitySemaphore, 50) == pdTRUE) {
			for (int i = 0; i < swarmSize; i++) {
				//Check if too close to other robots
				if (distanceDiff(thisRobot.coords, otherRobots[i].coords) < 0.4) {
					Serial.printf("Robot too close to robot %s", otherRobots[i].name.c_str());
				}
			}
		}
		vTaskDelay(200 / portTICK_PERIOD_MS);
	}
}

void getOrdersTask(void * pvParameters) {
	for(;;) { 
		Serial.printf("getOrders Task\r\n");
		String tempResult = MRS_wifiGetJson("/getOrders", "{\"id\": " + std::to_string(thisRobot.id) + "}");
		if ((tempResult.toInt() != 404) || (tempResult.toInt() != 500)) {
			// Remove '[' and ']'
			tempResult.replace("[", "");
			tempResult.replace("]", "");

			int arr[20];
			int index = 0;
			int start = 0;
			int end = 0;

			for (int i = 0; i < tempResult.length(); i++) {
				if (tempResult[i] == ',' || i == tempResult.length() - 1) {
					end = (i == tempResult.length() - 1) ? i + 1 : i;
					String numStr = tempResult.substring(start, end);
					arr[index++] = numStr.toInt();
					start = i + 1;
				}
			}
			Serial.printf("Orders: ");
			for (int i = 0; i < 6; i++) {
				Serial.printf("%d, ", arr[i]);
				if (arr[i] != 0)
					xQueueSend(mrsHandle.orderQueue, &arr[i], portMAX_DELAY);
			}
			Serial.printf("\r\n");
			vTaskDelay(5000 / portTICK_PERIOD_MS);
		}
	}
}

void completeOrdersTask(void * pvParameters) {
	for(;;) {
		uint8_t orders;
		if(xQueueReceive(mrsHandle.orderQueue, &orders, 50)) {
			switch (orders) {
				case forward:
					Serial.printf("Moving Forward\r\n");
				break;
				case backward:
					Serial.printf("Moving Backward\r\n");
				break;
				case left:
					Serial.printf("Moving left\r\n");
				break;
				case right:
					Serial.printf("Moving right\r\n");
				break;
				case sweep:
					Serial.printf("Sweeping\r\n");
				break;
				case perimiter:
					Serial.printf("Permiter moving\r\n");
				break;
				case scan:
					Serial.printf("scanning\r\n");
				break;
				case scan_sweep:
					Serial.printf("sweeping and scanning\r\n");
				break;
				case scan_perimiter:
					Serial.printf("perimeter and scanning\r\n");
				break;
				default:
					Serial.printf("Unknown Order\r\n");
			}
		}
	}
}
