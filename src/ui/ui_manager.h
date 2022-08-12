#ifndef ENVIRO_PE_SRC_UI_UI_MANAGER_H_
#define ENVIRO_PE_SRC_UI_UI_MANAGER_H_

#include "ui/screens/big_numbers.h"
#include "ui/screens/screen_manager.h"

class UiManager {
public:
  UiManager(RingBuffer<float> *temp_c_values, RingBuffer<float> *humid_values,
            RingBuffer<uint16_t> *pm25_values)
      : temp_c_values_(temp_c_values), humid_values_(humid_values),
        pm25_values_(pm25_values),
        screen_manager_(
            new BigNumbers(temp_c_values, humid_values, pm25_values)) {
    InitStyles();
    lv_scr_load(screen_manager_->screen);
  }

  // Call to indicate that new measurements were just taken and the UI should
  // update to reflect them.
  void UpdateMeasurements() { screen_manager_->UpdateMeasurements(); }

private:
  RingBuffer<float> *const temp_c_values_;
  RingBuffer<float> *const humid_values_;
  RingBuffer<uint16_t> *const pm25_values_;

  ScreenManager *screen_manager_;
};

#endif // ENVIRO_PE_SRC_UI_UI_MANAGER_H_
