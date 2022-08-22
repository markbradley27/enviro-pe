#ifndef ENVIRO_PE_SRC_UI_COMPONENTS_DIGITAL_CLOCK_H_
#define ENVIRO_PE_SRC_UI_COMPONENTS_DIGITAL_CLOCK_H_

class DigitalClock {
public:
  DigitalClock(lv_obj_t *const parent, DS3231 *const rtc)
      : parent_(parent), rtc_(rtc) {
    UpdateTime();
  }

  void UpdateTime() {
    bool unused;
    lv_label_set_text_fmt(parent_, "%d:%02d", rtc_->getHour(unused, unused),
                          rtc_->getMinute());
  }

private:
  DS3231 *const rtc_;
  lv_obj_t *const parent_;
};

#endif // ENVIRO_PE_SRC_UI_COMPONENTS_DIGITAL_CLOCK_H_
