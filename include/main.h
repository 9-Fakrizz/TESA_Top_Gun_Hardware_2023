#ifndef __MAIN_H__
#define __MAIN_H__

// include files
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
#include <Esp.h>
#include <esp_log.h>

#define WIFI_SSID       "Ok"
#define WIFI_PASSWORD   "q12345678"
#define MQTT_DEV_ID     666
#define MQTT_EVT_TOPIC  "tgr2023/bravely_divide/evt"
#define MQTT_CMD_TOPIC  "tgr2023/bravely_divide/cmd"

// type definitions
#define TASK_DETECTION_TYPE      0

typedef struct evt_msg_t {
    int date;
    float value;
    float cubic;
} evt_msg_t;

// shared variables
extern xQueueHandle evt_queue;
extern bool enable_flag;
extern bool trigger;
extern float cubic;
extern int date;
// public function prototypes

#endif 