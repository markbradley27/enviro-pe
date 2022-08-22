#ifndef ENVIRO_PE_SRC_SETTINGS_BRIGHTNESS_MANAGER_H_
#define ENVIRO_PE_SRC_SETTINGS_BRIGHTNESS_MANAGER_H_

class BrightnessManager {
public:
  BrightnessManager(const uint8_t pin) : pin_(pin) {
    pinMode(pin_, OUTPUT);
    // TODO: Load brightness from NVRAM.
    SetBrightness_(5);
  }

  uint8_t GetBrightness() { return brightness_; }

  // Brightness must be 0 through 10;
  void SetBrightness(const uint8_t brightness) {
    if (brightness > 10) {
      return;
    }
    // TODO: Save brightness to NVRAM.
    SetBrightness_(brightness);
  }

private:
  void SetBrightness_(const uint8_t brightness) {
    if (brightness > 10) {
      return;
    }
    brightness_ = brightness;
    analogWrite(pin_, brightness * 255 / 10);
  }

  const uint8_t pin_;
  uint8_t brightness_;
};

#endif // ENVIRO_PE_SRC_SETTINGS_BRIGHTNESS_MANAGER_H_
