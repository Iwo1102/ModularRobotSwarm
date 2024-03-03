/*
  advertising
*/
#include <Arduino.h>
#include <ArduinoBLE.h>
#include <string.h>
#include <stdio.h> 

#include "MRSwifiClient.h"

#define INTERVAL_TO_MS(x) (x / 0.625)
const std::string MEBEACON = "beacon1";
const std::string YOUBEACON = "beacon2";

bool adv_scan = 1;

int id = -1;

BLEService beaconService("19B10000-F512-0A71-54B1-D104768A1214");
BLECharacteristic rssiCharacterisitc("19B10000-F512-0A71-54B1-D104768A1214", BLERead | BLEWrite | BLEBroadcast, "0");

void setup() {
  Serial.begin(115200);
  while (!Serial);

  MRS_SetupConnection();

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  beaconService.addCharacteristic(rssiCharacterisitc);

  Serial.println("Bluetooth® Low Energy beacon");
  Serial.print("Device Adress: ");
  Serial.println(BLE.address());

  BLE.setLocalName(MEBEACON.c_str());
  
  BLE.scanForName(YOUBEACON.c_str());
  BLE.advertise();
}

void loop() {
float distance = 0;
  if (adv_scan) {
     BLEDevice peripheral = BLE.available();

    if (peripheral) {
      adv_scan = 1;
      Serial.print("Address: ");
      Serial.println(peripheral.address());

  
      Serial.println("Discovered a peripheral");
      Serial.println("-----------------------");


      if (peripheral.hasLocalName()) {
        Serial.print("Local Name: ");
        Serial.println(peripheral.localName());
      }

      if (peripheral.hasAdvertisedServiceUuid()) {
        Serial.print("Service UUIDs: ");
        for (int i = 0; i < peripheral.advertisedServiceUuidCount(); i++) {
          Serial.print(peripheral.advertisedServiceUuid(i));
          Serial.print(" ");
        }
        Serial.println();
      }


      Serial.print("RSSI: ");
      Serial.println(peripheral.rssi());
      char rssi[5];
      sprintf(rssi, "%d", peripheral.rssi());
      float power = (((float)-37 + (float)abs(peripheral.rssi())) / ((float)10 * (float)5));
      distance = pow(10, power);
      Serial.printf("Distance: %.2fm\r\n", distance);

      rssiCharacterisitc.writeValue(rssi);
      if (rssiCharacterisitc.broadcast())
        Serial.printf("Broadcast Successful\r\n");
      else
        Serial.printf("Broadcast Unsuccessful\r\n");

      BLE.stopScan();
      adv_scan = 0;
      delay(50);
    }
  } else {

    adv_scan = 1;
    BLE.scanForName(YOUBEACON.c_str());
    delay(100);
  }
	char distanceStr[10];
	snprintf(distanceStr, sizeof(distanceStr), "%f", distance);
  	if(id == -1) {
    	//findCell
  		MRS_wifiPostJson("/findBeaconCell", "name", MEBEACON, distanceStr);
		id = MRS_wifiGetJson("/getId", "name", MEBEACON).toInt();
  	} else {
		char idStr[10];
		snprintf(idStr, sizeof(idStr), "%d", id);
		MRS_wifiPostJson("/updateDistance", "id", idStr, distanceStr);
		if (!MRS_wifiGetJson("/TestConnection", "id", idStr).toInt()){
			id = -1;
			Serial.printf("Availability was lost\n");
		}
	}
	delay(1000);
}
