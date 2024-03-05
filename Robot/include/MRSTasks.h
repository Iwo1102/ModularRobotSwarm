#ifndef MRSTASKS_H
#define MRSTASKS_H

#include <Arduino.h>
#include <ArduinoBLE.h>
#include <math.h>
#include "FreeRTOSConfig.h"

#include "MRSAlgorithms.h"
#include "MRSWifiClient.h"
#include "MRSRobotData.h"

//BLE Tasks
void peripheralTask(void * pvParameters);
void distanceTask(void * pvParameters);

//WebServer Tasks
void findCellTask(void * pvParameters);
void testConnectionTask(void * pvParameters);
void getOthersTask(void * pvParameters);
void updateLocationTask(void * pvParameters);
void getDistanceTask(void * pvParameters);

struct mrsTask_h {
    BLEDevice peripheral;
    String beacon1 = "";
    String beacon2 = "";
    uint8_t currentBeacon = 1;
    float bbDistance, b1Distance, b2Distance;
    float coords[2] = {0, 0};
};

struct mrsTaskHandle_h {
    TaskHandle_t    peripheral = NULL,
                    distance = NULL,
                    findCell = NULL,
                    testConnection = NULL;

    SemaphoreHandle_t   BeaconfoundSemaphore = NULL,
                        testConnectionSemaphore = NULL;
};

extern struct mrsTask_h taskVals;
extern struct mrsTaskHandle_h  mrsHandle;

#endif
