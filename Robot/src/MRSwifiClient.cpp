#include "MRSWifiClient.h"

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
 }

int MRS_wifiPostJson(std::string page, std::string postValue) {
    std::string httpRequestData;
    std::string url;
    int httpResponseCode = 0;

    if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        
        url = MRS_wifi.serverName;
        url = url + page;

        http.begin(client, url.c_str());
        http.addHeader("Content-Type", "application/json");

        // Data to send with HTTP POST
        httpRequestData = postValue;
        // Send HTTP POST request
        httpResponseCode = http.POST(httpRequestData.c_str());
        Serial.printf("POST HTTP Response code: %d\n", httpResponseCode);

        http.end();
    } else {
        Serial.println("WiFi Disconnected");
    }
    return httpResponseCode;
}

String MRS_wifiGetJson(std::string page, std::string getValue) {
    std::string url;
    String returnJson = "{}";
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, getValue);

    // Test if parsing succeeds.
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return "500";
    }

    if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        
        url = MRS_wifi.serverName;
        url = url + page;

        if (page == "/TestConnection") {
            std::string id = doc["id"];
            url = url + "?id=" + id + "&type=0";
        } else if (page == "/getId") {
            std::string name = doc["name"];
            url = url + "?name=" + name + "&type=0";
        } else if (page == "/getDistance");
        else if (page == "/findOthers") {
            std::string id = doc["id"];
            url = url + "?id=" + id;
        } else {
            Serial.printf("Page not found \r\n");
            return "404";
        }

        http.begin(url.c_str());

        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            Serial.print("GET HTTP Response code: ");
            Serial.println(httpResponseCode);
            returnJson = http.getString();
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected");
        return "500";
    }
    return returnJson;
}
