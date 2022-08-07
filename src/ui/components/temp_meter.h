#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_TEMP_METER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_TEMP_METER_H_

#include "ui/components/arc_meter.h"

class TempMeter : public ArcMeter {
public:
  TempMeter(lv_obj_t *parent) : ArcMeter(parent, 0, 100, PALETTE_, "Temp") {
    InitLabelStyle(&label_style_, PALETTE_);
  }

private:
  static const lv_palette_t PALETTE_ = LV_PALETTE_RED;

  static lv_style_t label_style_;
};

lv_style_t TempMeter::label_style_;

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_TEMP_METER_H_
