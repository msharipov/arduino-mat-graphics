/* This program allows you to quickly measure voltage using the Arduino. The
 * program will display the potential difference between pins A1 and A2 as a
 * fraction of the difference between 5V and GND on the LED matrix. If the
 * difference happens to be negative, a minus sign will also be displayed.
*/ 

#include "Arduino_LED_Matrix.h"
#include <cstdint>
#include <LED_Matrix_Graphics.h>

ArduinoLEDMatrix matrix;
LMG::Frame frame;

void setup() {
  matrix.begin();
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  Serial.begin(9600);
}

void loop() {

  int16_t rel_voltage = analogRead(A0) - analogRead(A1);
  bool negative = (rel_voltage < 0);

  if (negative) {
    rel_voltage = -rel_voltage;
  }

  uint8_t X = rel_voltage * 10 / 1023,
          Y = rel_voltage * 100 / 1023 % 10,
          Z = rel_voltage * 1000 / 1023 % 10;

  frame.fill_rect(0, 7, 0, 11, 0); 

  if (rel_voltage == 1023) {

    frame.put_sym(LMG::DIGITS_35[1], 3, 9, 3, 5);
    frame.set_bit(3, 7, 1);
    frame.put_sym(LMG::DIGITS_35[0], 3, 4, 3, 5);
    frame.put_sym(LMG::DIGITS_35[0], 3, 0, 3, 5);

  } else {

    frame.set_bit(3, 11, 1);
    frame.put_sym(LMG::DIGITS_35[X], 3, 8, 3, 5);
    frame.put_sym(LMG::DIGITS_35[Y], 3, 4, 3, 5);
    frame.put_sym(LMG::DIGITS_35[Z], 3, 0, 3, 5);
  }

  if (negative) {
    frame.fill_rect(1, 8, 1, 10, 1); 
  }

  matrix.loadFrame(frame.getData());
  delay(200);
}
