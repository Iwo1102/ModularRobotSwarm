#ifndef MRSWIFICLIENT_H
#define MRSWIFICLIENT_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <cstring>

struct MRS_wifi {
    const char* SSID = "ssid";
    const char* PASSWORD = "password";
    const char* serverName = "Servername";
};

void MRS_SetupConnection();
int MRS_wifiPostJson(char[32], char[16], char[8]);
String MRS_wifiGetJson(char[32], char[4]);

extern struct MRS_wifi MRS_wifi;

#endif