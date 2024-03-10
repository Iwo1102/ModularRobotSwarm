#include "MRSwifiClient.h"
#include <cstring>

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

void MRS_wifiPostJson(std::string page, std::string valueType, std::string value, char distance[10]) {
    std::string httpRequestData;
    std::string url;
    if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        

        url = MRS_wifi.serverName;
        url = url + page;

        http.begin(client, url.c_str());
        http.addHeader("Content-Type", "application/json");

        // Data to send with HTTP POST
        //concat string into HTTPRequestData
        if (page == "/findBeaconCell")
            httpRequestData = "{\"name\": \"" + value + "\",\"distance\": " + distance + "}";
        if (page == "/updateDistance")
            httpRequestData = "{\"id\": " + value + ",\"distance\": " + distance + "}";
        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData.c_str());
        Serial.printf("POST HTTP Response code: %d\n", httpResponseCode);

        http.end();
    } else {
        Serial.println("WiFi Disconnected");

    }
}

String MRS_wifiGetJson(std::string page, std::string valueType, std::string value) {
    std::string url;
    String json = "{}";
    if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        
        url = MRS_wifi.serverName;
        url = url + page;


        if (page == "/TestConnection") {
            url = url + "?id=" + value + "&type=1";
        } else if (page == "/getId") {
            url = url + "?name=" + value + "&type=1";
        } else {
            return "-1";
        }

        http.begin(url.c_str());

        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            Serial.print("GET HTTP Response code: ");
            Serial.println(httpResponseCode);
            json = http.getString();
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected");
        return "-1";
    }
    return json;
}
