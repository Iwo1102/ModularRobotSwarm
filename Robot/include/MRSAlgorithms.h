#ifndef MRSALGORITHMS_H
#define MRSALGORITHMS_H
#include <math.h>

const uint8_t RSSIAVGSIZE = 3;

struct mrsAlgo_h {
    int16_t rssiValuesB1[RSSIAVGSIZE] = {0};
    int16_t rssiValuesB2[RSSIAVGSIZE] = {0};
    uint8_t rssiCountB1 = 0;
    uint8_t rssiCountB2 = 0;
    bool calcDistaceFlag = 0;
};

extern struct mrsAlgo_h algoVals;

float rssiToDistance(float fixedPower, float rssi, float environent);
void getRobotCoords(float* coordBuf, float b1r, float b2r, float bb);
float distanceDiff(float coord1Buff[2], float coord2Buff[2]);
#endif
