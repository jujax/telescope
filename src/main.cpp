#include <Arduino.h>
#include "variables.h"
#include "wifitelescope.h"
#include "rts2.h"
#include "soc/rtc_wdt.h"
#include "esp_task_wdt.h"
#include "A4988.h"

A4988 stepper_DA(MOTOR_STEPS, AD_DIR, AD_STEP);
A4988 stepper_DEC(MOTOR_STEPS, DEC_DIR, DEC_STEP);

void taskTime(void *params)
{
    esp_task_wdt_delete(NULL);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        if (!stepper_DA.getCurrentState())
            stepper_DA.move(4);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
    }
}

void taskMotorDA(void *params)
{
    esp_task_wdt_delete(NULL);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        unsigned wait_time_micros1 = stepper_DA.nextAction();
        if (!wait_time_micros1)
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}
void taskMotorDEC(void *params)
{
    esp_task_wdt_delete(NULL);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        unsigned wait_time_micros2 = stepper_DEC.nextAction();
        if (!wait_time_micros2)
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello World");
    char *def = "telescope";
    if (LittleFS.begin(true))
    {
        filesystem = true;
        File ssidFile = LittleFS.open("/ssid.txt");
        File pwdFile = LittleFS.open("/pwd.txt");
        if (ssidFile && pwdFile)
        {
            Serial.println("Wifi configuration found");
            char *ssid = strdup(ssidFile.readString().c_str());
            char *pwd = strdup(pwdFile.readString().c_str());
            ssidFile.close();
            pwdFile.close();
            wifiConnected = initWifi(ssid, pwd);
            Serial.println("Wifi connected");
        }
        // si pas de wifi configuré, on crée un point d'accès
        else
        {
            Serial.println("No wifi configuration found");
            Serial.println("Creating AP");
            initAPWifi();
        }
    }

    stepper_DA.begin(RPM, MICROSTEPS);
    stepper_DEC.begin(RPM, MICROSTEPS);
    stepper_DEC.enable();
    stepper_DA.enable();
    stepper_DA.setSpeedProfile(stepper_DA.LINEAR_SPEED, 100, 100);
    stepper_DEC.setSpeedProfile(stepper_DEC.LINEAR_SPEED, 100, 100);

    xTaskCreate(
        taskTime,   /* Task function. */
        "taskTime", /* name of task. */
        10000,      /* Stack size of task */
        NULL,       /* parameter of the task */
        1,          /* priority of the task */
        NULL);      /* Task handle to keep track of created task */

    xTaskCreatePinnedToCore(
        taskMotorDA,   /* Task function. */
        "taskMotorDA", /* name of task. */
        10000,         /* Stack size of task */
        NULL,          /* parameter of the task */
        9,             /* priority of the task */
        NULL, 1);      /* Task handle to keep track of created task */

    xTaskCreatePinnedToCore(
        taskMotorDEC,   /* Task function. */
        "taskMotorDEC", /* name of task. */
        10000,          /* Stack size of task */
        NULL,           /* parameter of the task */
        9,              /* priority of the task */
        NULL, 1);       /* Task handle to keep track of created task */

    initRTS2Server();
}

void loop()
{
    if (move_da)
    {
        Serial.println("Move DA");
        stepper_DA.startRotate((actual_da - goto_da) * RAPPORT_POULIE);
        Serial.println((actual_da - goto_da) * RAPPORT_POULIE);
        actual_da = goto_da;
        move_da = false;
    }
    if (move_dec)
    {
        Serial.println("Move DEC");
        stepper_DEC.startRotate((actual_dec - goto_dec) * RAPPORT_POULIE);
        Serial.println((actual_dec - goto_dec) * RAPPORT_POULIE);
        actual_dec = goto_dec;
        move_dec = false;
    }
    // if (!move_da && !move_dec && wifiConnected)
    //     ArduinoOTA.handle();
    delay(1000);
}
