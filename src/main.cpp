#include "Arduino.h"
#include "DHTesp.h"
#include "DS3231.h"
#include "LayoutManager.h"
#include "PMserial.h"

#include "ring_buffer.h"
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
#define TFT_DS 2
#define TOUCH_CS 16
#define TOUCH_IRQ 17
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240
LayoutManager lm(TFT_CS, TFT_DS, TOUCH_CS, TOUCH_IRQ, DISPLAY_WIDTH,
                 DISPLAY_HEIGHT, Orientation::LandscapeFlip, COLOR_BLACK);

void logTouch(Screen const &screen, Touch const &touch) {
  Serial.printf("Touch: x=%u, y=%u, pressure=%u\r\n", touch.x(), touch.y(),
                touch.pressure());
}

void setup() {
  // TODO: Figure out why I can't initialize the struct above anymore.
  timer_read_sensors.total_cycle_time = seconds(READ_SENSORS_INTERVAL_S);

  Serial.begin(115200);
  Serial.println("Heyo!");

  Wire.begin(); // Required for RTC.
  dht.setup(DHT_DATA_PIN, DHTesp::DHT22);
  pms.init();
  lm.setTouchEnd(logTouch);
  lm.begin();
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

  if (pms.read() == OK) {
    pm25_values.Insert(pms.pm25, millis());
    Serial.println("PM2.5: " + String(pm25_values.Latest().value));
  }
}

void loop() {
  lm.draw();

  if (timer_read_sensors.Complete()) {
    timer_read_sensors.Reset();
    ReadAllSensors();
  }
}
