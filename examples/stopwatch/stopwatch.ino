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

/*
 *  Displays a stopwatch on the LED matrix that counts up to 100 seconds before
 *  looping back to zero.
 */
#include "Arduino_LED_Matrix.h"
#include <LED_Matrix_Graphics.h>
#include <stdint.h>

ArduinoLEDMatrix matrix{};
LMG::Frame frame{};
uint32_t start{0};

void setup() {
  matrix.begin();
  start = millis();

  // Draw the decimal point
  frame.setLED(5, 7, HIGH);
}

void loop() {
  using LMG::Rect;

  // Offset at which the digit sprites are stored
  constexpr size_t DIGITS_OFFSET = 29;
  const uint32_t diff = millis() - start;
  const size_t tens_of_seconds = (diff / 10000) % 10;
  const size_t seconds = (diff / 1000) % 10;
  const size_t hundreds_of_ms = (diff / 100) % 10;

  if (tens_of_seconds == 0) {
    // Don't display the first digit if the time is less than 10 seconds.
    frame.fillRect(Rect(1, 5, 0, 2), LOW);
  } else {
    frame.drawSprite(LMG::DEFAULT_FONT_3x5[tens_of_seconds + DIGITS_OFFSET],
                     Rect(1, 5, 0, 2));
  }
  frame.drawSprite(LMG::DEFAULT_FONT_3x5[seconds + DIGITS_OFFSET],
                   Rect(1, 5, 4, 6));
  frame.drawSprite(LMG::DEFAULT_FONT_3x5[hundreds_of_ms + DIGITS_OFFSET],
                   Rect(1, 5, 9, 11));
  matrix.loadFrame(frame.getData());
}
