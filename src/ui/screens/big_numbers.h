#ifndef ENVIRO_PE_SRC_UI_SCREENS_BIG_NUMBERS_H_
#define ENVIRO_PE_SRC_UI_SCREENS_BIG_NUMBERS_H_

#include "lvgl.h"
#include "ring_buffer.h"
#include "ui/components/aqi_big_number.h"
#include "ui/components/humid_big_number.h"
#include "ui/components/temp_big_number.h"
#include "ui/screens/screen_manager.h"
#include "ui/styles.h"

static void settings_btn_handler(lv_event_t *e);
static void aqi_big_number_handler(lv_event_t *e);
static void humid_big_number_handler(lv_event_t *e);
static void temp_big_number_handler(lv_event_t *e);

class BigNumbers : public ScreenManager {
public:
  BigNumbers(RingBuffer<float> *temp_c_5s_values,
             RingBuffer<float> *temp_c_5m_avgs,
             RingBuffer<float> *humid_5s_values,
             RingBuffer<float> *humid_5m_avgs,
             RingBuffer<uint16_t> *pm25_5s_values,
             RingBuffer<uint16_t> *pm25_5m_avgs,
             std::function<void()> switch_to_settings,
             std::function<void()> switch_to_aqi_graph,
             std::function<void()> switch_to_humid_graph,
             std::function<void()> switch_to_temp_graph)
      : temp_c_5s_values_(temp_c_5s_values), temp_c_5m_avgs_(temp_c_5m_avgs),
        humid_5s_values_(humid_5s_values), humid_5m_avgs_(humid_5m_avgs),
        pm25_5s_values_(pm25_5s_values), pm25_5m_avgs_(pm25_5m_avgs),
        switch_to_settings(switch_to_settings),
        switch_to_aqi_graph(switch_to_aqi_graph),
        switch_to_humid_graph(switch_to_humid_graph),
        switch_to_temp_graph(switch_to_temp_graph) {

    lv_obj_t *window = lv_win_create(screen, HEADER_HEIGHT);

    lv_obj_t *settings_btn =
        lv_win_add_btn(window, LV_SYMBOL_SETTINGS, HEADER_HEIGHT);
    lv_obj_add_event_cb(settings_btn, settings_btn_handler, LV_EVENT_CLICKED,
                        this);

    lv_obj_t *content = lv_win_get_content(window);
    lv_obj_add_style(content, &big_number_content_style, LV_PART_MAIN);

    lv_obj_t *temp_obj = lv_obj_create(content);
    lv_obj_add_style(temp_obj, &big_number_container_style, LV_PART_MAIN);
    lv_obj_align(temp_obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(temp_obj, temp_big_number_handler, LV_EVENT_CLICKED,
                        this);
    temp_ = new TempBigNumber(temp_obj, temp_c_5s_values, temp_c_5m_avgs);

    lv_obj_t *humid_obj = lv_obj_create(content);
    lv_obj_add_style(humid_obj, &big_number_container_style, LV_PART_MAIN);
    lv_obj_align(humid_obj, LV_ALIGN_CENTER, -X_OFFSET, 0);
    lv_obj_add_event_cb(humid_obj, humid_big_number_handler, LV_EVENT_CLICKED,
                        this);
    humid_ = new HumidBigNumber(humid_obj, humid_5s_values, humid_5m_avgs);

    lv_obj_t *aqi_obj = lv_obj_create(content);
    lv_obj_add_style(aqi_obj, &big_number_container_style, LV_PART_MAIN);
    lv_obj_align(aqi_obj, LV_ALIGN_CENTER, X_OFFSET, 0);
    lv_obj_add_event_cb(aqi_obj, aqi_big_number_handler, LV_EVENT_CLICKED,
                        this);
    aqi_ = new AqiBigNumber(aqi_obj, pm25_5s_values, pm25_5m_avgs);
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

  std::function<void()> switch_to_settings;
  std::function<void()> switch_to_aqi_graph;
  std::function<void()> switch_to_humid_graph;
  std::function<void()> switch_to_temp_graph;

private:
  static const uint8_t X_OFFSET = 106;

  RingBuffer<float> *const temp_c_5s_values_;
  RingBuffer<float> *const temp_c_5m_avgs_;
  RingBuffer<float> *const humid_5s_values_;
  RingBuffer<float> *const humid_5m_avgs_;
  RingBuffer<uint16_t> *const pm25_5s_values_;
  RingBuffer<uint16_t> *const pm25_5m_avgs_;

  TempBigNumber *temp_ = NULL;
  HumidBigNumber *humid_ = NULL;
  AqiBigNumber *aqi_ = NULL;
};

static void settings_btn_handler(lv_event_t *e) {
  ((BigNumbers *)lv_event_get_user_data(e))->switch_to_settings();
}

static void aqi_big_number_handler(lv_event_t *e) {
  ((BigNumbers *)lv_event_get_user_data(e))->switch_to_aqi_graph();
}

static void humid_big_number_handler(lv_event_t *e) {
  ((BigNumbers *)lv_event_get_user_data(e))->switch_to_humid_graph();
}

static void temp_big_number_handler(lv_event_t *e) {
  ((BigNumbers *)lv_event_get_user_data(e))->switch_to_temp_graph();
}

#endif // ENVIRO_PE_SRC_UI_SCREENS_BIG_NUMBERS_H_
