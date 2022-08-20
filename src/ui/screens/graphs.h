#ifndef ENVIRO_PE_SRC_UI_SCREENS_GRAPHS_H_
#define ENVIRO_PE_SRC_UI_SCREENS_GRAPHS_H_

#include "ui/screens/screen_manager.h"

static void graphs_back_btn_handler(lv_event_t *e);

class Graphs : public ScreenManager {
public:
  Graphs(std::function<void()> switch_to_big_numbers)
      : switch_to_big_numbers(switch_to_big_numbers) {
    lv_obj_t *window = lv_win_create(screen, HEADER_HEIGHT);

    lv_obj_t *back_btn = lv_win_add_btn(window, LV_SYMBOL_LEFT, HEADER_HEIGHT);
    lv_obj_add_event_cb(back_btn, graphs_back_btn_handler, LV_EVENT_CLICKED,
                        this);

    lv_win_add_title(window, "Graphs");
  }

  void UpdateMeasurements() override{};

  std::function<void()> switch_to_big_numbers;
};

static void graphs_back_btn_handler(lv_event_t *e) {
  ((Graphs *)lv_event_get_user_data(e))->switch_to_big_numbers();
}

#endif // ENVIRO_PE_SRC_UI_SCREENS_GRAPHS_H_
