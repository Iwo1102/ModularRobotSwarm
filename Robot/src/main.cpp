/*
  Scan
*/

#include "MRSTasks.h"
#include "MRSwifiClient.h"
#include "MRSmotors.h"

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

	gpioInit();

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
	mrsHandle.orderQueue = xQueueCreate(20, sizeof(mrsOrdersStruct_h));

	xTaskCreatePinnedToCore(peripheralTask, "Peripheral Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 1, &mrsHandle.peripheral, 0);
	xTaskCreatePinnedToCore(distanceTask, "Distance Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 2, &mrsHandle.distance, 0);
	xTaskCreatePinnedToCore(findCellTask, "Find Cell Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 5, &mrsHandle.findCell, 0);
	xTaskCreatePinnedToCore(testConnectionTask, "Test Connection Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 3, &mrsHandle.testConnection, 0);
	xTaskCreatePinnedToCore(getBeaconDistanceTask, "Beacon-Beacon distance Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 5, &mrsHandle.getBeaconDistance, 0);
	xTaskCreatePinnedToCore(updateLocationTask, "Update Location Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 4, &mrsHandle.updateLocation, 0);
	xTaskCreatePinnedToCore(getOthersTask, "Get Other Robots Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 4, &mrsHandle.getOthers, 0);
	xTaskCreatePinnedToCore(checkProximityTask, "Check Proximity Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 3, &mrsHandle.checkProximity, 1);
	xTaskCreatePinnedToCore(getOrdersTask, "get orders Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 2, &mrsHandle.getOrders, 0);
	xTaskCreatePinnedToCore(completeOrdersTask, "complete orders Task", 4 * KILOBYTE, NULL, configMAX_PRIORITIES - 2, &mrsHandle.completeOrders, 1);

}

void loop() {
}
