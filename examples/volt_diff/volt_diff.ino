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

/* This program allows you to quickly measure voltage using the Arduino. The
 * program will display the potential difference between pins A0 and A1 as a
 * fraction of the difference between 5V and GND on the LED matrix. If the
 * difference happens to be negative, a minus sign will also be displayed.
 */

#include "Arduino_LED_Matrix.h"
#include <LED_Matrix_Graphics.h>
#include <cstdint>

ArduinoLEDMatrix matrix{};
LMG::Frame frame{};

void setup() {
  matrix.begin();
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

void loop() {
  using LMG::Rect;
  const size_t DIGITS_OFFSET{29};
  const Rect FIRST_DIGIT{0, 4, 1, 3};
  const Rect SECOND_DIGIT{0, 4, 5, 7};
  const Rect THIRD_DIGIT{0, 4, 9, 11};
  const Rect FIRST_DIGIT_ALTERNATE{0, 4, 0, 2};
  const Rect NEGATIVE_SIGN{6, 6, 8, 10};

  int16_t rel_voltage = analogRead(A0) - analogRead(A1);
  const bool negative = (rel_voltage < 0);

  if (negative) {
    rel_voltage = -rel_voltage;
  }

  const uint8_t first = rel_voltage * 10 / 1023;
  const uint8_t second = rel_voltage * 100 / 1023 % 10;
  const uint8_t third = rel_voltage * 1000 / 1023 % 10;

  frame.fillRect(
      Rect(0, LMG::LED_MATRIX_HEIGHT - 1, 0, LMG::LED_MATRIX_WIDTH - 1), LOW);

  if (rel_voltage == 1023) {

    frame.drawSprite(LMG::DEFAULT_FONT_3x5[DIGITS_OFFSET + 1],
                     FIRST_DIGIT_ALTERNATE);
    frame.setLED(4, 3, HIGH);
    frame.drawSprite(LMG::DEFAULT_FONT_3x5[DIGITS_OFFSET], SECOND_DIGIT);
    frame.drawSprite(LMG::DEFAULT_FONT_3x5[DIGITS_OFFSET], THIRD_DIGIT);

  } else {

    frame.setLED(4, 0, HIGH);
    frame.drawSprite(LMG::DEFAULT_FONT_3x5[DIGITS_OFFSET + first], FIRST_DIGIT);
    frame.drawSprite(LMG::DEFAULT_FONT_3x5[DIGITS_OFFSET + second],
                     SECOND_DIGIT);
    frame.drawSprite(LMG::DEFAULT_FONT_3x5[DIGITS_OFFSET + third], THIRD_DIGIT);
  }

  if (negative) {
    frame.fillRect(NEGATIVE_SIGN, HIGH);
  }

  matrix.loadFrame(frame.getData());
  delay(200);
}
