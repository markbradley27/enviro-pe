#ifndef ENVIRO_PE_SRC_UI_UI_MANAGER_H_
#define ENVIRO_PE_SRC_UI_UI_MANAGER_H_

#include "ui/screens/big_numbers.h"
#include "ui/screens/graphs.h"
#include "ui/screens/screen_manager.h"
#include "ui/screens/settings.h"

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
        pm25_5s_values_(pm25_5s_values), pm25_5m_avgs_(pm25_5m_avgs) {
    InitStyles();
    SwitchToBigNumbers();
    lv_scr_load(screen_manager_->screen);
  }

  // Call to indicate that new measurements were just taken and the UI should
  // update to reflect them.
  void UpdateMeasurements() { screen_manager_->UpdateMeasurements(); }

  void SwitchToBigNumbers() {
    ScreenManager *new_screen_manager =
        new BigNumbers(temp_c_5s_values_, temp_c_5m_avgs_, humid_5s_values_,
                       humid_5m_avgs_, pm25_5s_values_, pm25_5m_avgs_,
                       std::bind(&UiManager::SwitchToSettings, this),
                       std::bind(&UiManager::SwitchToGraphs, this));
    lv_scr_load(new_screen_manager->screen);
    delete screen_manager_;
    screen_manager_ = new_screen_manager;
  }

  void SwitchToSettings() {
    ScreenManager *new_screen_manager =
        new Settings(std::bind(&UiManager::SwitchToBigNumbers, this));
    lv_scr_load(new_screen_manager->screen);
    delete screen_manager_;
    screen_manager_ = new_screen_manager;
  }

  void SwitchToGraphs() {
    ScreenManager *new_screen_manager = new Graphs(
        std::bind(&UiManager::SwitchToBigNumbers, this));
    lv_scr_load(new_screen_manager->screen);
    delete screen_manager_;
    screen_manager_ = new_screen_manager;
  }

private:
  RingBuffer<float> *const temp_c_5s_values_;
  RingBuffer<float> *const temp_c_5m_avgs_;
  RingBuffer<float> *const humid_5s_values_;
  RingBuffer<float> *const humid_5m_avgs_;
  RingBuffer<uint16_t> *const pm25_5s_values_;
  RingBuffer<uint16_t> *const pm25_5m_avgs_;

  ScreenManager *screen_manager_ = NULL;
};

#endif // ENVIRO_PE_SRC_UI_UI_MANAGER_H_
