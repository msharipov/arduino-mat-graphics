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
