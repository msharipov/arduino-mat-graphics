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
#pragma once

#include <LED_Matrix_Graphics.h>

/// Represents a Tetris piece on the screen
class Piece {
  friend class GameState;

public:
  enum class PieceType {
    Block,
    HorizontalBar,
    VerticalBar,
  };

  /// These types can be spawned by randomPiece
  static constexpr PieceType SPAWNABLE[] = {
      PieceType::Block,
      PieceType::HorizontalBar,
  };

  static constexpr size_t SPAWNABLE_COUNT{sizeof(SPAWNABLE) /
                                          sizeof(PieceType)};

  static constexpr bool SPRITES[][6] = {
      {true, true, true, true}, // Block
      {true, true, true, true}, // HorizontalBar
      {true, true, true, true}, // VerticalBar
  };

private:
  PieceType ptype{};
  LMG::Rect area{0, 0, 0, 0};

public:
  /// Constructs a new piece at the top of the screen.
  /**
   * Unspawnable pieces are turned into the corresponding spawnable variant.
   */
  explicit Piece(const PieceType _ptype) {
    ptype = _ptype;
    switch (ptype) {
    case PieceType::Block:
      area = LMG::Rect(3, 4, 10, 11);
      break;
    case PieceType::VerticalBar:
      ptype = PieceType::HorizontalBar;
      [[fallthrough]];
    case PieceType::HorizontalBar:
      area = LMG::Rect(2, 5, 11, 11);
      break;
    }
  }

  static Piece randomPiece() {
    return Piece(Piece::SPAWNABLE[rand() % Piece::SPAWNABLE_COUNT]);
  }
};

class GameState {
  /// Holds the state of the pieces that have already been placed.
  bool placed_pieces[LMG::LED_MATRIX_HEIGHT][LMG::LED_MATRIX_WIDTH] = {0};

  /// Holds the state of the active piece
  Piece current_piece{Piece::PieceType::Block};

  /// Holds the current game time
  uint32_t current_tick{0};

  /// Determines whether the game is in line-clearing state.
  bool clearing_lines{false};

  /// When clearing lines, determines which line to check next.
  size_t line_to_check{0};

  /// Determines if the game is over.
  bool game_over{false};

  /// Tracks the score.
  uint16_t score{0};

public:
  GameState() { current_piece = Piece::randomPiece(); }

  /// Controls how fast the game runs in milliseconds per game tick.
  static constexpr uint32_t MS_PER_TICK{100};

  /// Controls how many ticks pass before the active piece descends
  /// automatically.
  static constexpr uint32_t TICKS_PER_DESCENT{10};

  /// Checks if the currently active piece can descend.
  /**
   * @returns True, if the piece can descend. False, if it reached the bottom
   *          or there is an obstacle in the way.
   */
  bool canDescend() {
    if (current_piece.area.getLowCol() == 0) {
      return false;
    }
    const int8_t high_row = current_piece.area.getHighRow();
    const int8_t low_row = current_piece.area.getLowRow();
    const int8_t high_col = current_piece.area.getHighCol();
    const int8_t low_col = current_piece.area.getLowCol();
    const int8_t width = high_col - low_col + 1;
    const int8_t height = high_row - low_row + 1;
    for (int8_t add_row = 0; add_row < height; add_row++) {
      for (int8_t add_col = 0; add_col < width; add_col++) {
        const bool part_of_active_piece =
            Piece::SPRITES[static_cast<size_t>(current_piece.ptype)]
                          [add_row * width + add_col];
        const bool space_below_full =
            placed_pieces[low_row + add_row][low_col + add_col - 1];
        if (part_of_active_piece && space_below_full) {
          return false;
        }
      }
    }
    return true;
  }

  /// Lowers the current active piece by one line.
  void descend() { current_piece.area.shiftColumns(-1); }

  bool placeCurrentPiece() {
    switch (current_piece.ptype) {
    case Piece::PieceType::Block: {
      const int8_t low_col = current_piece.area.getLowCol();
      const int8_t low_row = current_piece.area.getLowRow();
      placed_pieces[low_row][low_col] = true;
      placed_pieces[low_row][low_col + 1] = true;
      placed_pieces[low_row + 1][low_col] = true;
      placed_pieces[low_row + 1][low_col + 1] = true;
      break;
    }
    case Piece::PieceType::HorizontalBar: {
      const int8_t col = current_piece.area.getLowCol();
      const int8_t low_row = current_piece.area.getLowRow();
      for (int8_t add_row = 0; add_row < 4; add_row++) {
        placed_pieces[low_row + add_row][col] = true;
      }
      break;
    }
    case Piece::PieceType::VerticalBar: {
      const int8_t low_col = current_piece.area.getLowCol();
      const int8_t row = current_piece.area.getLowRow();
      for (int8_t add_col = 0; add_col < 4; add_col++) {
        placed_pieces[row][low_col + add_col] = true;
      }
      break;
    }
    }
    current_piece = Piece::randomPiece();
    clearing_lines = true;
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

  void nextTick() {
    if (game_over) {
      return;
    }
    current_tick++;
    if (current_tick % TICKS_PER_DESCENT == 0) {
      tryDescend();
    }
    if (clearing_lines) {
      if (line_to_check >= LMG::LED_MATRIX_WIDTH) {
        clearing_lines = false;
        line_to_check = 0;
      } else {
        if (isLineFull(line_to_check)) {
          clearLine(line_to_check);
          score++;
        } else {
          line_to_check++;
        }
      }
    }
    // Check if the game is over.
    for (size_t row = 0; row < LMG::LED_MATRIX_HEIGHT; row++) {
      if (placed_pieces[row][LMG::LED_MATRIX_WIDTH - 1]) {
        game_over = true;
      }
    }
    delay(GameState::MS_PER_TICK);
  }

  /// Lowers the active piece, if possible. Otherwise, places the piece where
  /// it currently is.
  void tryDescend() {
    if (canDescend()) {
      descend();
    } else {
      placeCurrentPiece();
    }
  }

  void rotatePiece() {
    switch (current_piece.ptype) {
    case Piece::PieceType::Block:
      break;
    case Piece::PieceType::VerticalBar: {
      const int8_t new_col = current_piece.area.getLowCol();
      int8_t new_low_row = current_piece.area.getLowRow();
      // Adjust the new position so that it fits on the screen
      if (new_low_row < 2) {
        new_low_row = 0;
      } else {
        new_low_row -= 2;
      }
      int8_t new_high_row = new_low_row + 4;
      if (new_high_row >= LMG::LED_MATRIX_HEIGHT) {
        new_high_row = LMG::LED_MATRIX_HEIGHT - 1;
        new_low_row = LMG::LED_MATRIX_HEIGHT - 5;
      }
      current_piece.area =
          LMG::Rect(new_low_row, new_high_row, new_col, new_col);
      current_piece.ptype = Piece::PieceType::HorizontalBar;
      break;
    }
    case Piece::PieceType::HorizontalBar: {
      const int8_t new_row = current_piece.area.getLowRow() + 2;
      int8_t new_low_col = current_piece.area.getLowCol();
      int8_t new_high_col = new_low_col + 4;
      // Adjust the new position so that it fits on the screen
      if (new_high_col >= LMG::LED_MATRIX_WIDTH) {
        new_high_col = LMG::LED_MATRIX_WIDTH - 1;
        new_low_col = LMG::LED_MATRIX_WIDTH - 5;
      }
      current_piece.area =
          LMG::Rect(new_row, new_row, new_low_col, new_high_col);
      current_piece.ptype = Piece::PieceType::VerticalBar;
      break;
    }
    }
  }

  void shiftPieceLeft() {
    if (current_piece.area.getLowRow() > 0) {
      current_piece.area.shiftRows(-1);
    }
  }

  void shiftPieceRight() {
    if (current_piece.area.getHighRow() < LMG::LED_MATRIX_HEIGHT - 1) {
      current_piece.area.shiftRows(1);
    }
  }

  void clearLine(const size_t line) {
    if (line > LMG::LED_MATRIX_HEIGHT - 1) {
      return;
    } else if (line == LMG::LED_MATRIX_HEIGHT - 1) {
      for (size_t row = 0; row < LMG::LED_MATRIX_HEIGHT; row++) {
        placed_pieces[row][line] = false;
      }
      return;
    }
    for (size_t row = 0; row < LMG::LED_MATRIX_HEIGHT; row++) {
      placed_pieces[row][line] = placed_pieces[row][line + 1];
    }
    clearLine(line + 1);
  }

  bool isLineFull(const size_t line) {
    if (line >= LMG::LED_MATRIX_HEIGHT - 1) {
      return false;
    }
    bool full{true};
    for (size_t row = 0; row < LMG::LED_MATRIX_HEIGHT; row++) {
      if (!placed_pieces[row][line]) {
        full = false;
        break;
      }
    }
    return full;
  }

  bool isGameOver() { return game_over; }

  uint32_t getScore() { return score; }
};
