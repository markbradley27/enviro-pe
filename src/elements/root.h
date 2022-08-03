#ifndef ENVIRO_PE_SRC_ELEMENTS_ROOT_
#define ENVIRO_PE_SRC_ELEMENTS_ROOT_

#include "../cater/cater_element.h"
#include "../ring_buffer.h"

class Root : public CaterElement {
public:
  Root(Adafruit_ILI9341 *const display, RingBuffer<float> *const temp_c_values,
       RingBuffer<float> *const humidity_values,
       RingBuffer<uint16_t> *const pm25_values)
      : CaterElement(display), temp_c_values_(temp_c_values),
        humidity_values_(humidity_values), pm25_values_(pm25_values) {}

  void Refresh() override { Update(); }

  void Update() override {
    display_->setCursor(0, 8);
    display_->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    display_->setTextSize(1);
    display_->println(
        "Temp: " + String(temp_c_values_->Latest().value) +
        ", Humidity: " + String(humidity_values_->Latest().value) +
        ", PM25: " + String(pm25_values_->Latest().value));
  }

private:
  RingBuffer<float> *const temp_c_values_;
  RingBuffer<float> *const humidity_values_;
  RingBuffer<uint16_t> *const pm25_values_;
};

#endif // ENVIRO_PE_SRC_ELEMENTS_ROOT_
