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
#include <LED_Matrix_Graphics.h>
#include <array>

ArduinoLEDMatrix matrix{};
LMG::Frame frame{};

/// Represents a Tetris piece on the screen
class Piece {
  friend class GameState;

public:
  enum class PieceType {
    Bar,
    Block,
  };

private:
  PieceType ptype{};
  LMG::Rect area{0, 0, 0, 0};

public:
  /// Constructs a new piece at the top of the screen
  explicit Piece(const PieceType _ptype) {
    ptype = _ptype;
    switch (ptype) {
    case PieceType::Block:
      area = LMG::Rect(3, 4, 10, 11);
      break;
    case PieceType::Bar:
      area = LMG::Rect(2, 5, 11, 11);
      break;
    }
  }
};

class GameState {
  /// Holds the state of the pieces that have already been placed.
  std::array<std::array<bool, LMG::LED_MATRIX_WIDTH>, LMG::LED_MATRIX_HEIGHT>
      placed_pieces{};

  /// Holds the state of the active piece
  Piece current_piece{Piece::PieceType::Block};

  /// Control how fast the game runs in milliseconds per game tick.
  static constexpr uint32_t MS_PER_TICK{100};

public:
  GameState() {}

  /// Checks if the currently active piece can descend.
  /**
   * @returns True, if the piece can descend. False, if it reached the bottom
   *          or there is an obstacle in the way.
   */
  bool canDescend() {
    if (current_piece.area.getLowCol() == 0) {
      return false;
    }
    switch (current_piece.ptype) {
    case Piece::PieceType::Block: {
      const uint8_t col = current_piece.area.getLowCol() - 1;
      const uint8_t low_row = current_piece.area.getLowRow();
      return !(placed_pieces[low_row][col] || placed_pieces[low_row + 1][col]);
    }
    case Piece::PieceType::Bar: {
      const uint8_t col = current_piece.area.getLowCol() - 1;
      const uint8_t low_row = current_piece.area.getLowRow();
      return !(placed_pieces[low_row][col] || placed_pieces[low_row + 1][col] ||
               placed_pieces[low_row + 2][col] ||
               placed_pieces[low_row + 3][col]);
    }
    }
  }
};

void setup() { matrix.begin(); }

void loop() {}
