#ifndef ENVIRO_PE_SRC_UI_SCREENS_ARC_DASH_H_
#define ENVIRO_PE_SRC_UI_SCREENS_ARC_DASH_H_

#include "lvgl.h"
#include "ring_buffer.h"
#include "ui/components/aqi_meter.h"
#include "ui/components/humid_meter.h"
#include "ui/components/temp_meter.h"
#include "util.h"

class ArcDash {
public:
  ArcDash(RingBuffer<float> *temp_c_values, RingBuffer<float> *humid_values,
          RingBuffer<uint16_t> *pm25_values)
      : temp_c_values_(temp_c_values), humid_values_(humid_values),
        pm25_values_(pm25_values), screen_(lv_obj_create(NULL)),
        temp_meter_(screen_), humid_meter_(screen_), aqi_meter_(screen_) {

    lv_obj_align(temp_meter_.meter, LV_ALIGN_CENTER, -ARC_X_OFFSET,
                 ARC_Y_OFFSET);
    lv_obj_align(humid_meter_.meter, LV_ALIGN_CENTER, ARC_X_OFFSET,
                 ARC_Y_OFFSET);
    lv_obj_align(aqi_meter_.meter, LV_ALIGN_CENTER, 0, -ARC_Y_OFFSET);
  }

  lv_obj_t *Screen() { return screen_; }

  void Update() {
    const float temp_f =
        CToF(temp_c_values_->Average(minutes(MOVING_AVERAGE_MINUTES)));
    lv_meter_set_indicator_end_value(temp_meter_.meter, temp_meter_.arc,
                                     temp_f);
    lv_label_set_text_fmt(temp_meter_.label, "%.0f°", temp_f);
    const float humidity =
        humid_values_->Average(minutes(MOVING_AVERAGE_MINUTES));
    lv_meter_set_indicator_end_value(humid_meter_.meter, humid_meter_.arc,
                                     humidity);
    lv_label_set_text_fmt(humid_meter_.label, "%.0f%%", humidity);
    const uint16_t pm25 =
        pm25_values_->Average(minutes(MOVING_AVERAGE_MINUTES));
    lv_meter_set_indicator_end_value(aqi_meter_.meter, aqi_meter_.arc, pm25);
    lv_label_set_text_fmt(aqi_meter_.label, "%d", pm25);
  }

private:
  static const uint8_t MOVING_AVERAGE_MINUTES = 2;
  static const uint8_t ARC_X_OFFSET = 93;
  static const uint8_t ARC_Y_OFFSET = 50;

  RingBuffer<float> *temp_c_values_;
  RingBuffer<float> *humid_values_;
  RingBuffer<uint16_t> *pm25_values_;

  lv_obj_t *screen_;

  TempMeter temp_meter_;
  HumidMeter humid_meter_;
  AqiMeter aqi_meter_;
};

#endif // ENVIRO_PE_SRC_UI_SCREENS_ARC_DASH_H_
