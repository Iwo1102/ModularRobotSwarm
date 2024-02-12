#include <Arduino.h>
#include <ArduinoBLE.h>
#include <math.h>
#include "FreeRTOSConfig.h"

#include "MRSTasks.h"
#include "MRSAlgorithms.h"

struct mrsTask_h taskVals;
struct mrsTaskHandle_h  mrsHandle;

BLEDevice peripheral;

void peripheralTask(void * pvParameters) {
	for (;;) {
		Serial.println("Periferal Task");
		// check if a peripheral has been discovered
		peripheral = BLE.available();

		if (peripheral) {
			// discovered a peripheral
			Serial.println("Discovered a peripheral");
			Serial.println("-----------------------");

			// print address
			Serial.print("Address: ");
			Serial.println(peripheral.address());
			if (taskVals.currentBeacon == 1)
				taskVals.beacon1 = peripheral.address();
			else 
				taskVals.beacon2 = peripheral.address();

			BLE.stopScan();

			// print the local name, if present
			if (peripheral.hasLocalName()) {
				Serial.print("Local Name: ");
				Serial.println(peripheral.localName());
			}
			xSemaphoreGive(mrsHandle.BeaconfoundSemaphore);
		}
		// vTaskDelete(NULL);
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void distanceTask(void * pvParameters) {
	// print the RSSI
	for(;;) {
		Serial.println("Distance Task");  
		if(xSemaphoreTake(mrsHandle.BeaconfoundSemaphore, 100) == pdTRUE) {
			Serial.print("RSSI: ");
			Serial.println(peripheral.rssi());
			//Serial.println("Fake RSSI: -37");
			float distance = rssiToDistance(-50, peripheral.rssi(), 5);
			Serial.printf("Distance: %.2fm\r\n", distance);
			if (taskVals.currentBeacon == 1) {
				Serial.println("Beacon 1");
			taskVals.b1Distance = distance;
			} else {
				Serial.println("Beacon 2");
				taskVals.b2Distance = distance;
			}
			distance = 0;

			if (taskVals.b1Distance != 0 && taskVals.b2Distance != 0) {
				taskVals.temp = pow(taskVals.bbDistance, 2) + pow(taskVals.b1Distance, 2) - pow(taskVals.b2Distance, 2);
				taskVals.coords[0] = taskVals.temp / (2 * taskVals.bbDistance);
				taskVals.coords[1] = (pow(taskVals.b1Distance, 2) - pow(taskVals.coords[0], 2));
				if (taskVals.coords[1] < 0) {
					taskVals.coords[1] = sqrt(abs(taskVals.coords[1]));
					taskVals.coords[1] = taskVals.coords[1] * -1;
				} else {
					taskVals.coords[1] = sqrt(taskVals.coords[1]);
				}
				Serial.printf("co-ordinates: %.2f, %.2f\r\n", taskVals.coords[0], taskVals.coords[1]);
			}
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}

void switchBeaconTask(void * pvParameters) {
	for(;;) {
		Serial.println("Switch Beacon Task");
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
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}
