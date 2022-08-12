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
    lv_obj_set_layout(content, LV_LAYOUT_GRID);
    static const lv_coord_t column_dsc[4] = {
        LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    static const lv_coord_t row_dsc[2] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(content, column_dsc, row_dsc);

    lv_obj_t *left_cell = lv_obj_create(content);
    lv_obj_add_style(left_cell, &big_number_container_style, LV_PART_MAIN);
    lv_obj_set_grid_cell(left_cell, LV_GRID_ALIGN_CENTER, 0, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);
    temp_ = new TempBigNumber(left_cell, temp_c_values);

    lv_obj_t *mid_cell = lv_obj_create(content);
    lv_obj_add_style(mid_cell, &big_number_container_style, LV_PART_MAIN);
    lv_obj_set_grid_cell(mid_cell, LV_GRID_ALIGN_CENTER, 1, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);
    humid_ = new HumidBigNumber(mid_cell, humid_values_);

    lv_obj_t *right_cell = lv_obj_create(content);
    lv_obj_add_style(right_cell, &big_number_container_style, LV_PART_MAIN);
    lv_obj_set_grid_cell(right_cell, LV_GRID_ALIGN_CENTER, 2, 1,
                         LV_GRID_ALIGN_CENTER, 0, 1);
    aqi_ = new AqiBigNumber(right_cell, pm25_values_);
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
  /*
  static constexpr lv_coord_t COLUMN_DSC[4] = {320 / 3, 320 / 3, 320 / 3,
                                               LV_GRID_TEMPLATE_LAST};
  static constexpr lv_coord_t ROW_DSC[2] = {240 - HEADER_HEIGHT,
                                            LV_GRID_TEMPLATE_LAST};
                                            */

  RingBuffer<float> *const temp_c_values_;
  RingBuffer<float> *const humid_values_;
  RingBuffer<uint16_t> *const pm25_values_;

  TempBigNumber *temp_ = NULL;
  HumidBigNumber *humid_ = NULL;
  AqiBigNumber *aqi_ = NULL;
};

#endif // ENVIRO_PE_SRC_UI_SCREENS_BIG_NUMBERS_H_
