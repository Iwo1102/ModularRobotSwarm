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

	thisRobot.name = "Robot2";

	Serial.println("Bluetooth® Low Energy Central scan");
	Serial.print("Device Adress: ");
	Serial.println(BLE.address());

	// start scanning for peripheral
	BLE.scanForName("beacon1");

	mrsHandle.BeaconfoundSemaphore = xSemaphoreCreateBinary();
	mrsHandle.testConnectionSemaphore = xSemaphoreCreateBinary();
	mrsHandle.getDistanceSemaphore = xSemaphoreCreateBinary();
	mrsHandle.checkProximitySemaphore = xSemaphoreCreateBinary();
	mrsHandle.distanceEvent = xEventGroupCreate();

	xTaskCreate(peripheralTask, "Peripheral Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 1, &mrsHandle.peripheral);
	xTaskCreate(distanceTask, "Distance Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 2, &mrsHandle.distance);
	xTaskCreate(findCellTask, "Find Cell Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 5, &mrsHandle.findCell);
	xTaskCreate(testConnectionTask, "Test Connection Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 3, &mrsHandle.testConnection);
	xTaskCreate(getBeaconDistanceTask, "Beacon-Beacon distance Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 5, &mrsHandle.getBeaconDistance);
	xTaskCreate(updateLocationTask, "Update Location Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 4, &mrsHandle.updateLocation);
	xTaskCreate(getOthersTask, "Get Other Robots Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 4, &mrsHandle.getOthers);
	xTaskCreate(checkProximityTask, "Check Proximity Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 3, &mrsHandle.checkProximity);
}

void loop() {
}
