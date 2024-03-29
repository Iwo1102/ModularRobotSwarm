/*
  Scan
*/

#include "MRSTasks.h"
#include "MRSwifiClient.h"

void setup() {
	Serial.begin(115200);
	while (!Serial);

	// Connect to Wifi
	MRS_SetupConnection();
	// begin initialization
	if (!BLE.begin()) {
		Serial.println("starting Bluetooth® Low Energy module failed!");

		while (1);
	}

	Serial.println("Bluetooth® Low Energy Central scan");
	Serial.print("Device Adress: ");
	Serial.println(BLE.address());

	// start scanning for peripheral
	BLE.scanForName("beacon1");

	mrsHandle.BeaconfoundSemaphore = xSemaphoreCreateBinary();
	mrsHandle.testConnectionSemaphore = xSemaphoreCreateBinary();
	mrsHandle.getDistanceSemaphore = xSemaphoreCreateBinary();

	xTaskCreate(peripheralTask, "Peripheral Task", 2 * KILOBYTE, NULL, configMAX_PRIORITIES - 1, &mrsHandle.peripheral);
	xTaskCreate(distanceTask, "Distance Task", 2 * KILOBYTE, NULL, configMAX_PRIORITIES - 2, &mrsHandle.distance);
	xTaskCreate(findCellTask, "Find Cell Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 5, &mrsHandle.findCell);
	xTaskCreate(testConnectionTask, "Test Connection Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 4, &mrsHandle.testConnection);
	xTaskCreate(getBeaconDistanceTask, "Beacon-Beacon distance Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 4, &mrsHandle.getBeaconDistance);
	xTaskCreate(updateLocationTask, "Update Location Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 4, &mrsHandle.updateLocation);
}

void loop() {
}
