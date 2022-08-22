#ifndef ENVIRO_PE_SRC_UI_SCREENS_GRAPHS_H_
#define ENVIRO_PE_SRC_UI_SCREENS_GRAPHS_H_

#include "ui/screens/screen_manager.h"
#include "ui/styles.h"

static void graphs_back_btn_handler(lv_event_t *e);
static void graphs_zoom_out_btn_handler(lv_event_t *e);
static void graphs_zoom_in_btn_handler(lv_event_t *e);

static void graphs_draw_part_cb(lv_event_t *e) {
  lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
  if (!lv_obj_draw_part_check_type(dsc, &lv_chart_class,
                                   LV_CHART_DRAW_PART_TICK_LABEL)) {
    return;
  }

  if (dsc->id == LV_CHART_AXIS_PRIMARY_X && dsc->text) {
    lv_snprintf(dsc->text, dsc->text_length, "%i", -24 + 2 * dsc->value);
  }
}

class Graphs : public ScreenManager {
public:
  Graphs(RingBuffer<float> *const temp_c_5m_avgs,
         std::function<void()> switch_to_big_numbers)
      : temp_c_5m_avgs_(temp_c_5m_avgs),
        switch_to_big_numbers(switch_to_big_numbers) {
    lv_obj_t *window = lv_win_create(screen, HEADER_HEIGHT);

    lv_obj_t *back_btn = lv_win_add_btn(window, LV_SYMBOL_LEFT, HEADER_HEIGHT);
    lv_obj_add_event_cb(back_btn, graphs_back_btn_handler, LV_EVENT_CLICKED,
                        this);

    lv_obj_t *title = lv_win_add_title(window, "Graphs");
    lv_obj_add_style(title, &graph_title_style, LV_PART_MAIN);

    lv_obj_t *zoom_out_btn =
        lv_win_add_btn(window, LV_SYMBOL_MINUS, HEADER_HEIGHT);
    lv_obj_add_event_cb(zoom_out_btn, graphs_zoom_out_btn_handler,
                        LV_EVENT_CLICKED, this);
    lv_obj_t *zoom_in_btn =
        lv_win_add_btn(window, LV_SYMBOL_PLUS, HEADER_HEIGHT);
    lv_obj_add_event_cb(zoom_in_btn, graphs_zoom_in_btn_handler,
                        LV_EVENT_CLICKED, this);

    lv_obj_t *content = lv_win_get_content(window);
    // Borrowing big_number_content_style for the lack of borders 'n margins.
    lv_obj_add_style(content, &big_number_content_style, LV_PART_MAIN);

    chart_ = lv_chart_create(content);
    lv_obj_set_size(chart_, lv_pct(90), lv_pct(88));
    lv_obj_align(chart_, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_chart_set_type(chart_, LV_CHART_TYPE_LINE);
    lv_obj_add_style(chart_, &graph_point_style, LV_PART_INDICATOR);
    lv_chart_set_div_line_count(chart_, 0, 0);
    lv_chart_set_range(chart_, LV_CHART_AXIS_PRIMARY_Y, 30, 100);
    lv_chart_set_axis_tick(chart_, LV_CHART_AXIS_PRIMARY_X, 5, 5, 13, 2, true,
                           30);
    lv_chart_set_axis_tick(chart_, LV_CHART_AXIS_PRIMARY_Y, 5, 0, 8, 1, true,
                           30);
    lv_obj_add_event_cb(chart_, graphs_draw_part_cb, LV_EVENT_DRAW_PART_BEGIN,
                        NULL);

    temp_series_ = lv_chart_add_series(chart_, lv_palette_main(LV_PALETTE_RED),
                                       LV_CHART_AXIS_PRIMARY_Y);

    UpdateMeasurements();
  }

  void UpdateMeasurements() override {
    int x = 24 * 60 / 5;
    lv_chart_set_point_count(chart_, x);
    lv_chart_set_all_value(chart_, temp_series_, LV_CHART_POINT_NONE);

    --x;
    for (auto riter = temp_c_5m_avgs_->rbegin();
         riter != temp_c_5m_avgs_->rend(); ++riter) {
      temp_series_->y_points[x] = CToF(riter->value);
      --x;
    }
    lv_chart_refresh(chart_);
  }

  void ZoomOut() {
    zoom_ = std::max((uint32_t)256, zoom_ * 2 / 3);
    lv_chart_set_zoom_x(chart_, zoom_);
  }
  void ZoomIn() {
    zoom_ = zoom_ * 3 / 2;
    lv_chart_set_zoom_x(chart_, zoom_);
  }

  std::function<void()> switch_to_big_numbers;

private:
  RingBuffer<float> *const temp_c_5m_avgs_;

  lv_obj_t *chart_;
  lv_chart_series_t *temp_series_;

  uint32_t zoom_ = 256;
};

static void graphs_back_btn_handler(lv_event_t *e) {
  ((Graphs *)lv_event_get_user_data(e))->switch_to_big_numbers();
}

static void graphs_zoom_out_btn_handler(lv_event_t *e) {
  ((Graphs *)lv_event_get_user_data(e))->ZoomOut();
}
static void graphs_zoom_in_btn_handler(lv_event_t *e) {
  ((Graphs *)lv_event_get_user_data(e))->ZoomIn();
}

#endif // ENVIRO_PE_SRC_UI_SCREENS_GRAPHS_H_
