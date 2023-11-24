#include "main.h"
#include "task_mqtt.h"
#include "net_mqtt.h"

// constants
#define TAG             "task_mqtt"

// static variables
StaticJsonDocument<128> evt_doc;

static char evt_buf[128];

// static function prototypes
static void task_mqtt_fcn(void* arg);

// task initialization
void task_mqtt_init(mqtt_callback_t mqtt_callback) {
    xTaskCreate(
        task_mqtt_fcn,          /* Task function. */
        "MQTT Task",            /* String with name of task. */
        4096,                   /* Stack size in bytes. */
        (void*)mqtt_callback,   /* Parameter passed as input of the task */
        1,         /* Priority of the task. */
        NULL);                  /* Task handle. */
    ESP_LOGI(TAG, "task_mqtt created at %d", millis());
}

// task function
void task_mqtt_fcn(void* arg) {
    // task initialization
    net_mqtt_init(WIFI_SSID, WIFI_PASSWORD);
    net_mqtt_connect(MQTT_DEV_ID, MQTT_EVT_TOPIC, (mqtt_callback_t)arg);
    while(1) {
        static evt_msg_t evt_msg;
        // task function
        xQueueReceive(evt_queue, &evt_msg, portMAX_DELAY);
        ESP_LOGI(TAG, "task_mqtt run at %d", millis());
        evt_doc.clear();
        evt_doc["date"] = evt_msg.date;
        evt_doc["waterlevel"] = evt_msg.value;
        evt_doc["cubic"] = evt_msg.cubic;
        serializeJson(evt_doc, evt_buf);

        if (evt_buf[0] != 0) {
            net_mqtt_publish(MQTT_EVT_TOPIC, evt_buf);
        }
    }
}