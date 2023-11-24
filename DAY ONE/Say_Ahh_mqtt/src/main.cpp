#include "main.h"
#include "task_mqtt.h"
#include "net_mqtt.h"
#include <Arduino.h>
#include <esp_log.h>
#include "hw_mic.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "task_sound.h"

#define TAG "main"

#define STACK_SIZE 4096
#define THRESHOLD 10
#define SOUND_DURATION_THRESHOLD 1000
#define SCALE 1e6
#define NUM_SAMPLES  160
#define SAMPLE_RATE 16000
static char cmd_buf[128];
static unsigned int num_samples = NUM_SAMPLES;
static int32_t samples[160];

// constants
#define TAG             "main"

// static function prototypes
void print_memory(void);
static void mqtt_callback(char* topic, byte* payload, unsigned int length);
// static variables
bool enable_flag = true;

StaticJsonDocument<128> cmd_doc;
QueueHandle_t evt_queue;

// Setup hardware
void setup() {
  Serial.begin(115200);
  //print_memory();
  evt_queue = xQueueCreate(3, sizeof(evt_msg_t));
  task_mqtt_init(mqtt_callback);
  hw_mic_init(SAMPLE_RATE);
  task_sound_init();
}

// Main loop
void loop() {
  net_mqtt_loop();
  delay(100);
}

// Print memory information
void print_memory() {
  ESP_LOGI(TAG, "Total heap: %u", ESP.getHeapSize());
  ESP_LOGI(TAG, "Free heap: %u", ESP.getFreeHeap());
  ESP_LOGI(TAG, "Total PSRAM: %u", ESP.getPsramSize());
  ESP_LOGI(TAG, "Free PSRAM: %d", ESP.getFreePsram());
}

// callback function to handle MQTT message
 void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  ESP_LOGI(TAG, "Message arrived on topic %s", topic);
  ESP_LOGI(TAG, "Payload: %.*s", length, payload);
  memcpy(cmd_buf, payload, length);
  cmd_buf[length] = '\0';
  deserializeJson(cmd_doc, cmd_buf);
  if (cmd_doc["ID"] == MQTT_DEV_ID) {
    if (cmd_doc["enable"] == true) {
      enable_flag = true;
      ESP_LOGI(TAG, "Got enable command");
    } else {
      enable_flag = false;
      ESP_LOGI(TAG, "Got disable command");
    }
  }
 }