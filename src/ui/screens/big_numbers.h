#ifndef ENVIRO_PE_SRC_UI_SCREENS_BIG_NUMBERS_H_
#define ENVIRO_PE_SRC_UI_SCREENS_BIG_NUMBERS_H_

#include "lvgl.h"
#include "ring_buffer.h"
#include "ui/components/aqi_big_number.h"
#include "ui/components/humid_big_number.h"
#include "ui/components/temp_big_number.h"
#include "ui/screens/screen_manager.h"
#include "ui/styles.h"

class BigNumbers : public ScreenManager {
public:
  BigNumbers(RingBuffer<float> *temp_c_values, RingBuffer<float> *humid_values,
             RingBuffer<uint16_t> *pm25_values)
      : temp_c_values_(temp_c_values), humid_values_(humid_values),
        pm25_values_(pm25_values) {

    lv_obj_t *window = lv_win_create(screen, HEADER_HEIGHT);

    lv_win_add_title(window, "Big Numbers");

    lv_obj_t *content = lv_win_get_content(window);
    lv_obj_add_style(content, &big_number_content_style, LV_PART_MAIN);

    lv_obj_t *temp_obj = lv_obj_create(content);
    lv_obj_add_style(temp_obj, &big_number_container_style, LV_PART_MAIN);
    lv_obj_align(temp_obj, LV_ALIGN_CENTER, 0, 0);
    temp_ = new TempBigNumber(temp_obj, temp_c_values);

    lv_obj_t *humid_obj = lv_obj_create(content);
    lv_obj_add_style(humid_obj, &big_number_container_style, LV_PART_MAIN);
    lv_obj_align(humid_obj, LV_ALIGN_CENTER, -X_OFFSET, 0);
    humid_ = new HumidBigNumber(humid_obj, humid_values_);

    lv_obj_t *aqi_obj = lv_obj_create(content);
    lv_obj_add_style(aqi_obj, &big_number_container_style, LV_PART_MAIN);
    lv_obj_align(aqi_obj, LV_ALIGN_CENTER, X_OFFSET, 0);
    aqi_ = new AqiBigNumber(aqi_obj, pm25_values_);
  }

  ~BigNumbers() {
    if (temp_ != NULL) {
      delete temp_;
    }
    if (humid_ != NULL) {
      delete humid_;
    }
    if (aqi_ != NULL) {
      delete aqi_;
    }
  }

  void UpdateMeasurements() {
    temp_->Update();
    humid_->Update();
    aqi_->Update();
  }

private:
  static const uint8_t HEADER_HEIGHT = 40;
  static const uint8_t X_OFFSET = 106;

  RingBuffer<float> *const temp_c_values_;
  RingBuffer<float> *const humid_values_;
  RingBuffer<uint16_t> *const pm25_values_;

  TempBigNumber *temp_ = NULL;
  HumidBigNumber *humid_ = NULL;
  AqiBigNumber *aqi_ = NULL;
};

#endif // ENVIRO_PE_SRC_UI_SCREENS_BIG_NUMBERS_H_
