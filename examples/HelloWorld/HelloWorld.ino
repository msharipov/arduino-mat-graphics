/*!
 *  Copyright 2025 Maxim Sharipov (msharipovr@gmail.com).
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
const uint8_t HELLO[5] = {7, 4, 11, 11, 14},
              WORLD[5] = {22, 14, 17, 11, 3};
uint8_t step = 0;

void setup() {
  matrix.begin();
}

void loop() {
  
  frame.draw_text_3_4(LMG::DEFAULT_FONT_3_4, 11, 0, 4, HELLO, 5, 1, step - 6);
  frame.draw_text_3_4(LMG::DEFAULT_FONT_3_4, 11, 0, 0, WORLD, 5, 1, step - 6);
  matrix.loadFrame(frame.getData());
  step = (step + 1) % 25;
  delay(300);
}
