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
	coordBuf[1] = sqrt(fabs(pow(b1r, 2)) - pow(coordBuf[0], 2));
   if (b1r >= b2r)
        coordBuf[1] *= -1;
}

float distanceDiff(float coord1Buff[2], float coord2Buff[2]) {
    return sqrt(pow((coord2Buff[0] - coord1Buff[0]), 2) + pow((coord2Buff[1] - coord1Buff[1]), 2));
}

float edgeDistance(float line1Coords[2], float line2Coords[2], float robotCoords[2]) {
    float distance;
    float slope = (line2Coords[1] -  line1Coords[1]) / (line2Coords[0] -  line1Coords[0]);

    distance = (fabs (line1Coords[1] - slope * line1Coords[0] - robotCoords[1])) / (sqrt(1 + slope * slope));
    return distance;
}


