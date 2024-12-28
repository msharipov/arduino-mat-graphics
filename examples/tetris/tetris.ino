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
#include "Arduino_LED_Matrix.h"
#include "Tetris.h" // Contains the game code
#include <LED_Matrix_Graphics.h>

ArduinoLEDMatrix matrix{};
LMG::Frame placed{};
GameState game{};
bool rotate_button_pushed{false};
bool shift_left_button_pushed{false};
bool shift_right_button_pushed{false};

/// Wire up buttons to these pins such that pushing the button sets the pin to
/// high.
constexpr pin_size_t DROP_PIECE_BUTTON{2};
constexpr pin_size_t ROTATE_PIECE_BUTTON{3};
constexpr pin_size_t SHIFT_LEFT_BUTTON{4};
constexpr pin_size_t SHIFT_RIGHT_BUTTON{5};

void setup() {
  matrix.begin();
  pinMode(DROP_PIECE_BUTTON, INPUT);
  pinMode(ROTATE_PIECE_BUTTON, INPUT);
  pinMode(SHIFT_LEFT_BUTTON, INPUT);
  pinMode(SHIFT_RIGHT_BUTTON, INPUT);
}

/// Draws the current game score to the screen
void drawScore() {
  using LMG::Frame, LMG::Rect, LMG::DEFAULT_FONT_3x5;
  Frame score_screen{};
  const uint32_t score = game.getScore();
  constexpr size_t DIGITS_OFFSET{29};
  const size_t ones = score % 10;
  score_screen.drawSprite(DEFAULT_FONT_3x5[DIGITS_OFFSET + ones],
                          Rect(1, 5, 9, 11));
  const size_t tens = (score / 10) % 10;
  score_screen.drawSprite(DEFAULT_FONT_3x5[DIGITS_OFFSET + tens],
                          Rect(1, 5, 5, 7));
  const size_t hundreds = (score / 100) % 10;
  score_screen.drawSprite(DEFAULT_FONT_3x5[DIGITS_OFFSET + hundreds],
                          Rect(1, 5, 1, 3));
  matrix.loadFrame(score_screen.getData());
}

void loop() {
  using LMG::Frame;

  if (game.isGameOver()) {
    drawScore();
    return;
  }

  // Controls for piece rotation
  if (digitalRead(ROTATE_PIECE_BUTTON) == HIGH) {
    if (!rotate_button_pushed) {
      game.rotatePiece();
      rotate_button_pushed = true;
    }
  } else {
    rotate_button_pushed = false;
  }

  // Controls for shifting pieces
  if (digitalRead(SHIFT_LEFT_BUTTON) == HIGH) {
    if (!shift_left_button_pushed) {
      game.shiftPieceLeft();
      shift_left_button_pushed = true;
    }
  } else {
    shift_left_button_pushed = false;
  }

  if (digitalRead(SHIFT_RIGHT_BUTTON) == HIGH) {
    if (!shift_right_button_pushed) {
      game.shiftPieceRight();
      shift_right_button_pushed = true;
    }
  } else {
    shift_right_button_pushed = false;
  }

  // Controls for dropping pieces
  if (digitalRead(DROP_PIECE_BUTTON) == HIGH) {
    game.tryDescend();
  }

  // Update the game state
  game.nextTick();

  // Create a frame that shows the placed pieces
  placed = game.drawPlaced();

  // Combine the placed pieces with the active piece and update the screen
  matrix.loadFrame((placed + game.drawActive()).getData());
}
