#include <Arduino.h>
#include <M5Unified.h>
#include <meter.h>

Meter::Meter(String label, String unit, int maxValue, int minValue)
    : label(label), unit(unit), maxValue(maxValue), minValue(minValue){
}

void Meter::initScaleCanvas(M5Canvas *scaleCanvas, int display_width, int display_height){
    this->scaleCanvas = scaleCanvas;

    width = display_width;
    height = display_height;

    if(width >= height){
        radius = height;
    }else{
        radius = width;
    }
    distanceToNumber = radius / 2 - radius / 8;
    distanceToScaleInside = radius / 2 - radius / 20;
    distanceToScaleOutside = radius / 2;
    handLength = radius / 4;

    this->scaleCanvas->createSprite(width, height);
}

// 目盛り部分の描画
void Meter::drawScaleCanvas(){
    // 一目盛りずつの描画: ディスプレイが小さいモジュールでは省略する
    scaleCanvas->clearDisplay();
    scaleCanvas->fillScreen(colorTheme.backgroundColor);
    if(radius >= 240){
        for(int i = 0; i < 81; i++){
            float xOutside = distanceToScaleOutside * cos(radians(i * oneTick + minDegree)) + width / 2;
            float yOutside = distanceToScaleOutside * sin(radians(i * oneTick + minDegree)) + height / 2;
            float xInside = distanceToScaleInside * cos(radians(i * oneTick + minDegree)) + width / 2;
            float yInside = distanceToScaleInside * sin(radians(i * oneTick + minDegree)) + height / 2;
            this->scaleCanvas->drawLine(xOutside, yOutside, xInside, yInside, colorTheme.oneScaleColor);
        }
    }

    // 数値描画部分の目盛りの描画: (maxValue - minValue) / 8ずつ
    for(int i = 0; i < 9; i++){
        float xOutside = distanceToScaleOutside * cos(radians(i * markedTick + minDegree)) + width / 2;
        float yOutside = distanceToScaleOutside * sin(radians(i * markedTick + minDegree)) + height / 2;
        float xInside = distanceToScaleInside * cos(radians(i * markedTick + minDegree)) + width / 2;
        float yInside = distanceToScaleInside * sin(radians(i * markedTick + minDegree)) + height / 2;
        this->scaleCanvas->drawLine(xOutside, yOutside, xInside, yInside, colorTheme.markedScaleColor);
    }

    // 目盛りの円弧の描画
    this->scaleCanvas->fillArc(width / 2, height / 2, radius / 2 - 2, radius / 2, minDegree, maxDegree, colorTheme.arcColor);

    // 目盛りの数字のフォントはディスプレイのサイズによって適切なものにする
    if(radius < 240){
        this->scaleCanvas->setFont(&fonts::TomThumb);
    }else{
        this->scaleCanvas->setFont(&fonts::FreeSansOblique9pt7b);
    }
    // 目盛りの数字の描画: (maxValue - minValue) / 8ずつ
    // TODO: 気圧以外の場合のときに重要となるはず
    this->scaleCanvas->setColor(colorTheme.numberColor);
    for(int i = 0; i < 9; i++){
        float x = distanceToNumber * cos(radians(i * markedTick + minDegree)) + width / 2;
        float y = distanceToNumber * sin(radians(i * markedTick + minDegree)) + height / 2;
        this->scaleCanvas->drawCenterString(String(i * ((maxValue - minValue) / 8) + minValue), x, y - 4);
    }

    // 文字の描画
    this->scaleCanvas->setColor(colorTheme.labelColor);
    this->scaleCanvas->setFont(&fonts::FreeMonoOblique9pt7b);
    this->scaleCanvas->drawCenterString(label, width / 2, radius - radius / 10);

    this->scaleCanvas->pushSprite(0, 0);
}

void Meter::initDynamicCanvas(M5Canvas *dynamicCanvas){
    this->dynamicCanvas = dynamicCanvas;
    this->dynamicCanvas->createSprite(radius / 2, radius / 2 + radius / 10);
}

void Meter::drawDynamicCanvas(float value){
    dynamicCanvas->clearDisplay();
    dynamicCanvas->fillScreen(colorTheme.backgroundColor);
    // 最大値, 最小値でクリッピング
    if(value > maxValue){
        value = maxValue;
    }else if(value < minValue){
        value = minValue;
    }
    // 最小値との差を計算し目盛りの位置を決める
    float valueDiff = (value - minValue) * oneTick * (scaleNum / (maxValue - minValue));
    float x = handLength * cos(radians(valueDiff + minDegree)) + handLength;
    float y = handLength * sin(radians(valueDiff + minDegree)) + handLength;
    dynamicCanvas->drawWedgeLine(handLength, handLength, x, y, 5, 1, colorTheme.headColor);

    // 値の数字のフォントはディスプレイのサイズによって適切なものにする
    if(radius < 240){
        this->dynamicCanvas->setFont(&fonts::Font0);
    }else{
        this->dynamicCanvas->setFont(&fonts::FreeSerifBoldItalic9pt7b);
    }
    // 値の描画
    dynamicCanvas->setColor(colorTheme.valueColor);
    dynamicCanvas->drawCenterString(String(value) + unit, handLength, handLength + height / 5);
    if(width == height){
        dynamicCanvas->pushSprite(width / 4, height / 4);
    }else if(width == 240){
        dynamicCanvas->pushSprite(width / 4 + width / 9, height / 4);
    }else if(width == 320){
        dynamicCanvas->pushSprite(width / 4 + width / 16, height / 4);
    }
}
