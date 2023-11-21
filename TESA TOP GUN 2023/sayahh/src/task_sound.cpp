#include "hw_mic.h"
#include "main.h"
#include "task_sound.h"
#include <esp_log.h>
#include "net_mqtt.h"

#define TAG "task_sound"

#define THRESHOLD 1
#define SAMPLE_RATE 16000
#define SCALE 1e6
#define NUM_SAMPLES  160
static unsigned int num_samples = NUM_SAMPLES;
static int32_t samples[160];
unsigned long soundStartTime = 0;
bool soundDetected = false;
float w_mean[10];
float volm = 0;
int i = 0;
int count = 0;

static void task_sound_fcn(void* arg);

// task initialization
void task_sound_init() {
    xTaskCreate(
      task_sound_fcn,
      "sound",
      2048,
      NULL,
      1,
      NULL
    ); 
}

// task function
void task_sound_fcn(void* arg) {
    for (;;) {
         evt_msg_sound evt_sound = {
            .type = TASK_SOUND_TYPE,
            .timestamp = count,
            .sound = false,
            .duration = 0
        };

        //ESP_LOGI(TAG, "loop at %d", millis());
        num_samples = NUM_SAMPLES;
        hw_mic_read(samples, &num_samples);
        float sample_avg = 0;

        for (int i = 0; i < num_samples; i++) {
            sample_avg += (float) samples[i] / SCALE;
        }
        sample_avg = abs(sample_avg)/500;
        w_mean[i] = sample_avg;
        i++;
        if(i == 10){
            for (int i = 0; i < 10; i++) {
                sample_avg += (float) w_mean[i];
            }
            volm = sample_avg/10;
            i=0;         
        }
        //Serial.println(volm);
       
        if (volm > THRESHOLD) {
            if (!soundDetected) {
                // Sound event started
                soundDetected = true;
                soundStartTime = millis();
                //ESP_LOGI(TAG, "Sound event started at %lu", soundStartTime);
               // Serial.println("SOUNDDDDDDDD");
                evt_sound.sound = true; 
                count++;
                xQueueSend(evt_queue, &evt_sound, portMAX_DELAY);
            }
        } else {
            if (soundDetected) {
                // Sound event ended
                unsigned long soundEndTime = millis();
                unsigned long soundDuration = soundEndTime - soundStartTime;
                //ESP_LOGI(TAG, "Sound event ended at %lu. Duration: %lu ms", soundEndTime, soundDuration);
                Serial.print("DURATION : ");
                Serial.println(soundDuration);
                evt_sound.sound = false;
                evt_sound.duration = soundDuration;
                xQueueSend(evt_queue, &evt_sound, portMAX_DELAY);
                soundDetected = false;
                soundStartTime = 0;
            }
        }
        delay(1);
    }
}