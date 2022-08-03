#include "Adafruit_ILI9341.h"
#include "Arduino.h"
#include "DHTesp.h"
#include "DS3231.h"
#include "PMserial.h"

#include "elements/root.h"
#include "ring_buffer.h"
#include "touch_dispatcher.h"
#include "util.h"

#define READ_SENSORS_INTERVAL_S 5

// DHT22 Temp/Humidity sensor
#define DHT_DATA_PIN 13
DHTesp dht;
RingBuffer<float> temp_c_values(10 * 60 / READ_SENSORS_INTERVAL_S);
RingBuffer<float> humidity_values(10 * 60 / READ_SENSORS_INTERVAL_S);

// PMS5003 AQI sensor
#define PMS_RX 32
#define PMS_TX 33
SerialPM pms(PMSx003, PMS_RX, PMS_TX);
RingBuffer<uint16_t> pm25_values(10 * 60 / READ_SENSORS_INTERVAL_S);

// DS3231 RTC
RTClib rtc;

// Timers
Timer timer_read_sensors;

// ILI9341 Display
#define TFT_CS 5
#define TFT_DC 2
#define TOUCH_CS 16
#define TOUCH_IRQ 17
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
#define DISPLAY_ROTATION 3
const TS_Calibration
    TOUCH_SCREEN_CALIBRATION(TS_Point(13, 11), TS_Point(3795, 3704),
                             TS_Point(312, 113), TS_Point(482, 2175),
                             TS_Point(167, 214), TS_Point(2084, 640),
                             DISPLAY_WIDTH, DISPLAY_HEIGHT);
XPT2046_Calibrated touch_screen(TOUCH_CS, TOUCH_IRQ);
TouchDispatcher touch_dispatcher(&touch_screen);
Adafruit_ILI9341 display(TFT_CS, TFT_DC);

// Cater UI
Root root_element(&display, &temp_c_values, &humidity_values, &pm25_values);

void setup() {
  // TODO: Figure out why I can't initialize the struct above anymore.
  timer_read_sensors.total_cycle_time = seconds(READ_SENSORS_INTERVAL_S);

  Serial.begin(115200);
  Serial.println("Heyo!");

  Wire.begin(); // Required for RTC.
  dht.setup(DHT_DATA_PIN, DHTesp::DHT22);
  pms.init();

  touch_screen.begin();
  touch_screen.setRotation(DISPLAY_ROTATION);
  touch_screen.calibrate(TOUCH_SCREEN_CALIBRATION);
  display.begin();
  display.setRotation(DISPLAY_ROTATION);

  touch_dispatcher.registerHandler(
      TS_Point(0, 0), TS_Point(DISPLAY_WIDTH, DISPLAY_HEIGHT),
      [&display](const int16_t x, const int16_t y, const int16_t z) {
        Serial.printf("Touched; x: %d, y: %d, z: %d\r\n", x, y, z);
        display.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.println("Touched; x: " + String(x) + ", y: " + String(y) +
                        ", z: " + String(z));
      });

  root_element.Refresh();
}

void ReadAllSensors() {
  Serial.println("Reading sensors...");

  const DateTime now = rtc.now();
  Serial.println("Time: " + String(now.year()) + "-" + String(now.month()) +
                 "-" + String(now.day()) + "T" + String(now.hour()) + ":" +
                 String(now.minute()) + ":" + String(now.second()));

  float temp_c = dht.getTemperature();
  if (!isnan(temp_c)) {
    temp_c_values.Insert(temp_c, millis());
    Serial.println("Temp: " + String(temp_c_values.Latest().value) + "°C, " +
                   String(CToF(temp_c_values.Latest().value)) + "°F");
  }

  float humidity = dht.getHumidity();
  if (!isnan(humidity)) {
    humidity_values.Insert(humidity, millis());
    Serial.println("Humidity: " + String(humidity_values.Latest().value) + "%");
  }

  if (pms.read() == SerialPM::OK) {
    pm25_values.Insert(pms.pm25, millis());
    Serial.println("PM2.5: " + String(pm25_values.Latest().value));
  }

  root_element.Update();
}

void loop() {
  touch_dispatcher.update();

  if (timer_read_sensors.Complete()) {
    timer_read_sensors.Reset();
    ReadAllSensors();
  }
}
