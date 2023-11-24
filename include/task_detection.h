#ifndef __TASK_DETECTION_H__
#define __TASK_DETECTION_H__

// include files
#include "main.h"
#include <Arduino.h>
#include "hw_camera.h"
// add header file of Edge Impulse firmware
#include <FreeRTOS.h>
#include <esp_log.h>

// shared variables

// public function prototypes
void task_detection_init(void);

#endif  // __TASK_BUTTON_H__