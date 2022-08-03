#ifndef ENVIRO_PE_SRC_TOUCH_DISPATCHER_
#define ENVIRO_PE_SRC_TOUCH_DISPATCHER_

#include <vector>

#include "Arduino.h"
#include "XPT2046_Calibrated.h"

typedef std::function<void(const int16_t x, const int16_t y, const int16_t z)>
    TouchHandler;
class TouchDispatcher {
public:
  TouchDispatcher(XPT2046_Calibrated *const touch_screen)
      : touch_screen_(touch_screen) {}

  void update() {
    if (!touch_screen_->touched()) {
      return;
    }

    const TS_Point point = touch_screen_->getPoint();
    for (const auto &entry : handlers_) {
      if (entry.upper_left.x < point.x && point.x < entry.lower_right.x &&
          entry.upper_left.y < point.y && point.y < entry.lower_right.y) {
        entry.handler(point.x, point.y, point.z);
      }
    }
  }

  int registerHandler(const TS_Point &upper_left, const TS_Point &lower_right,
                      TouchHandler handler) {
    handlers_.push_back({next_handler_id_, handler, upper_left, lower_right});
    return next_handler_id_++;
  }

  void unregisterHandler(const int id) {
    for (auto itr = handlers_.begin(); itr != handlers_.end(); ++itr) {
      if (itr->id == id) {
        handlers_.erase(itr);
        return;
      }
    }
  }

private:
  struct HandlerEntry {
    int id;
    TouchHandler handler;
    TS_Point upper_left;
    TS_Point lower_right;
  };

  int next_handler_id_ = 0;
  std::vector<HandlerEntry> handlers_;

  XPT2046_Calibrated *const touch_screen_;
};

#endif // ENVIRO_PE_SRC_TOUCH_DISPATCHER_
