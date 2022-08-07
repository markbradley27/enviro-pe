#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_HUMID_METER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_HUMID_METER_H_

#include "ui/components/arc_meter.h"

class HumidMeter : public ArcMeter {
public:
  HumidMeter(lv_obj_t *parent) : ArcMeter(parent, 0, 100, PALETTE_, "Humid") {
    InitLabelStyle(&label_style_, PALETTE_);
  }

private:
  static const lv_palette_t PALETTE_ = LV_PALETTE_BLUE;

  static lv_style_t label_style_;
};

lv_style_t HumidMeter::label_style_;

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_HUMID_METER_H_
