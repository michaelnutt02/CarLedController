/*
  CarLedController.h - Library for controlling rgbw leds mounted under a car.
  Created by Michael Nutt, March 20 2022.
  Private domain.
*/

#ifndef CarLedController_h
#define CarLedController_h

    #include "Arduino.h"
    #include <Adafruit_NeoPixel.h>
    class CarLedController
    {
        public:
            CarLedController(Adafruit_NeoPixel strip, int leds[][], double brightness);
            void initLoop();
            void renderLoop();
            void initStrip(int ledCount, int ledPin);
            void setLed(int row, int col, uint32_t color);
            void showEdges(uint32_t colorL, uint32_t colorU, uint32_t colorR, uint32_t colorD, uint32_t colorCorner);
            void blinkerRight(uint32_t color, int interval);
        private:
            int _numRows;
            int _numCols;
            int _leds[][];
            int _ledCount;
            double _brightness;
            Adafruit_NeoPixel _strip;
    };

#endif