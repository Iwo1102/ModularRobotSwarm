#include <math.h>


/*
* fixedPower: Measured RSSI value at 1 metre
* rssi: the RSSI value calcuted
* environent: Constant depending on the signal loss in the enviornment. Range 2-4
*/
float rssiToDistance(float fixedPower, float rssi, float environent) {
    float power = ((fixedPower + (float)abs(rssi)) / (10 * environent));
    float distance = pow(10, power);

    return distance;
}