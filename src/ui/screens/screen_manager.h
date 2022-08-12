#ifndef ENVIRO_PE_SRC_UI_SCREENS_SCREEN_MANAGER_H_
#define ENVIRO_PE_SRC_UI_SCREENS_SCREEN_MANAGER_H_

#include "lvgl.h"

class ScreenManager {
public:
  ScreenManager() : screen(lv_obj_create(NULL)) {}

  // Call to indicate that new measurements were just taken and the UI should
  // update to reflect them.
  virtual void UpdateMeasurements() = 0;

  lv_obj_t *screen;
};

#endif // ENVIRO_PE_SRC_UI_SCREENS_SCREEN_MANAGER_H_
