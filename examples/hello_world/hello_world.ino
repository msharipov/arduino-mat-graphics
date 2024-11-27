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
