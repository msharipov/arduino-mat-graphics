#include "Arduino_LED_Matrix.h"
#include <stdint.h>
#include <LED_Matrix_Graphics.h>

ArduinoLEDMatrix matrix;
uint32_t frame[3] = {0};

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

  frame[0] = 0, frame[1] = 0, frame[2] = 0;

  if (rel_voltage == 1023) {

    LMG_put_sym(frame, DIGITS_35[1], 3, 9, 3, 5);
    LMG_set_bit(frame, 3, 7, 1);
    LMG_put_sym(frame, DIGITS_35[0], 3, 4, 3, 5);
    LMG_put_sym(frame, DIGITS_35[0], 3, 0, 3, 5);

  } else {

    LMG_set_bit(frame, 3, 11, 1);
    LMG_put_sym(frame, DIGITS_35[X], 3, 8, 3, 5);
    LMG_put_sym(frame, DIGITS_35[Y], 3, 4, 3, 5);
    LMG_put_sym(frame, DIGITS_35[Z], 3, 0, 3, 5);
  }

  if (negative) {
    LMG_fill_rect(frame, 1, 8, 1, 10, 1); 
  }

  matrix.loadFrame(frame);
  delay(200);
}
