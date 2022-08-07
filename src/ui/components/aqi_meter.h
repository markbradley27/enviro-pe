#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_AQI_METER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_AQI_METER_H_

#include "ui/components/arc_meter.h"

class AqiMeter : public ArcMeter {
public:
  AqiMeter(lv_obj_t *parent) : ArcMeter(parent, 0, 50, PALETTE_, "PM2.5") {
    InitLabelStyle(&label_style_, PALETTE_);
  }

private:
  static const lv_palette_t PALETTE_ = LV_PALETTE_GREEN;

  static lv_style_t label_style_;
};

lv_style_t AqiMeter::label_style_;

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_AQI_METER_H_
