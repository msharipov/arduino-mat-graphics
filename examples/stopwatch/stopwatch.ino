#include "Arduino_LED_Matrix.h"
#include <stdint.h>
#include <LED_Matrix_Graphics.h>

ArduinoLEDMatrix matrix;
uint32_t frame[3] = {0};
uint32_t start;

void setup() {
  matrix.begin();
  start = millis();
  matrix_set_bit(frame, 3, 4, 1);
}

void loop() {
  uint32_t diff = millis() - start;
  uint8_t first = (diff / 10000) % 10,
          second = (diff / 1000) % 10,
          third = (diff / 100) % 10;
  dig35_to_mat(frame, 3, 9, first);
  dig35_to_mat(frame, 3, 5, second);
  dig35_to_mat(frame, 3, 0, third);
  matrix.loadFrame(frame);
}
