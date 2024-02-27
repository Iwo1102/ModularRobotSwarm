#include "MRSwifiClient.h"

struct MRS_wifi MRS_wifi;

void MRS_SetupConnection() {
        WiFi.begin(MRS_wifi.SSID, MRS_wifi.PASSWORD);
        Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
 
    Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

int MRS_wifiPostJson(char page[32], char name[16], char distance[8]) {
    char httpRequestData[128];
    char url[64];
    if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        
        strcpy(url, MRS_wifi.serverName);
        strcat(url, page);

        http.begin(client, url);

        http.addHeader("Content-Type", "application/json");

        // Data to send with HTTP POST
        //concat string into HTTPRequestData
        //String httpRequestData = "{\"name\":" + name + ",\"distance\":" + distance + "}";
        if (page == "/findBeaconCell")
        strcpy(httpRequestData, "{\"name\": \"");
        strcat(httpRequestData, name);
        strcat(httpRequestData, "\",\"distance\":");
        strcat(httpRequestData, distance);

        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);

        http.end();
    } else {
        Serial.println("WiFi Disconnected");

    }
}

String MRS_wifiGetJson(char page[32], char id[4]) {
    char url[64];
    String json = "{}";
    if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        
        strcpy(url, MRS_wifi.serverName);
        strcat(url, page);

        if (page == "/TestConnection") {
            strcat(url, "?id=");
            strcat(url, id);
            strcat(url, "&type=1");
        }

        http.begin(url);

        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            json = http.getString();
        } else {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected");

    }
    return json;
}
