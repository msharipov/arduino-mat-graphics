#include "Arduino_LED_Matrix.h"
#include <stdint.h>
#include <LED_Matrix_Graphics.h>

ArduinoLEDMatrix matrix;
uint32_t frame[3] = {0};
uint32_t start;

void setup() {
  matrix.begin();
  start = millis();
  LMG_set_bit(frame, 3, 4, 1);
}

void loop() {
  uint32_t diff = millis() - start;
  uint8_t X = (diff / 10000) % 10,
          Y = (diff / 1000) % 10,
          Z = (diff / 100) % 10;
  
  if (X == 0) {
    LMG_fill_rect(frame, 3, 9, 7, 11, 0);
  } else {
    LMG_put_sym(frame, DIGITS_35[X], 3, 9, 3, 5);
  }
  LMG_put_sym(frame, DIGITS_35[Y], 3, 5, 3, 5);
  LMG_put_sym(frame, DIGITS_35[Z], 3, 0, 3, 5);
  matrix.loadFrame(frame);
}
