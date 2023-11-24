#ifndef __TASK_SOUND_H__
#define __TASK_SOUND_H__

// include files
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
#include <esp_log.h>
#include "hw_mic.h"
// shared variables

// public function prototypes
void task_sound_init();

#endif 