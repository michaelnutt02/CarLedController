/*
  CarLedController.cpp - Library for controlling rgbw leds mounted under a car.
  Created by Michael Nutt, March 20 2022.
  Private domain.
*/

#include "Arduino.h"
#include "CarLedController.h"
#include <Adafruit_NeoPixel.h>

CarLedController::CarLedController(int* leds, int numCols, int numRows, int brightness)
{
    _leds = leds;
    _brightness = brightness;
    _numRows = numRows;
    _numCols = numCols;
}

void CarLedController::initStrip(int ledCount, int ledPin) {
    _strip = Adafruit_NeoPixel(ledCount, ledPin, NEO_GBRW + NEO_KHZ800);
    _ledCount = ledCount;
    _strip.begin();
    for(int i = 0; i < ledCount; i++) {
        _strip.setPixelColor(i, _strip.Color(0, 0, 0, 0));
    }
    _strip.show();
    for(int i = 0; i < ledCount; i++) {
        _strip.setPixelColor(i, _strip.Color(255, 0, 0, 0));
        _strip.show();
        delay(25);
    }
    for(int i = 0; i < ledCount; i++) {
        _strip.setPixelColor(i, _strip.Color(0, 0, 0, 0));
    }
    _strip.show();
}

void CarLedController::blinkerRight(uint32_t color, int interval) {
    int loopIterations = 6;
    int i = (millis() / interval) % loopIterations;

    int startHorizontal = _numCols / 2;
    int endHorizontal = _numCols - 1;

    for(int col = startHorizontal; col < endHorizontal; col++) {
        setLed(0, col, _strip.Color(0,0,0));
        setLed(_numRows - 1, col, _strip.Color(0,0,0));
    }

    for(int row = 0; row < _numRows; row++) {
        setLed(row, _numCols - 1, _strip.Color(0,0,0));
    }

    for(int col = startHorizontal; col < endHorizontal; col++) {
        if(map(col, startHorizontal, endHorizontal, 0, 4) <= i) {
            setLed(0, col, color);
            setLed(_numRows - 1, col, color);
        }
    }

    for(int row = 0; row < _numRows; row++) {
        if(4 <= i) {
            setLed(row, _numCols - 1, color);
        }
    }
}

void CarLedController::blinkerRightBraking(uint32_t color, int interval) {
    int loopIterations = 6;
    int i = (millis() / interval) % loopIterations;

    int startHorizontal = _numCols / 2;
    int endHorizontal = _numCols - 1;

    for(int col = startHorizontal; col < endHorizontal; col++) {
        setLed(0, col, _strip.Color(0,0,0));
        setLed(_numRows - 1, col, _strip.Color(0,0,0));
    }

    for(int row = 0; row < _numRows; row++) {
        setLed(row, _numCols - 1, _strip.Color(0,0,0));
    }

    for(int col = startHorizontal; col < endHorizontal; col++) {
        if(map(col, startHorizontal, endHorizontal, 0, 4) <= i) {
            setLed(0, col, color);
            setLed(_numRows - 1, col, _strip.Color(255,0,0));
        }
    }

    for(int row = 0; row < _numRows; row++) {
        if(4 <= i) {
            setLed(row, _numCols - 1, color);
        }
    }
}

void CarLedController::showEdges(uint32_t colorL, uint32_t colorU, uint32_t colorR, uint32_t colorD, uint32_t colorCorner) {
    for(int row = 0; row < _numRows; row++) {
        setLed(row, 0, colorL);
        setLed(row, _numCols - 1, colorR);
    }
    for(int col = 0; col < _numCols; col++) {
        setLed(0, col, colorU);
        setLed(_numRows - 1, col, colorD);
    }
    setLed(1, 0, colorCorner);
    setLed(0, 1, colorCorner);
    setLed(_numRows - 2, 0, colorCorner);
    setLed(_numRows - 1, 1, colorCorner);
    setLed(0, _numCols - 2, colorCorner);
    setLed(1, _numCols - 1, colorCorner);
    setLed(_numRows - 2, _numCols - 1, colorCorner);
    setLed(_numRows - 1, _numCols - 2, colorCorner);
}

void CarLedController::initLoop() {
    for(int i = 0; i < _ledCount; i++) {
        _strip.setPixelColor(i, _strip.Color(0, 0, 0, 0));
    }
}

void CarLedController::renderLoop() {
    _strip.setBrightness(_brightness);
    _strip.show();
}

void CarLedController::setLed(int row, int col, uint32_t color) {
    if(_leds[row*_numRows+col] >= 0) _strip.setPixelColor(_leds[row*_numRows+col], color);
}

long HSBtoRGB(float _hue, float _sat, float _brightness) {
    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;
    
    if (_sat == 0.0) {
        red = _brightness;
        green = _brightness;
        blue = _brightness;
    } else {
        if (_hue == 360.0) {
            _hue = 0;
        }

        int slice = _hue / 60.0;
        float hue_frac = (_hue / 60.0) - slice;

        float aa = _brightness * (1.0 - _sat);
        float bb = _brightness * (1.0 - _sat * hue_frac);
        float cc = _brightness * (1.0 - _sat * (1.0 - hue_frac));
        
        switch(slice) {
            case 0:
                red = _brightness;
                green = cc;
                blue = aa;
                break;
            case 1:
                red = bb;
                green = _brightness;
                blue = aa;
                break;
            case 2:
                red = aa;
                green = _brightness;
                blue = cc;
                break;
            case 3:
                red = aa;
                green = bb;
                blue = _brightness;
                break;
            case 4:
                red = cc;
                green = aa;
                blue = _brightness;
                break;
            case 5:
                red = _brightness;
                green = aa;
                blue = bb;
                break;
            default:
                red = 0.0;
                green = 0.0;
                blue = 0.0;
                break;
        }
    }

    long ired = red * 255.0;
    long igreen = green * 255.0;
    long iblue = blue * 255.0;
    
    return long((ired << 16) | (igreen << 8) | (iblue));
}

void CarLedController::GAMER(int interval, float density) {
    int loopIterations = 361;
    int i = (millis() / interval) % loopIterations;

    float saturation = 1; // Between 0 and 1 (0 = gray, 1 = full color)
    float brightness = 1; // Between 0 and 1 (0 = dark, 1 is full brightness)

    float ledScale = (float)361/_ledCount;

    for(int j = 0; j < _ledCount; j++) {
        float hue = (int)(i+j*ledScale*density) % 361;
        long color = HSBtoRGB(hue, saturation, brightness); 
        int red = color >> 16 & 255;
        int green = color >> 8 & 255;
        int blue = color & 255;
        _strip.setPixelColor(j, _strip.Color(red, green, blue));
    }
}