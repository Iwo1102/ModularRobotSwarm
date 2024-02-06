/*
  Scan
*/
#include <Arduino.h>
#include <ArduinoBLE.h>
#include <math.h>

String beacon1 = "";
String beacon2 = "";
uint8_t currentBeacon = 1;
int16_t value;
double bbDistance = 1, b1Distance, b2Distance, temp;
double coords[2] = {0, 0};


void setup() {
  Serial.begin(115200);
  while (!Serial);

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
}



void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral
    Serial.println("Discovered a peripheral");
    Serial.println("-----------------------");
    
     

    // print address
    Serial.print("Address: ");
    Serial.println(peripheral.address());
    if (currentBeacon == 1) {
      beacon1 = peripheral.address();
    } else {
      beacon2 = peripheral.address();
    }

    BLE.stopScan();

    // print the local name, if present
    if (peripheral.hasLocalName()) {
      Serial.print("Local Name: ");
      Serial.println(peripheral.localName());
    }

    // print the RSSI
    Serial.print("RSSI: ");
    Serial.println(peripheral.rssi());
    double power = (((double)-50 + (double)abs(peripheral.rssi())) / ((double)10 * (double)5));
    double distance = pow(10, power);
    Serial.printf("Distance: %.2fm\r\n", distance);
    if (currentBeacon == 1) {
      Serial.println("Beacon 1");
      b1Distance = distance;
    } else {
      Serial.println("Beacon 2");
      b2Distance = distance;
    }
    distance = 0;

    if (b1Distance != 0 && b2Distance != 0) {
      temp = pow(bbDistance, 2) + pow(b1Distance, 2) - pow(b2Distance, 2);
      coords[0] = temp / (2 * bbDistance);
      coords[1] = (pow(b1Distance, 2) - pow(coords[0], 2));
      if (coords[1] < 0) {
        coords[1] = sqrt(abs(coords[1]));
        coords[1] = coords[1] * -1;
      } else {
        coords[1] = sqrt(coords[1]);
      }
      Serial.printf("co-ordinates: %.2f, %.2f\r\n", coords[0], coords[1]);
    }

    Serial.println();
    if (currentBeacon == 1) {
      currentBeacon = 2;
      BLE.stopScan();
      delay(1000);
      BLE.scanForName("beacon2"); 
    } else {
      currentBeacon = 1;
      BLE.stopScan();
      delay(1000);
      BLE.scanForName("beacon1"); 
    }
    delay(1000);
  }
}
