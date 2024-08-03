#include <Arduino.h>
#include <M5Unified.h>

// メーター
class Meter {
  private:
    String label; // 何のメーターか
    String unit; // 単位
    // 取り得る値の範囲
    int maxValue;
    int minValue;

    int32_t width;
    int32_t height;
    int32_t radius; // 幅と高さの内, 小さい方の値
    int distanceToNumber; // 中央から各目盛りの数字までの距離
    // 中央から各目盛りまでの距離
    int distanceToScaleInside;
    int distanceToScaleOutside;
    int handLength; // 針の長さ

    // メーターの色のテーマ
    struct ColorTheme{
      // 背景の色
      int backgroundColor = BLACK;
      // 1目盛り分の色
      int oneScaleColor = WHITE;
      // 数字付き目盛りの色
      int markedScaleColor = RED;
      // 円弧の色
      int arcColor = WHITE;
      // 目盛りの数字の色
      int numberColor = WHITE;
      // ラベルの色
      int labelColor = WHITE;
      // 針の色
      int headColor = BLUE;
      // 値の色
      int valueColor = WHITE;
      
      ColorTheme(){}

      ColorTheme(int backgroundColor, int oneScaleColor, int markedScaleColor, int arcColor,
       int numberColor, int labelColor, int headColor, int valueColor){
        this->backgroundColor = backgroundColor;
        this->oneScaleColor = oneScaleColor;
        this->markedScaleColor = markedScaleColor;
        this->arcColor = arcColor;
        this->numberColor = numberColor;
        this->labelColor = labelColor;
        this->headColor = headColor;
        this->valueColor = valueColor;
      }
    } colorTheme;

    static const int minDegree = 150; // 最小値がとる角度
    static const int maxDegree = 390; // 最大値がとる角度
    static const int oneTick = 3; // 一目盛り
    static const int markedTick = oneTick * 10; // 強調される目盛り
    static const int scaleNum = 80; // 目盛りの数

    M5Canvas *scaleCanvas;
    M5Canvas *dynamicCanvas;
  
  public:
    Meter(String label, String unit, int maxValue, int minValue);

    // 目盛り部分の初期化
    void initScaleCanvas(M5Canvas *scaleCanvas, int display_width, int display_height);

    // 目盛り部分の描画
    void drawScaleCanvas();

    // 針, 値部分の初期化
    void initDynamicCanvas(M5Canvas *dynamicCanvas);

    // 針, 値部分の描画
    void drawDynamicCanvas(float value);
};