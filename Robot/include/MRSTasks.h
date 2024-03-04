#ifndef MRSTASKS_H
#define MRSTASKS_H

void peripheralTask(void * pvParameters);
void distanceTask(void * pvParameters);
void switchBeaconTask(void * pvParameters);

struct mrsTask_h {
    BLEDevice peripheral;
    String beacon1 = "";
    String beacon2 = "";
    uint8_t currentBeacon = 1;
    float bbDistance = 1, b1Distance, b2Distance;
    float coords[2] = {0, 0};
    const std::string name = "Robot1";
};

struct mrsTaskHandle_h {
    TaskHandle_t peripheral = NULL, distance = NULL;
    SemaphoreHandle_t BeaconfoundSemaphore = NULL;
};

extern struct mrsTask_h taskVals;
extern struct mrsTaskHandle_h  mrsHandle;

#endif
