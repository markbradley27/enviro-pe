#ifndef ENVIRO_PE_SRC_UI_SCREENS_TEMP_GRAPH_H_
#define ENVIRO_PE_SRC_UI_SCREENS_TEMP_GRAPH_H_

#include "ui/screens/graph.h"

class TempGraph : public Graph {
public:
  TempGraph(RingBuffer<float> *const temp_c_5m_avgs,
            std::function<void()> switch_to_big_numbers)
      : temp_c_5m_avgs_(temp_c_5m_avgs), Graph(switch_to_big_numbers) {
    lv_label_set_text(title_, "Temp (°F)");

    lv_chart_set_range(chart_, LV_CHART_AXIS_PRIMARY_Y, 30, 100);
    lv_chart_set_axis_tick(chart_, LV_CHART_AXIS_PRIMARY_Y, 5, 0, 8, 1, true,
                           30);

    series_ = lv_chart_add_series(chart_, lv_palette_main(LV_PALETTE_RED),
                                  LV_CHART_AXIS_PRIMARY_Y);

    UpdateMeasurements();
  }

  void UpdateMeasurements() override {
    int x = 24 * 60 / 5;
    lv_chart_set_point_count(chart_, x);
    lv_chart_set_all_value(chart_, series_, LV_CHART_POINT_NONE);

    --x;
    for (auto riter = temp_c_5m_avgs_->rbegin();
         riter != temp_c_5m_avgs_->rend(); ++riter) {
      series_->y_points[x] = CToF(riter->value);
      --x;
    }
    lv_chart_refresh(chart_);
  }

private:
  RingBuffer<float> *const temp_c_5m_avgs_;

  lv_chart_series_t *series_;
};

#endif // ENVIRO_PE_SRC_UI_SCREENS_TEMP_GRAPH_H_
