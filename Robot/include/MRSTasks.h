#ifndef MRSTASKS_H
#define MRSTASKS_H

#include <Arduino.h>
#include <ArduinoBLE.h>
#include "FreeRTOSConfig.h"

#include "MRSAlgorithms.h"
#include "MRSWifiClient.h"
#include "MRSRobotData.h"
#include "MRSmotors.h"

const int KILOBYTE = 1024;
const int ORDERLENGTH = 20;

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
void getOrdersTask(void * pvParameters);

//Motor Tasks
void completeOrdersTask(void * pvParameters);
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
                    checkProximity = NULL,
                    getOrders = NULL,
                    completeOrders = NULL;

    SemaphoreHandle_t   BeaconfoundSemaphore = NULL,
                        testConnectionSemaphore = NULL,
                        getDistanceSemaphore = NULL,
                        checkProximitySemaphore = NULL;
    EventGroupHandle_t  distanceEvent = NULL;
    QueueHandle_t orderQueue = NULL;
};

struct mrsOrdersStruct_h {
    String direction;
    float distance;
};

enum mrsOrders_h {
    forward = 1,
    backward,
    left,
    right,
    stop,
    sweep,
    perimiter,
    scan,
    scan_sweep,
    scan_perimiter
};

extern struct mrsTask_h taskVals;
extern struct mrsTaskHandle_h  mrsHandle;

#endif
