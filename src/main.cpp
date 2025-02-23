#include <Arduino.h>
#include "variables.h"
#include "wifitelescope.h"
#include "rts2.h"
#include "soc/rtc_wdt.h"
#include "esp_task_wdt.h"
#include "A4988.h"
#include "lx200.h"
#include "motors.h"

RTS2 rts2;
LX200 lx200(Serial2);
WifiTelescope wifiTelescope;
Motors motors;

void taskTime(void *params)
{
    esp_task_wdt_delete(NULL);
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        if (!motors.getCurrentStateDA())
            motors.moveDA(4);
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
        unsigned wait_time_micros1 = motors.nextActionDA();
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
        unsigned wait_time_micros2 = motors.nextActionDEC();
        if (!wait_time_micros2)
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Hello World");
    
    Serial.println("Creating AP");
    wifiTelescope.initAP();

    motors.initialize();

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

    rts2.begin();
}

void loop()
{
    if (move_da)
    {
        Serial.println("Move DA");
        motors.moveDA((actual_da - goto_da) * RAPPORT_POULIE);
        Serial.println((actual_da - goto_da) * RAPPORT_POULIE);
        actual_da = goto_da;
        move_da = false;
    }
    if (move_dec)
    {
        Serial.println("Move DEC");
        motors.moveDEC((actual_dec - goto_dec) * RAPPORT_POULIE);
        Serial.println((actual_dec - goto_dec) * RAPPORT_POULIE);
        actual_dec = goto_dec;
        move_dec = false;
    }
    // if (!move_da && !move_dec && wifiConnected)
    //     ArduinoOTA.handle();
    delay(1000);
}
