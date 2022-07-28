#include "Arduino.h"
#include "DHTesp.h"
#include "DS3231.h"

#include "ring_buffer.h"
#include "util.h"

#define READ_SENSORS_INTERVAL_S 5

// DHT22 Temp/Humidity sensor
#define DHT_DATA_PIN D0
DHTesp dht;
RingBuffer<float> temp_c_values(10 * 60 / READ_SENSORS_INTERVAL_S);
RingBuffer<float> humidity_values(10 * 60 / READ_SENSORS_INTERVAL_S);

// DS3231 RTC
RTClib rtc;

Timer timer_read_sensors = {seconds(READ_SENSORS_INTERVAL_S)};

void setup() {
  Serial.begin(115200);
  Serial.println("Heyo!");

  Wire.begin();

  dht.setup(DHT_DATA_PIN, DHTesp::DHT22);
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
}

void loop() {
  if (timer_read_sensors.Complete()) {
    timer_read_sensors.Reset();
    ReadAllSensors();
  }
}
