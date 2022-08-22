#ifndef ENVIRO_PE_SRC_UI_SCREENS_SCREEN_MANAGER_H_
#define ENVIRO_PE_SRC_UI_SCREENS_SCREEN_MANAGER_H_

#include "lvgl.h"

class ScreenManager {
public:
  ScreenManager() : screen(lv_obj_create(NULL)) {}
  virtual ~ScreenManager() { lv_obj_del_async(screen); }

  // Call to indicate that new measurements were just taken and the UI should
  // update to reflect them.
  virtual void UpdateMeasurements(){};

  // Call to update any time displays.
  virtual void UpdateTime(){};

  lv_obj_t *screen;

protected:
  // Header height to be used for all windows.
  static const uint8_t HEADER_HEIGHT = 40;
};

#endif // ENVIRO_PE_SRC_UI_SCREENS_SCREEN_MANAGER_H_
