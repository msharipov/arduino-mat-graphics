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

/// Represents a Tetris piece on the screen
class Piece {
  friend class GameState;

public:
  enum class PieceType {
    Block,
    HorizontalBar,
  };

  /// Total number of different piece types
  static constexpr size_t NUMBER_OF_TYPES{2};

  static constexpr bool SPRITES[][6] = {
      {true, true, true, true}, // Block
      {true, true, true, true}, // HorizontalBar
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
    case PieceType::HorizontalBar:
      area = LMG::Rect(2, 5, 11, 11);
      break;
    }
  }

  static Piece randomPiece() {
    return Piece(Piece::PieceType(rand() % Piece::NUMBER_OF_TYPES));
  }
};

class GameState {
  /// Holds the state of the pieces that have already been placed.
  bool placed_pieces[LMG::LED_MATRIX_HEIGHT][LMG::LED_MATRIX_WIDTH] = {0};

  /// Holds the state of the active piece
  Piece current_piece{Piece::PieceType::Block};

public:
  GameState() { current_piece = Piece::randomPiece(); }

  /// Control how fast the game runs in milliseconds per game tick.
  static constexpr uint32_t MS_PER_TICK{100};

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
    case Piece::PieceType::HorizontalBar: {
      const uint8_t col = current_piece.area.getLowCol() - 1;
      const uint8_t low_row = current_piece.area.getLowRow();
      return !(placed_pieces[low_row][col] || placed_pieces[low_row + 1][col] ||
               placed_pieces[low_row + 2][col] ||
               placed_pieces[low_row + 3][col]);
    }
    }
  }

  void descend() { current_piece.area.shiftColumns(-1); }

  bool placeCurrentPiece() {
    switch (current_piece.ptype) {
    case Piece::PieceType::Block: {
      const uint8_t low_col = current_piece.area.getLowCol();
      const uint8_t low_row = current_piece.area.getLowRow();
      placed_pieces[low_row][low_col] = true;
      placed_pieces[low_row][low_col + 1] = true;
      placed_pieces[low_row + 1][low_col] = true;
      placed_pieces[low_row + 1][low_col + 1] = true;
      break;
    }
    case Piece::PieceType::HorizontalBar: {
      const uint8_t col = current_piece.area.getLowCol();
      const uint8_t low_row = current_piece.area.getLowRow();
      placed_pieces[low_row][col] = true;
      placed_pieces[low_row + 1][col] = true;
      placed_pieces[low_row + 2][col] = true;
      placed_pieces[low_row + 3][col] = true;
      break;
    }
    }
    current_piece = Piece::randomPiece();
  }

  LMG::Frame drawPlaced() {
    LMG::Frame placed{};
    placed.drawSprite(
        &placed_pieces[0][0],
        LMG::Rect(0, LMG::LED_MATRIX_HEIGHT - 1, 0, LMG::LED_MATRIX_WIDTH - 1));
    return placed;
  }

  LMG::Frame drawActive() {
    LMG::Frame active{};
    active.drawSprite(Piece::SPRITES[static_cast<int32_t>(current_piece.ptype)],
                      current_piece.area);
    return active;
  }
};

ArduinoLEDMatrix matrix{};
LMG::Frame placed{};
GameState game{};

void setup() { matrix.begin(); }

void loop() {
  using LMG::Frame;
  matrix.loadFrame((placed + game.drawActive()).getData());
  delay(GameState::MS_PER_TICK);
  if (game.canDescend()) {
    game.descend();
  } else {
    game.placeCurrentPiece();
    placed = game.drawPlaced();
  }
}
