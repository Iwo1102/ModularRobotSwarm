#ifndef MRSWIFICLIENT_H
#define MRSWIFICLIENT_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string.h>

struct MRS_wifi {
    const char* SSID = "ssid";
    const char* PASSWORD = "password";
    const char* serverName = "Servername";
};

extern struct MRS_wifi MRS_wifi;

#endif