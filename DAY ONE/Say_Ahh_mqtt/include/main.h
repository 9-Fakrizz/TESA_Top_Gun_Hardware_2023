#ifndef __MAIN_H__
#define __MAIN_H__

// include files
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
#include <Esp.h>
#include <esp_log.h>

// constants
#define TASK_MQTT_PRIO          2
#define TASK_SOUND_TYPE         0

#define WIFI_SSID       "Ok"
#define WIFI_PASSWORD   "q12345678"
#define MQTT_DEV_ID     666
#define MQTT_EVT_TOPIC  "tgr2023/bravely_divide/evt"
#define MQTT_CMD_TOPIC  "tgr2023/bravely_divide/cmd"

// type definitions
typedef struct evt_msg_t {
    int type;
    uint32_t timestamp;
    bool pressed;
    uint32_t value;
} evt_msg_t;

typedef struct evt_msg_sound{
    int type;
    uint32_t timestamp;
    bool sound;
    uint32_t duration;
} evt_msg_sound;

// shared variables
extern xQueueHandle evt_queue;
extern bool enable_flag;

// public function prototypes

#endif 