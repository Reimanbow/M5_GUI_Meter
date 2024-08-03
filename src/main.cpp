#include <Arduino.h>
#include <M5Unified.h>
#include <meter.h>

// 目盛りの描画
M5Canvas canvas_scale(&M5.Display);
// 動的な部分の描画
M5Canvas canvas_dynamic(&M5.Display);

// メーターを描画するクラス
Meter barometer = Meter("Baro", "hPa", 1050, 970);

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  // 目盛り部分の初期化と描画
  barometer.initScaleCanvas(&canvas_scale, M5.Display.width(), M5.Display.height());
  barometer.drawScaleCanvas();
  
  // 針, 値部分の初期化
  barometer.initDynamicCanvas(&canvas_dynamic);
}

float count = 970;
float coefficient = 0.5;

void loop() {
  unsigned long start_ms = millis();

  float pressure = count;
  if(pressure >= 1050){
    pressure = 1050;
    coefficient = -0.5;
  }else if(pressure <= 970){
    pressure = 970;
    coefficient = 0.5;
  }
  count += coefficient;
  barometer.drawDynamicCanvas(pressure);

  while(millis() - start_ms < 100);
}
