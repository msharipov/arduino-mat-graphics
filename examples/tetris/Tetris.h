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
    HorizontalZigZag,
    VerticalZigZag,
    HorizontalZigZagMirror,
    VerticalZigZagMirror,
    UpT,
    RightT,
    DownT,
    LeftT,
    UpL,
    RightL,
    DownL,
    LeftL,
    UpLMirror,
    RightLMirror,
    DownLMirror,
    LeftLMirror,
  };

  /// These types can be spawned by randomPiece
  static constexpr PieceType SPAWNABLE[] = {
      PieceType::Block,
      PieceType::HorizontalBar,
      PieceType::HorizontalZigZag,
      PieceType::HorizontalZigZagMirror,
      PieceType::UpT,
      PieceType::DownT,
      PieceType::RightL,
      PieceType::LeftL,
      PieceType::RightLMirror,
      PieceType::LeftLMirror,
  };

  static constexpr size_t SPAWNABLE_COUNT{sizeof(SPAWNABLE) /
                                          sizeof(PieceType)};

  static constexpr bool SPRITES[][6] = {
      {true, true, true, true},               // Block
      {true, true, true, true},               // HorizontalBar
      {true, true, true, true},               // VerticalBar
      {false, true, true, true, true, false}, // HorizontalZigZag
      {true, true, false, false, true, true}, // VerticalZigZag
      {true, false, true, true, false, true}, // HorizontalZigZagMirror
      {false, true, true, true, true, false}, // VerticalZigZagMirror
      {false, true, true, true, false, true}, // UpT
      {false, true, false, true, true, true}, // RightT
      {true, false, true, true, true, false}, // DownT
      {true, true, true, false, true, false}, // LeftT
      {true, true, true, true, false, false}, // UpL
      {true, true, false, true, false, true}, // RightL
      {false, false, true, true, true, true}, // DownL
      {true, false, true, false, true, true}, // LeftL
      {true, false, false, true, true, true}, // UpLMirror
      {true, true, true, false, true, false}, // RightLMirror
      {true, true, true, false, false, true}, // DownLMirror
      {false, true, false, true, true, true}, // LeftLMirror
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
    case PieceType::VerticalZigZag:
      ptype = PieceType::HorizontalZigZag;
      [[fallthrough]];
    case PieceType::HorizontalZigZag:
      area = LMG::Rect(2, 4, 10, 11);
      break;
    case PieceType::VerticalZigZagMirror:
      ptype = PieceType::HorizontalZigZagMirror;
      [[fallthrough]];
    case PieceType::HorizontalZigZagMirror:
      area = LMG::Rect(2, 4, 10, 11);
      break;
    case PieceType::RightT:
      [[fallthrough]];
    case PieceType::LeftT:
      ptype = PieceType::UpT;
      [[fallthrough]];
    case PieceType::UpT:
      [[fallthrough]];
    case PieceType::DownT:
      area = LMG::Rect(2, 4, 10, 11);
      break;
    case PieceType::UpL:
      [[fallthrough]];
    case PieceType::DownL:
      ptype = PieceType::RightL;
      [[fallthrough]];
    case PieceType::RightL:
      [[fallthrough]];
    case PieceType::LeftL:
      area = LMG::Rect(2, 4, 10, 11);
      break;
    case PieceType::UpLMirror:
      [[fallthrough]];
    case PieceType::DownLMirror:
      ptype = PieceType::RightLMirror;
      [[fallthrough]];
    case PieceType::RightLMirror:
      [[fallthrough]];
    case PieceType::LeftLMirror:
      area = LMG::Rect(2, 4, 10, 11);
      break;
    }
  }

  /// Places a random piece from SPAWNABLE at the top of the screen
  static Piece randomPiece() {
    return Piece(Piece::SPAWNABLE[rand() % Piece::SPAWNABLE_COUNT]);
  }

  /// Returns the type that this piece turns into when rotated
  PieceType nextVariant() {
    switch (ptype) {
    case PieceType::Block:
      return PieceType::Block;
    case PieceType::HorizontalBar:
      return PieceType::VerticalBar;
    case PieceType::VerticalBar:
      return PieceType::HorizontalBar;
    case PieceType::VerticalZigZag:
      return PieceType::HorizontalZigZag;
    case PieceType::HorizontalZigZag:
      return PieceType::VerticalZigZag;
    case PieceType::VerticalZigZagMirror:
      return PieceType::HorizontalZigZagMirror;
    case PieceType::HorizontalZigZagMirror:
      return PieceType::VerticalZigZagMirror;
    case PieceType::UpT:
      return PieceType::RightT;
    case PieceType::RightT:
      return PieceType::DownT;
    case PieceType::DownT:
      return PieceType::LeftT;
    case PieceType::LeftT:
      return PieceType::UpT;
    case PieceType::UpL:
      return PieceType::RightL;
    case PieceType::RightL:
      return PieceType::DownL;
    case PieceType::DownL:
      return PieceType::LeftL;
    case PieceType::LeftL:
      return PieceType::UpL;
    case PieceType::UpLMirror:
      return PieceType::RightLMirror;
    case PieceType::RightLMirror:
      return PieceType::DownLMirror;
    case PieceType::DownLMirror:
      return PieceType::LeftLMirror;
    case PieceType::LeftLMirror:
      return PieceType::UpLMirror;
    }
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

  /// Places the active piece where it currently is on the screen.
  /*
   *  Activates line clearing. Spawns the next piece at the top of the screen.
   */
  void placeCurrentPiece() {
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
        if (part_of_active_piece) {
          placed_pieces[low_row + add_row][low_col + add_col] = true;
        }
      }
    }
    current_piece = Piece::randomPiece();
    clearing_lines = true;
  }

  /// Draws the placed pieces to a Frame.
  LMG::Frame drawPlaced() {
    LMG::Frame placed{};
    placed.drawSprite(
        &placed_pieces[0][0],
        LMG::Rect(0, LMG::LED_MATRIX_HEIGHT - 1, 0, LMG::LED_MATRIX_WIDTH - 1));
    return placed;
  }

  /// Draws the active piece to a Frame.
  LMG::Frame drawActive() {
    LMG::Frame active{};
    active.drawSprite(Piece::SPRITES[static_cast<int32_t>(current_piece.ptype)],
                      current_piece.area);
    return active;
  }

  /// Updates the game state by one tick.
  /*
   *  If the game is in line clearing mode, clears a single line.
   */
  void nextTick() {
    if (game_over) {
      return;
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
    } else {
      current_tick++;
      if (current_tick % TICKS_PER_DESCENT == 0) {
        tryDescend();
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

  /// Attempts to rotate the active piece clockwise.
  void rotatePiece() {
    const Piece::PieceType next_type = current_piece.nextVariant();
    const int8_t low_row = current_piece.area.getLowRow();
    const int8_t high_row = current_piece.area.getHighRow();
    const int8_t height = high_row - low_row + 1;
    const int8_t low_col = current_piece.area.getLowCol();
    const int8_t high_col = current_piece.area.getHighCol();
    const int8_t width = high_col - low_col + 1;
    const int8_t new_width = height;
    const int8_t new_height = width;
    const int8_t new_high_col = low_col + new_width - 1;
    if (new_high_col > LMG::LED_MATRIX_WIDTH - 1) {
      return; // Do not rotate if the top would be out of bounds.
    }
    int8_t new_low_row = (low_row + high_row) / 2 - (new_height - 1) / 2;
    if (new_low_row < 0) {
      new_low_row = 0;
    }
    int8_t new_high_row = new_low_row + new_height - 1;
    if (new_high_row >= LMG::LED_MATRIX_HEIGHT - 1) {
      new_high_row = LMG::LED_MATRIX_HEIGHT - 1;
      new_low_row = new_high_row - new_height + 1;
    }
    const Piece::PieceType next_ptype = current_piece.nextVariant();
    const LMG::Rect new_area =
        LMG::Rect(new_low_row, new_high_row, low_col, new_high_col);
    for (int8_t add_row = 0; add_row < new_height; add_row++) {
      for (int8_t add_col = 0; add_col < new_width; add_col++) {
        const bool part_of_rotated_piece =
            Piece::SPRITES[static_cast<size_t>(next_ptype)]
                          [add_row * new_width + add_col];
        const bool space_occupied =
            placed_pieces[new_low_row + add_row][low_col + add_col];
        if (part_of_rotated_piece && space_occupied) {
          return; // Do nothing if rotation is impossible.
        }
      }
    }
    // If there are no conflicts, update the active piece.
    current_piece.ptype = next_ptype;
    current_piece.area = new_area;
  }

  /// Check is the active piece can shift to the left
  bool canShiftLeft() {
    const int8_t low_row = current_piece.area.getLowRow();
    if (low_row < 1) {
      return false;
    }
    const int8_t low_col = current_piece.area.getLowCol();
    const int8_t width = current_piece.area.getHighCol() - low_col + 1;
    for (int8_t add_col = 0; add_col < width; add_col++) {
      const bool part_of_active_piece =
          Piece::SPRITES[static_cast<size_t>(current_piece.ptype)][add_col];
      const bool left_is_occupied =
          placed_pieces[low_row - 1][low_col + add_col];
      if (part_of_active_piece && left_is_occupied) {
        return false;
      }
    }
    return true;
  }

  /// Check is the active piece can shift to the right
  bool canShiftRight() {
    const int8_t high_row = current_piece.area.getHighRow();
    if (high_row >= LMG::LED_MATRIX_HEIGHT - 1) {
      return false;
    }
    const int8_t low_col = current_piece.area.getLowCol();
    const int8_t width = current_piece.area.getHighCol() - low_col + 1;
    for (int8_t add_col = 0; add_col < width; add_col++) {
      const bool part_of_active_piece =
          Piece::SPRITES[static_cast<size_t>(current_piece.ptype)]
                        [high_row * width + add_col];
      const bool right_is_occupied =
          placed_pieces[high_row + 1][low_col + add_col];
      if (part_of_active_piece && right_is_occupied) {
        return false;
      }
    }
    return true;
  }

  /// Shifts the piece to the left (lower row)
  void shiftPieceLeft() {
    if (canShiftLeft()) {
      current_piece.area.shiftRows(-1);
    }
  }

  /// Shifts the piece to the right (higher row)
  void shiftPieceRight() {
    if (canShiftRight()) {
      current_piece.area.shiftRows(1);
    }
  }

  /// Clears all blocks in a line and shifts the all higher lines down.
  void clearLine(const size_t line) {
    if (line > LMG::LED_MATRIX_WIDTH - 1) {
      return;
    } else if (line == LMG::LED_MATRIX_WIDTH - 1) {
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

  /// Checks if all spaces in a line are occupied.
  bool isLineFull(const size_t line) {
    if (line >= LMG::LED_MATRIX_WIDTH - 1) {
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
