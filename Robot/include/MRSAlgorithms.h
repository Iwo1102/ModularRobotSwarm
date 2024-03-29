#ifndef MRSALGORITHMS_H
#define MRSALGORITHMS_H
    #include <math.h>

    float rssiToDistance(float fixedPower, float rssi, float environent);
    void getRobotCoords(float* coordBuf, float b1r, float b2r, float bb);
#endif
