#include "main.h"
#include "task_detection.h"
#include "task_mqtt.h"
#include "net_mqtt.h"
#include "hw_camera.h"

// constants
#define TAG             "main"


#define BTN_PIN 0
#define EI_CAMERA_RAW_FRAME_BUFFER_COLS 240
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS 240
#define EI_CAMERA_FRAME_BYTE_SIZE 3
#define BMP_BUF_SIZE (EI_CAMERA_RAW_FRAME_BUFFER_COLS * EI_CAMERA_RAW_FRAME_BUFFER_ROWS * EI_CAMERA_FRAME_BYTE_SIZE)

// static function prototypes
void print_memory(void);
static void mqtt_callback(char* topic, byte* payload, unsigned int length);
static uint8_t *bmp_buf;
// static variables
bool enable_flag = true;
StaticJsonDocument<128> cmd_doc;

static char cmd_buf[128];
int date = 0;
float cubic = 0;

QueueHandle_t evt_queue;

// Setup hardware
void setup() {
  Serial.begin(115200);
  print_memory();
  evt_queue = xQueueCreate(3, sizeof(evt_msg_t));
  
  task_detection_init();
  task_mqtt_init(mqtt_callback);

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

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  ESP_LOGI(TAG, "Message arrived on topic %s", topic);
  ESP_LOGI(TAG, "Payload: %.*s", length, payload);
  memcpy(cmd_buf, payload, length);
  cmd_buf[length] = '\0';
  if (strcmp(cmd_buf, "cap") == 0) {
      ESP_LOGI(TAG, "yooooooooooooooooooohoooooo");
      trigger = true;    
  }
  deserializeJson(cmd_doc, cmd_buf);
  cubic = cmd_doc["cubic"];
  date = cmd_doc["date"];
  //xQueueSend(evt_queue, &evt_msg, portMAX_DELAY);
 }
// void mqtt_callback(char* topic, byte* payload, unsigned int length) {
//   ESP_LOGI(TAG, "Message arrived on topic %s", topic);
//   ESP_LOGI(TAG, "Payload: %.*s", length, payload);
//   memcpy(cmd_buf, payload, length);
//   cmd_buf[length] = '\0';
//   deserializeJson(cmd_doc, cmd_buf);
//   if (cmd_doc["ID"] == MQTT_DEV_ID) {
//     if (cmd_doc["enable"] == true) {
//       enable_flag = true;
//       ESP_LOGI(TAG, "Got enable command");
//     } else {
//       enable_flag = false;
//       ESP_LOGI(TAG, "Got disable command");
//     }
//   }
//  }