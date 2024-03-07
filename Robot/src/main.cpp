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

	xTaskCreate(peripheralTask, "Peripheral Task", 2 * 1024, NULL, configMAX_PRIORITIES - 1, &mrsHandle.peripheral);
	xTaskCreate(distanceTask, "Distance Task", 2 * 1024, NULL, configMAX_PRIORITIES - 3, &mrsHandle.distance);
	xTaskCreate(findCellTask, "Find Cell Task", 4 * 1024, NULL, configMAX_PRIORITIES - 2, &mrsHandle.findCell);
	mrsHandle.BeaconfoundSemaphore = xSemaphoreCreateBinary();
	mrsHandle.testConnectionSemaphore = xSemaphoreCreateBinary();
}

void loop() {
}
