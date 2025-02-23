#ifndef WIFI_TELESCOPE_H
#define WIFI_TELESCOPE_H

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

class WifiTelescope {
public:
    /**
     * @brief Constructeur par défaut
     */
    WifiTelescope() {}

    /**
     * @brief Initialise la connexion WiFi en mode client
     * @param ssid Nom du réseau WiFi
     * @param password Mot de passe du réseau
     * @return true si la connexion est réussie, false sinon
     */
    bool initClient(const char* ssid, const char* password) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        
        for (int attempt = 0; attempt < 4; attempt++) {
            Serial.println(attempt);
            delay(2500);
            
            if (WiFi.waitForConnectResult() == WL_CONNECTED) {
                setupOTA();
                Serial.println(WiFi.localIP());
                MDNS.begin("telescope");
                ArduinoOTA.begin();
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Initialise le WiFi en mode point d'accès
     */
    void initAP() {
        WiFi.mode(WIFI_AP);
        WiFi.softAP("telescope", "telescope");
        MDNS.begin("telescope");
        setupOTA();
        ArduinoOTA.begin();
    }

private:
    /**
     * @brief Configure les callbacks pour la mise à jour OTA
     */
    void setupOTA() {
        ArduinoOTA
            .onStart([]() {
                String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
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
                switch (error) {
                    case OTA_AUTH_ERROR: Serial.println("Auth Failed"); break;
                    case OTA_BEGIN_ERROR: Serial.println("Begin Failed"); break;
                    case OTA_CONNECT_ERROR: Serial.println("Connect Failed"); break;
                    case OTA_RECEIVE_ERROR: Serial.println("Receive Failed"); break;
                    case OTA_END_ERROR: Serial.println("End Failed"); break;
                }
            });
    }
};

// Création d'une instance globale pour la compatibilité avec le code existant
WifiTelescope wifiTelescope;

// Fonctions wrapper pour maintenir la compatibilité avec le code existant
inline bool initWifi(char* ssid, char* password) {
    return wifiTelescope.initClient(ssid, password);
}

inline void initAPWifi() {
    wifiTelescope.initAP();
}

#endif // WIFI_TELESCOPE_H