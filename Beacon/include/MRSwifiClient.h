#ifndef MRSWIFICLIENT_H
#define MRSWIFICLIENT_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string>

struct MRS_wifi {
    const char* SSID = "VM8258558";
    const char* PASSWORD = "d3affhtse4hS";
    const char* serverName = "http://192.168.0.122:3000";
};

void MRS_SetupConnection();
void MRS_wifiPostJson(std::string, std::string, std::string, char*);
String MRS_wifiGetJson(std::string, std::string, std::string);

extern struct MRS_wifi MRS_wifi;

#endif
