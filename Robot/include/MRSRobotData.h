#ifndef MRSROBOTDATA_H
#define MRSROBOTDATA_H

#include <string>

struct mrsData_h {
    std::string name = "";
    float coords[2] = {0, 0};
    int id;
};

extern struct mrsData_h thisRobot;
#endif
