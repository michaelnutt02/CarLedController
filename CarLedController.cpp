/*
  CarLedController.cpp - Library for controlling rgbw leds mounted under a car.
  Created by Michael Nutt, March 20 2022.
  Private domain.
*/

#include "Arduino.h"
#include "CarLedController.h"
#include <Adafruit_NeoPixel.h>

CarLedController::CarLedController(int leds[][], double brightness)
{
    _leds = leds;
    _brightness = brightness;
    _numRows = sizeof(leds) / sizeof(leds[0]);
    _numCols = sizeof(leds[0]) / sizeof(leds[0][0]);
}

void CarLedController::initStrip(int ledCount, int ledPin) {
    _strip = Adafruit_NeoPixel strip(ledCount, ledPin, NEO_RGBW + NEO_KHZ800);
    _ledCount = ledCount;
    _strip.begin();
    _strip.show();
}

void CarLedController::blinkerRight(uint32_t color, int interval) {
    const loopIterations = 6;
    const i = (millis() / interval) % loopIterations;

    const startHorizontal = _numCols / 2;
    const endHorizontal = _numCols - 1;

    for(int col = startHorizontal; col < endHorizontal; col++) {
        if(map(col, startHorizontal, endHorizontal, 0, 3) <= i) {
            setLed(0, col, color);
            setLed(_numRows - 1, col, color);
        }
    }

    for(int row = 0; row < _numRows/2; row++) {
        if(map(row, 0, _numRows/2, 4, 5) <= i) {
            setLed(row, 0, color);
            setLed(_numRows - 1 - row, _numCols - 1, color);
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
    setLed(0, 0], colorCorner);
    setLed(_numRows - 1, 0, colorCorner);
    setLed(0, _numCols - 1, colorCorner);
    setLed(_numRows - 1, _numCols - 1, colorCorner);
}

void CarLedController::initLoop() {
    for(int i = 0; i < _ledCount) _strip.setPixelColor(i, _strip.Color(0, 0, 0, 0));
}

void CarLedController::renderLoop() {
    _strip.setBrightness(_brightness);
    _strip.show();
}

void CarLedController::setLed(int row, int col, uint32_t color) {
    if(leds[row][col] != -1) _strip.setPixelColor(_leds[row][col], color);
}