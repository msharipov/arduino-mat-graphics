/*!
 *  Copyright 2024 Maxim Sharipov (msharipovr@gmail.com).
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

/**
 * Cycles the entire LED matrix on and off, one LED at a time.
 */

#include "Arduino_LED_Matrix.h"
#include <LED_Matrix_Graphics.h>

ArduinoLEDMatrix matrix {};
LMG::Frame frame {};

void setup() {
  matrix.begin();
}

void loop() {
  for (size_t row = 0; row < LMG::LED_MATRIX_HEIGHT; row++) {
    for (size_t col = 0; col < LMG::LED_MATRIX_WIDTH; col++) {
      frame.setLED(row, col, true);
      matrix.loadFrame(frame.getData());
      delay(20);
    }
  }

  for (size_t row = 0; row < LMG::LED_MATRIX_HEIGHT; row++) {
    for (size_t col = 0; col < LMG::LED_MATRIX_WIDTH; col++) {
      frame.setLED(row, col, false);
      matrix.loadFrame(frame.getData());
      delay(20);
    }
  }
}
