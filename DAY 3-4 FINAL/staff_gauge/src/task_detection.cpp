#include "main.h"
#include "task_detection.h"
#include "net_mqtt.h"
#include "waterwTER_inferencing.h"
#include "edge-impulse-sdk/dsp/image/image.hpp"

// constants
#define TAG "task_detection"

#define BTN_PIN 0
#define EI_CAMERA_RAW_FRAME_BUFFER_COLS 240
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS 240
#define EI_CAMERA_FRAME_BYTE_SIZE 3
#define BMP_BUF_SIZE (EI_CAMERA_RAW_FRAME_BUFFER_COLS * EI_CAMERA_RAW_FRAME_BUFFER_ROWS * EI_CAMERA_FRAME_BYTE_SIZE)

static void obj_detection_fcn(void* arg);
static evt_msg_t evt_msg;
// static variables
static uint8_t *bmp_buf;
bool trigger = false;
// static function prototypes
void print_memory(void);
void ei_prepare_feature(uint8_t *img_buf, signal_t *signal);
int ei_get_feature_callback(size_t offset, size_t length, float *out_ptr);
void ei_use_result(ei_impulse_result_t result);

void task_detection_init() {
    xTaskCreate(
        obj_detection_fcn,          /* Task function. */
        "task detection",            /* String with name of task. */
        4096,                   /* Stack size in bytes. */
        NULL,   /* Parameter passed as input of the task */
        2,         /* Priority of the task. */
        NULL);                  /* Task handle. */
    ESP_LOGI(TAG, "task_detection created at %d", millis());
}
// main loop
void obj_detection_fcn(void* arg){
  hw_camera_init();
  trigger = false;
  bmp_buf = (uint8_t *)ps_malloc(BMP_BUF_SIZE);
  if (psramInit())
  {
      ESP_LOGI(TAG, "PSRAM initialized");
  }
  else
  {
      ESP_LOGE(TAG, "PSRAM not available");
  }
  for(;;){
    static bool press_state = false;
    static uint32_t prev_millis = 0;
    
    if (digitalRead(BTN_PIN) == 0 || trigger == true  )
    {
      if ((millis() - prev_millis > 500) && (press_state == false))
      {
        uint32_t Tstart, elapsed_time;
        uint32_t width, height;
        trigger = false;
        prev_millis = millis();
        Tstart = millis();
        // get raw data
        ESP_LOGI(TAG, "Taking snapshot...");
        // use raw bmp image
        hw_camera_raw_snapshot(bmp_buf, &width, &height);

        elapsed_time = millis() - Tstart;
        ESP_LOGI(TAG, "Snapshot taken (%d) width: %d, height: %d", elapsed_time, width, height);
        print_memory();
        // prepare feature
        Tstart = millis();
        ei::signal_t signal;
        // generate feature
        ei_prepare_feature(bmp_buf, &signal);

        elapsed_time = millis() - Tstart;
        ESP_LOGI(TAG, "Feature taken (%d)", elapsed_time);
        print_memory();
        // run classifier
        Tstart = millis();
        ei_impulse_result_t result = {0};
        bool debug_nn = false;
        // run classifier
        run_classifier(&signal, &result, debug_nn);

        elapsed_time = millis() - Tstart;
        ESP_LOGI(TAG, "Classification done (%d)", elapsed_time);
        print_memory();
        // use result
        ei_use_result(result);

        press_state = true;
      }
    }
    else
    {
      if (press_state)
      {
        press_state = false;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(50)); 
  }
}

// prepare feature
void ei_prepare_feature(uint8_t *img_buf, signal_t *signal)
{
  signal->total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
  signal->get_data = &ei_get_feature_callback;
  if ((EI_CAMERA_RAW_FRAME_BUFFER_ROWS != EI_CLASSIFIER_INPUT_WIDTH) || (EI_CAMERA_RAW_FRAME_BUFFER_COLS != EI_CLASSIFIER_INPUT_HEIGHT))
  {
    ei::image::processing::crop_and_interpolate_rgb888(
        img_buf,
        EI_CAMERA_RAW_FRAME_BUFFER_COLS,
        EI_CAMERA_RAW_FRAME_BUFFER_ROWS,
        img_buf,
        EI_CLASSIFIER_INPUT_WIDTH,
        EI_CLASSIFIER_INPUT_HEIGHT);
  }
}

// get feature callback
int ei_get_feature_callback(size_t offset, size_t length, float *out_ptr)
{
  size_t pixel_ix = offset * 3;
  size_t pixels_left = length;
  size_t out_ptr_ix = 0;

  while (pixels_left != 0)
  {
    out_ptr[out_ptr_ix] = (bmp_buf[pixel_ix] << 16) + (bmp_buf[pixel_ix + 1] << 8) + bmp_buf[pixel_ix + 2];

    // go to the next pixel
    out_ptr_ix++;
    pixel_ix += 3;
    pixels_left--;
  }
  return 0;
}

// use result from classifier
void ei_use_result(ei_impulse_result_t result) {
  evt_msg_t evt_msg = {
            .date = date,
            .cubic = cubic
        };
  ESP_LOGI(TAG, "Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
  result.timing.dsp, result.timing.classification, result.timing.anomaly);
  bool bb_found = result.bounding_boxes[0].value > 0;
  auto bb = result.bounding_boxes[0];
  if(bb_found) {
    ESP_LOGI(TAG, "VALID");
  }else{
    ESP_LOGI(TAG, "INVALID!!");
  }

  // for (size_t ix = 0; ix < result.bounding_boxes_count; ix++) {
  //   auto bb = result.bounding_boxes[ix];
  //   if (bb.value == 0) {
  //     continue;
  //   }
  //   ESP_LOGI(TAG, "%s (%f) [ x: %u, y: %u, width: %u, height: %u ]", bb.label, bb.value, bb.x, bb.y, bb.width, bb.height);
  //   count++;
  //   if(bb.label)
  //   evt_msg.count = count;
  //   evt_msg.value = 
  // }
  if(bb.label == "108") evt_msg.value = 108.1;
  if(bb.label == "109") evt_msg.value = 109.4;
  if(bb.label == "110") evt_msg.value = 110.6;
  if(bb.label == "111") evt_msg.value = 111.2;
  if(bb.label == "112") evt_msg.value = 112.7;
  if(bb.label == "113") evt_msg.value = 113.3;
  if(bb.label == "114") evt_msg.value = 114.5;
  if(bb.label == "115") evt_msg.value = 115.8;
  if(bb.label == "116") evt_msg.value = 116.0;
  if(bb.label == "117") evt_msg.value = 117.9;
  if(bb.label == "118") evt_msg.value = 118.5;
  if(bb.label == "119") evt_msg.value = 119.8;
  ESP_LOGI(TAG, "%s (%f) [ x: %u, y: %u, width: %u, height: %u ]", bb.label, bb.value, bb.x, bb.y, bb.width, bb.height);
  xQueueSend(evt_queue, &evt_msg, portMAX_DELAY);

}