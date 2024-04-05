#ifndef MRSTASKS_H
#define MRSTASKS_H

#include <Arduino.h>
#include <ArduinoBLE.h>
#include "FreeRTOSConfig.h"

#include "MRSAlgorithms.h"
#include "MRSWifiClient.h"
#include "MRSRobotData.h"

#define KILOBYTE 1024

#define calcCoordsBit (1 << 0)

//BLE Tasks
void peripheralTask(void * pvParameters);
void distanceTask(void * pvParameters);

//WebServer Tasks
void findCellTask(void * pvParameters);
void testConnectionTask(void * pvParameters);
void getOthersTask(void * pvParameters);
void updateLocationTask(void * pvParameters);
void getBeaconDistanceTask(void * pvParameters);
void checkProximityTask(void * pvParameters);

struct mrsTask_h {
    BLEDevice peripheral;
    String beacon1 = "";
    String beacon2 = "";
    uint8_t currentBeacon = 1;
    float bbDistance = 1, b1Distance, b2Distance;
};

struct mrsTaskHandle_h {
    TaskHandle_t    peripheral = NULL,
                    distance = NULL,
                    findCell = NULL,
                    testConnection = NULL,
                    getBeaconDistance = NULL,
                    updateLocation = NULL,
                    getOthers = NULL,
                    checkProximity = NULL;

    SemaphoreHandle_t   BeaconfoundSemaphore = NULL,
                        testConnectionSemaphore = NULL,
                        getDistanceSemaphore = NULL,
                        checkProximitySemaphore = NULL;
    EventGroupHandle_t  distanceEvent = NULL;
};

extern struct mrsTask_h taskVals;
extern struct mrsTaskHandle_h  mrsHandle;

#endif
