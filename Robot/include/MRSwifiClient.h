#ifndef MRSWIFICLIENT_H
#define MRSWIFICLIENT_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <cstring>
#include <string>

#include "MRSRobotData.h"

struct MRS_wifi {
    const char* SSID = "";
    const char* PASSWORD = "";
    const char* serverName = "";
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
