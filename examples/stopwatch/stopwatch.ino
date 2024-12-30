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
#include "Arduino_LED_Matrix.h"
#include <stdint.h>
#include <LED_Matrix_Graphics.h>

ArduinoLEDMatrix matrix;
LMG::Frame frame;
uint32_t start;

void setup() {
  matrix.begin();
  start = millis();
  frame.set_bit(3, 4, 1);
}

void loop() {
  uint32_t diff = millis() - start;
  uint8_t X = (diff / 10000) % 10,
          Y = (diff / 1000) % 10,
          Z = (diff / 100) % 10;
  
  if (X == 0) {
    frame.fill_rect(3, 9, 7, 11, 0);
  } else {
    frame.put_sym(LMG::DIGITS_35[X], 3, 9, 3, 5);
  }
  frame.put_sym(LMG::DIGITS_35[Y], 3, 5, 3, 5);
  frame.put_sym(LMG::DIGITS_35[Z], 3, 0, 3, 5);
  matrix.loadFrame(frame.getData());
}
