#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

bool initWifi(char *ssid, char *password)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    for (int x = 0; x < 4; x++)
    {
        Serial.println(x);
        delay(2500);
        if (WiFi.waitForConnectResult() != WL_CONNECTED)
            continue;
        ArduinoOTA
            .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH)
                    type = "sketch";
                else // U_SPIFFS
                    type = "filesystem";

                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                Serial.println("Start updating " + type);
            })
            .onEnd([]() {
                Serial.println("\nEnd");
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                Serial.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR)
                    Serial.println("Auth Failed");
                else if (error == OTA_BEGIN_ERROR)
                    Serial.println("Begin Failed");
                else if (error == OTA_CONNECT_ERROR)
                    Serial.println("Connect Failed");
                else if (error == OTA_RECEIVE_ERROR)
                    Serial.println("Receive Failed");
                else if (error == OTA_END_ERROR)
                    Serial.println("End Failed");
            });
        Serial.println(WiFi.localIP());
        Serial.println(MDNS.begin("telescope"));
        ArduinoOTA.begin();
        return true;
    }
    return false;
}

void initAPWifi()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP("telescope", "telescope");
    MDNS.begin("telescope");
    ArduinoOTA.begin();
}