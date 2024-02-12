#include "Arduino_LED_Matrix.h"
#include <stdint.h>
#include <LED_Matrix_Graphics.h>

ArduinoLEDMatrix matrix;
uint32_t frame[3] = {0};
const uint8_t HELLO[5] = {7, 4, 11, 11, 14},
              WORLD[5] = {22, 14, 17, 11, 3};
uint8_t step = 0;

void setup() {
  matrix.begin();
}

void loop() {
  
  LMG_put_text_34(frame, SYMBOLS_34, 11, 0, 4, HELLO, 5, 1, step - 6);
  LMG_put_text_34(frame, SYMBOLS_34, 11, 0, 0, WORLD, 5, 1, step - 6);
  matrix.loadFrame(frame);
  step = (step + 1) % 25;
  delay(300);
}
