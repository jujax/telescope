#include <ESPAsyncWebServer.h>
#include "ArduinoJson.h"
#include "SPIFFS.h"

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "application/json", "{\"message\":\"Not found\"}");
}

void initRTS2Server()
{
    if (filesystem)
    {
        server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(200, "application/json", "{\"ret\": 0}");
            ESP.restart();
        });
        server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
            if (request->hasParam("ssid") && request->hasParam("pwd"))
            {
                String s = request->getParam("ssid")->value();
                String p = request->getParam("pwd")->value();
                File ssidFile = SPIFFS.open("/ssid.txt", "w");
                File pwdFile = SPIFFS.open("/pwd.txt", "w");
                if (ssidFile && pwdFile)
                {
                    ssidFile.print(s);
                    pwdFile.print(p);
                    ssidFile.close();
                    pwdFile.close();
                }
                request->send(200, "application/json", "{\"ssid\": \"" + s + "\", \"pwd\": \"" + p + "\"}");
            }
            else
                request->send(404, "application/json", "{\"message\":\"Not found\"}");
        });
    }
    server.on("/api/devbytype", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("t") && request->getParam("t")->value() == "2")
            request->send(200, "application/json", "[\"telescope\"]");
        else
            request->send(404, "application/json", "{\"message\":\"Not found\"}");
    });
    server.on("/api/deviceinfo", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (request->hasParam("d") && request->getParam("d")->value() == "telescope")
            request->send(200, "application/json", "{\"readonly\": 0}");
        else
            request->send(404, "application/json", "{\"message\":\"Not found\"}");
    });
    server.on("/api/get", HTTP_GET, [](AsyncWebServerRequest *request) {
        const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5);
        DynamicJsonDocument doc(capacity);
        JsonObject d = doc.createNestedObject("d");
        d["LONGITUD"] = longitude;
        d["LATITUDE"] = latitude;
        d["ALTITUDE"] = altitude;
        d["target_distance"] = 0;

        JsonObject d_TEL = d.createNestedObject("TEL");
        d_TEL["ra"] = actual_da;
        d_TEL["dec"] = actual_dec;
        //d_TEL["ha"] = calculHA(calculLST(), actual_da);

        String json;
        serializeJson(doc, json);
        if (request->hasParam("d") && request->getParam("d")->value() == "telescope")
            request->send(200, "application/json", json);
        else
            request->send(404, "application/json", "{\"message\":\"Not found\"}");
    });
    server.on("/api/cmd", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (!request->hasParam("d") || request->getParam("d")->value() != "telescope")
            request->send(404, "application/json", "{\"message\":\"Not found\"}");
        else
        {
            String moveCmd = request->getParam("c")->value();
            int r = moveCmd.indexOf(" ");
            int d = moveCmd.indexOf(" ", r + 1);
            double ra = moveCmd.substring(r + 1, d).toDouble();
            double dec = moveCmd.substring(d + 1).toDouble();
            goto_da = ra;
            move_da = true;
            goto_dec = dec;
            move_dec = true;
            request->send(200, "application/json", "{\"ret\": 0}");
        }
    });

    server.onNotFound(notFound);
    server.begin();
}