#ifndef ENVIRO_PE_SRC_CATER_CATER_ELEMENT_
#define ENVIRO_PE_SRC_CATER_CATER_ELEMENT_

#include "touch_dispatcher.h"

class CaterElement {
public:
  CaterElement(Adafruit_ILI9341 *const display) : display_(display) {}
  CaterElement(Adafruit_ILI9341 *const display,
               TouchDispatcher *const touch_dispatcher)
      : display_(display), touch_dispatcher_(touch_dispatcher) {}

  virtual void Refresh() = 0;

  virtual void Update() = 0;

protected:
  Adafruit_ILI9341 *const display_;
  TouchDispatcher *const touch_dispatcher_ = nullptr;
};

#endif // ENVIRO_PE_SRC_CATER_CATER_ELEMENT_
