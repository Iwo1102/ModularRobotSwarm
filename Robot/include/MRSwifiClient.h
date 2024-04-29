#ifndef MRSWIFICLIENT_H
#define MRSWIFICLIENT_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <cstring>
#include <string>

#include "MRSRobotData.h"

struct MRS_wifi {
    const char* SSID = "VM8258558";
    const char* PASSWORD = "d3affhtse4hS";
    const char* serverName = "http://192.168.0.122:3000";
};

enum MRS_pages {
    testConnection,
    getId
};

void MRS_SetupConnection();
int MRS_wifiPostJson(std::string, std::string);
String MRS_wifiGetJson(std::string, std::string = "{}");

extern struct MRS_wifi MRS_wifi;

#endif
