#include "MRSAlgorithms.h"

/*
* fixedPower:   Measured RSSI value at 1 metre
* rssi:         The RSSI value calcuted
* environent:   Constant depending on the signal loss in the enviornment. Range 2-4
*/
float rssiToDistance(float fixedPower, float rssi, float environent) {
    float power = ((fixedPower + (float)abs(rssi)) / (10 * environent));
    float distance = pow(10, power);

    return distance;
}

/*
* coordBuf: Buffer pointing to Coordinate array
* b1r:      Distance between beacon 1 and robot
* b2r:      Distance between beacon 2 and robot
* bb:       Distance between  2 beacons
*/
void getRobotCoords(float* coordBuf, float b1r, float b2r, float bb) {
    float temp = pow(bb, 2) + pow(b1r, 2) - pow(b2r, 2);
	coordBuf[0] = temp / (2 * bb);
	coordBuf[1] = (pow(b1r, 2) - pow(coordBuf[0], 2));
    if (coordBuf[1] < 0) {
        coordBuf[1] = sqrt(abs(coordBuf[1]));
        coordBuf[1] = coordBuf[1] * -1;
    } else {
        coordBuf[1] = sqrt(coordBuf[1]);
    }
}
