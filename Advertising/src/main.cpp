/*
  Scan

  This example scans for Bluetooth® Low Energy peripherals and prints out their advertising details:
  address, local name, advertised service UUID's.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  This example code is in the public domain.
*/
#include <Arduino.h>
#include <ArduinoBLE.h>
#include <string.h>

#define INTERVAL_TO_MS(x) (x / 0.625)
#define MEBEACON "beacon2"
#define YOUBEACON "beacon1"

bool adv_scan = 1;


void setup() {
  Serial.begin(115200);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");

    while (1);
  }
  BLE.setLocalName(MEBEACON);

  

  Serial.println("Bluetooth® Low Energy beacon");
  Serial.print("Device Adress: ");
  Serial.println(BLE.address());

  
  BLE.scanForName(YOUBEACON);
  BLE.advertise();
}

void loop() {
  int16_t rssi;

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

      Serial.print("RSSI: ");
      Serial.println(peripheral.rssi());
     
      BLE.stopScan();
      adv_scan = 0;
    }
  } else {

    adv_scan = 1;
    BLE.scanForName(YOUBEACON);
  }

  delay(1000);
}
