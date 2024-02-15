#include <Arduino.h>
#include <ArduinoBLE.h>
#include <math.h>
#include "FreeRTOSConfig.h"

#include "MRSTasks.h"
#include "MRSAlgorithms.h"

struct mrsTask_h taskVals;
struct mrsTaskHandle_h  mrsHandle;

void peripheralTask(void * pvParameters) {
	for (;;) {
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
    	
		if(xSemaphoreTake(mrsHandle.BeaconfoundSemaphore, 0) == pdTRUE) {
			Serial.print("RSSI: ");
			Serial.println(taskVals.peripheral.rssi());
			float distance = rssiToDistance(-50, taskVals.peripheral.rssi(), 5);
			Serial.printf("Distance: %.2fm\r\n", distance);
			if (taskVals.currentBeacon == 1)
				taskVals.b1Distance = distance;
			else
				taskVals.b2Distance = distance;
			distance = 0;

			//Get the Co-ordinates of the Robot
			if (taskVals.b1Distance != 0 && taskVals.b2Distance != 0) {
				getRobotCoords(taskVals.coords, taskVals.b1Distance, taskVals.b2Distance, taskVals.bbDistance);
				Serial.printf("co-ordinates: %.2f, %.2f\r\n", taskVals.coords[0], taskVals.coords[1]);
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
