#ifndef ENVIRO_PE_SRC_UI_SCREENS_SETTINGS_H_
#define ENVIRO_PE_SRC_UI_SCREENS_SETTINGS_H_

#include "ui/screens/screen_manager.h"
#include "ui/styles.h"

static void settings_back_btn_handler(lv_event_t *e);

class Settings : public ScreenManager {
public:
  Settings(BrightnessManager *const brightness_manager,
           std::function<void()> switch_to_big_numbers)
      : brightness_manager_(brightness_manager),
        switch_to_big_numbers(switch_to_big_numbers) {
    menu = lv_menu_create(screen);
    lv_obj_set_size(menu, lv_pct(100), lv_pct(100));
    lv_menu_set_mode_root_back_btn(menu, LV_MENU_ROOT_BACK_BTN_ENABLED);
    lv_obj_add_event_cb(menu, settings_back_btn_handler, LV_EVENT_CLICKED,
                        this);

    // Display page
    lv_obj_t *display_page = lv_menu_page_create(menu, NULL);
    lv_obj_t *cont = lv_menu_cont_create(display_page);
    lv_obj_t *label = lv_label_create(cont);
    lv_label_set_text(label, "Brightness:");

    lv_obj_t *btn = lv_btn_create(cont);
    lv_obj_add_event_cb(btn, Settings::BrightnessDownHandler, LV_EVENT_CLICKED,
                        this);
    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_MINUS);
    brightness_label_ = lv_label_create(cont);
    lv_label_set_text_fmt(brightness_label_, "%d",
                          brightness_manager_->GetBrightness());
    btn = lv_btn_create(cont);
    lv_obj_add_event_cb(btn, Settings::BrightnessUpHandler, LV_EVENT_CLICKED,
                        this);
    label = lv_label_create(btn);
    lv_label_set_text(label, LV_SYMBOL_PLUS);

    // Time page
    lv_obj_t *time_page = lv_menu_page_create(menu, NULL);
    cont = lv_menu_cont_create(time_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Date:");

    // Main page
    lv_obj_t *main_page = lv_menu_page_create(menu, NULL);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Display");
    lv_menu_set_load_page_event(menu, cont, display_page);

    cont = lv_menu_cont_create(main_page);
    label = lv_label_create(cont);
    lv_label_set_text(label, "Time");
    lv_menu_set_load_page_event(menu, cont, time_page);

    lv_menu_set_page(menu, main_page);
  }

  static void BrightnessDownHandler(lv_event_t *e) {
    Settings *const settings = ((Settings *)lv_event_get_user_data(e));
    uint8_t brightness = settings->brightness_manager_->GetBrightness();
    if (brightness == 1) {
      return;
    }
    --brightness;
    settings->brightness_manager_->SetBrightness(brightness);
    lv_label_set_text_fmt(settings->brightness_label_, "%d", brightness);
  }

  static void BrightnessUpHandler(lv_event_t *e) {
    Settings *const settings = ((Settings *)lv_event_get_user_data(e));
    uint8_t brightness = settings->brightness_manager_->GetBrightness();
    if (brightness == 10) {
      return;
    }
    ++brightness;
    settings->brightness_manager_->SetBrightness(brightness);
    lv_label_set_text_fmt(settings->brightness_label_, "%d", brightness);
  }

  std::function<void()> switch_to_big_numbers;

  lv_obj_t *menu;

private:
  BrightnessManager *const brightness_manager_;

  lv_obj_t *brightness_label_;
};

static void settings_back_btn_handler(lv_event_t *e) {
  lv_obj_t *obj = lv_event_get_target(e);
  Settings *settings = (Settings *)lv_event_get_user_data(e);

  if (lv_menu_back_btn_is_root(settings->menu, obj)) {
    settings->switch_to_big_numbers();
  }
}

#endif // ENVIRO_PE_SRC_UI_SCREENS_SETTINGS_H_
