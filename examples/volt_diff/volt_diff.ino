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

  uint8_t first = rel_voltage * 10 / 1023,
          second = rel_voltage * 100 / 1023 % 10,
          third = rel_voltage * 1000 / 1023 % 10;

  frame[0] = 0, frame[1] = 0, frame[2] = 0;

  if (rel_voltage == 1023) {

    dig35_to_mat(frame, 3, 9, 1);
    matrix_set_bit(frame, 3, 7, 1);
    dig35_to_mat(frame, 3, 4, 0);
    dig35_to_mat(frame, 3, 0, 0);

  } else {

    matrix_set_bit(frame, 3, 11, 1);
    dig35_to_mat(frame, 3, 8, first);
    dig35_to_mat(frame, 3, 4, second);
    dig35_to_mat(frame, 3, 0, third);
  }

  if (negative) {
    mat_fill_rect(frame, 1, 8, 1, 10, 1); 
  }

  matrix.loadFrame(frame);
  delay(200);
}
