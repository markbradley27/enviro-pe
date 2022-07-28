#include "Arduino.h"
#include "DHTesp.h"

#include "ring_buffer.h"
#include "util.h"

#define READ_SENSORS_INTERVAL_S 5

// DHT22 Temp/Humidity sensor
DHTesp dht;
RingBuffer<float> temp_c_values(10 * 60 / READ_SENSORS_INTERVAL_S);
RingBuffer<float> humidity_values(10 * 60 / READ_SENSORS_INTERVAL_S);

Timer timer_read_sensors = {seconds(READ_SENSORS_INTERVAL_S)};

void setup() {
  Serial.begin(115200);
  Serial.println("Heyo!");

  dht.setup(D1, DHTesp::DHT22);
}

void ReadAllSensors() {
  Serial.println("Reading sensors...");
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
