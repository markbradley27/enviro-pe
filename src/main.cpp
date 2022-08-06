#include "DHTesp.h"
#include "DS3231.h"
#include "PMserial.h"
#include "TFT_eSPI.h"
#include "lvgl.h"

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

// Display
#define DISP_HOR_RES 320
#define DISP_VER_RES 240
#define DISP_BUF_SIZE (DISP_HOR_RES * DISP_VER_RES / 10)
TFT_eSPI tft = TFT_eSPI();
static lv_disp_draw_buf_t lv_draw_buf;
static lv_color_t lv_buf[DISP_BUF_SIZE];
static lv_disp_drv_t lv_disp_drv;

lv_obj_t *screen_main;
lv_obj_t *label;

void DisplayFlush(lv_disp_drv_t *disp, const lv_area_t *area,
                  lv_color_t *color_p) {
  const uint32_t w = area->x2 - area->x1 + 1;
  const uint32_t h = area->y2 - area->y1 + 1;
  uint32_t wh = w * h;
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void setup() {
  // TODO: Figure out why I can't initialize the struct above anymore.
  timer_read_sensors.total_cycle_time = seconds(READ_SENSORS_INTERVAL_S);

  Serial.begin(115200);
  Serial.println("Heyo!");

  Wire.begin(); // Required for RTC.
  dht.setup(DHT_DATA_PIN, DHTesp::DHT22);
  pms.init();

  tft.init();
  tft.setRotation(3);

  // Sanity check display.
  tft.fillScreen(TFT_CYAN);
  tft.fillTriangle(10, 10, 20, 10, 10, 20, TFT_DARKGREEN);

  lv_init();
  lv_disp_draw_buf_init(&lv_draw_buf, lv_buf, NULL, DISP_BUF_SIZE);
  lv_disp_drv_init(&lv_disp_drv);
  lv_disp_drv.flush_cb = DisplayFlush;
  lv_disp_drv.draw_buf = &lv_draw_buf;
  lv_disp_drv.hor_res = DISP_HOR_RES;
  lv_disp_drv.ver_res = DISP_VER_RES;
  lv_disp_drv_register(&lv_disp_drv);

  screen_main = lv_obj_create(NULL);
  label = lv_label_create(screen_main);
  lv_label_set_text(label, "Herp derp.");
  lv_obj_set_size(label, 240, 40);
  lv_obj_set_pos(label, 30, 15);

  lv_scr_load(screen_main);
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
}

void loop() {
  lv_task_handler();

  if (timer_read_sensors.Complete()) {
    timer_read_sensors.Reset();
    ReadAllSensors();
  }
}
