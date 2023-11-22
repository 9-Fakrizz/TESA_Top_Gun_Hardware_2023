#include <Arduino.h>
#include "main.h"
#define TAG "main"
#include <esp_log.h>
#include "hw_mic.h"
#define SAMPLE_RATE 16000
#define NUM_SAMPLES  160
static unsigned int num_samples = NUM_SAMPLES;
#define SCALE 1e6
static int32_t samples[160];

void setup() {
Serial.begin(115200);
hw_mic_init(SAMPLE_RATE);
}
void loop() {
ESP_LOGI(TAG,"loop at %d",millis());
num_samples = NUM_SAMPLES ;
hw_mic_read(samples,&num_samples);
float sample_avg = 0;
for (int i=0; i< num_samples; i++ ){
  sample_avg += (float)samples[i]/SCALE;
}
Serial.println(sample_avg/num_samples);
delay(1);
}