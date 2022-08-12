#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_BIG_NUMBER_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_BIG_NUMBER_H_

#include "lvgl.h"

#include "ui/styles.h"

class BigNumber {
public:
  BigNumber(lv_obj_t *const parent, const char *title) {
    big_number_ = lv_label_create(parent);
    lv_obj_add_style(big_number_, &big_number_style, LV_PART_MAIN);
    lv_obj_align(big_number_, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t *title_label = lv_label_create(parent);
    lv_label_set_text(title_label, title);
    lv_obj_align(title_label, LV_ALIGN_TOP_MID, 0, 20);
  }

protected:
  lv_obj_t *big_number_;
};

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_BIG_NUMBER_H_
