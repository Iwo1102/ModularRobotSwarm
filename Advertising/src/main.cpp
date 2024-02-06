/*
  advertising
*/
#include <Arduino.h>
#include <ArduinoBLE.h>

#define INTERVAL_TO_MS(x) (x / 0.625)
#define MEBEACON "beacon2"
#define YOUBEACON "beacon1"

bool adv_scan = 1;

BLEService beaconService("19B10000-F512-0A71-54B1-D104768A1214");
BLECharacteristic rssiCharacterisitc("19B10000-F512-0A71-54B1-D104768A1214", BLERead | BLEWrite | BLEBroadcast, "0");

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }

  beaconService.addCharacteristic(rssiCharacterisitc);

  Serial.println("Bluetooth® Low Energy beacon");
  Serial.print("Device Adress: ");
  Serial.println(BLE.address());

  BLE.setLocalName(MEBEACON);
  
  BLE.scanForName(YOUBEACON);
  BLE.advertise();
}

void loop() {

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
      double power = (((double)-37 + (double)abs(peripheral.rssi())) / ((double)10 * (double)5));
      double distance = pow(10, power);
      Serial.printf("Distance: %.2fm\r\n", distance);

      distance = 0;

      rssiCharacterisitc.writeValue(rssi);
      if (rssiCharacterisitc.broadcast())
        Serial.printf("Broadcast Successful\r\n");
      else
        Serial.printf("Broadcast Unsuccessful\r\n");

      BLE.stopScan();
      adv_scan = 0;
      delay(1000);
    }
  } else {

    adv_scan = 1;
    BLE.scanForName(YOUBEACON);
    delay(2000);
  }

 
}
