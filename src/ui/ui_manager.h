#ifndef ENVIRO_PE_SRC_UI_UI_MANAGER_H_
#define ENVIRO_PE_SRC_UI_UI_MANAGER_H_

#include "ui/screens/big_numbers.h"
#include "ui/screens/screen_manager.h"

class UiManager {
public:
  UiManager(RingBuffer<float> *temp_c_5s_values,
            RingBuffer<float> *temp_c_5m_avgs,

            RingBuffer<float> *humid_5s_values,
            RingBuffer<float> *humid_5m_avgs,
            RingBuffer<uint16_t> *pm25_5s_values,
            RingBuffer<uint16_t> *pm25_5m_avgs)
      : temp_c_5s_values_(temp_c_5s_values), temp_c_5m_avgs_(temp_c_5m_avgs),
        humid_5s_values_(humid_5s_values), humid_5m_avgs_(humid_5m_avgs),
        pm25_5s_values_(pm25_5s_values), pm25_5m_avgs_(pm25_5m_avgs),
        screen_manager_(new BigNumbers(temp_c_5s_values, temp_c_5m_avgs,
                                       humid_5s_values, humid_5m_avgs,
                                       pm25_5s_values, pm25_5m_avgs)) {
    InitStyles();
    lv_scr_load(screen_manager_->screen);
  }

  // Call to indicate that new measurements were just taken and the UI should
  // update to reflect them.
  void UpdateMeasurements() { screen_manager_->UpdateMeasurements(); }

private:
  RingBuffer<float> *const temp_c_5s_values_;
  RingBuffer<float> *const temp_c_5m_avgs_;
  RingBuffer<float> *const humid_5s_values_;
  RingBuffer<float> *const humid_5m_avgs_;
  RingBuffer<uint16_t> *const pm25_5s_values_;
  RingBuffer<uint16_t> *const pm25_5m_avgs_;

  ScreenManager *screen_manager_;
};

#endif // ENVIRO_PE_SRC_UI_UI_MANAGER_H_
