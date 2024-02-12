#ifndef MRSTASKS_H
#define MRSTASKS_H

void peripheralTask(void * pvParameters);
void distanceTask(void * pvParameters);
void switchBeaconTask(void * pvParameters);

struct mrsTask_h {
    String beacon1 = "";
    String beacon2 = "";
    uint8_t currentBeacon = 1;
    int16_t value;
    float bbDistance = 1, b1Distance, b2Distance, temp;
    double coords[2] = {0, 0};
};

struct mrsTaskHandle_h {
    TaskHandle_t *peripheralHandle, *switchButtonHandle, *distanceHandle;
    SemaphoreHandle_t BeaconfoundSemaphore = NULL;
    SemaphoreHandle_t switchBeaconSemaphore = NULL;
};

extern struct mrsTask_h taskVals;
extern struct mrsTaskHandle_h  mrsHandle;

#endif
