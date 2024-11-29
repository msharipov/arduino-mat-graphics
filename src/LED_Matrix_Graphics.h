/*!
 *  Created by Maxim Sharipov (msharipovr@gmail.com) 2024.
 *
 *  MIT license, all text above must be included in any redistribution
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

#ifndef MATRIX_PRINTING_H
#define MATRIX_PRINTING_H

#include <array>
#include <cstddef>
#include <cstdint>

/**
 * Whenever this library refers to the rows and columns of the LED matrix, both
 * are indexed from 0. The LED that is closest to the center of the board is
 * located at (0,0). The columns range from 0 to 11 and the rows range from 0
 * to 7, inclusively.
 *
 *       ╔══════════════════════════╗
 *       ║[o] ○ ◘◘◘◘◘◘◘◘◘◘ ◘◘◘◘◘◘◘◘ ║
 *       ║                          ║
 *  usb ▒▒▒▒    •••          ≡███≡  ╚╗
 *      ▒▒▒▒    •••          ≡███≡  ○║
 *       ║            col →        ••║
 *       ║  ▒▒▒▒▒     0፡፡፡፡፡፡፡፡፡11 ••║
 *       ║  ▒▒▒▒▒ row ፡፡፡፡፡፡፡፡፡፡፡፡ ••║
 *       ║  ▒▒▒▒▒   ↓ ፡፡፡፡፡፡፡፡፡፡፡፡   ║
 *   DC █████         7፡፡፡፡፡፡፡፡፡፡፡   ║
 * jack █████                       ○║
 *       ║    ○ ••• ◘◘◘◘◘◘◘◘ ◘◘◘◘◘◘ ╔╝
 *       ╚══════════════════════════╝
 */
namespace LMG {

/// Number of rows in the LED matrix.
constexpr uint8_t LED_MATRIX_HEIGHT{8};

/// Number of columns in the LED matrix.
constexpr uint8_t LED_MATRIX_WIDTH{12};

/// Stores the state of the LED matrix.
class Frame {
  std::array<uint32_t, 3> data{0, 0, 0};

public:
  /// Constructs a frame with all lights off.
  Frame() {}

  /// Returns the frame data as an array of 32-bit integers.
  /**
   * The `loadFrame` function from the Arduino LED Matrix library takes an array
   * of three 32-bit unsigned integers as input. If we have an
   * `ArduinoLEDMatrix matrix` and a `Frame f`, then
   * `matrix.loadFrame(f.getData())` will update the LED matrix with the current
   * state of `f`.
   *
   * @returns A raw pointer to the data array.
   */
  const uint32_t *getData();

  /// Sets the state of a single LED.
  /**
   * @param row The row in which the LED is located.
   * @param col The column in which the LED is located.
   * @param bit Whether the LED should be on.
   */
  void inline setLED(const uint8_t row, const uint8_t col, const bool bit);

  /// Inverts the state of a single LED.
  /**
   * @param row The row in which the LED is located.
   * @param col The column in which the LED is located.
   */
  void invertLED(const uint8_t row, const uint8_t col);

  // Draws a [width]-by-[height] symbol to the LED matrix with its bottom
  // right corner at coordinates given by [row] and [column]. All LEDs
  // used by the symbol must lie within the matrix.
  void put_sym(const bool *symbols, const int8_t row, const int8_t col,
               const int8_t width, const int8_t height);

  // Works like put_sym() but skips every position that is outside of the
  // rectangle given by vertices ([row_l], [col_l]) and ([row_h], [col_h]).
  void put_sym_bnd(const bool *symbols, const int8_t row, const int8_t col,
                   const int8_t width, const int8_t height, const int8_t row_l,
                   const int8_t col_l, const int8_t row_h, const int8_t col_h);

  // Sets all LEDs in a rectangle given by vertices ([row_l], [col_l]) and
  // ([row_h], [col_h]) to the value of [bit].
  void fill_rect(const int8_t row_l, const int8_t col_l, const int8_t row_h,
                 const int8_t col_h, const bool bit);

  // Inverts all LEDs in a rectangle given by vertices ([row_l], [col_l]) and
  // ([row_h], [col_h]).
  void invert_rect(const int8_t row_l, const int8_t col_l, const int8_t row_h,
                   const int8_t col_h);

  // Prints [msg] of length [msg_len] inside of a text box between [col_l] and
  // [col_h], with [row] being the lowest row and [spacing] being the amount
  // of empty space between each symbol. [step] shifts the text inside of the
  // text box to the right (step<0) and to the left (step>0).
  void draw_text_3_4(const bool symbols[][12], int8_t col_h, int8_t col_l,
                     const int8_t row, const uint8_t msg[],
                     const std::size_t msg_len, const uint8_t spacing,
                     const int8_t step);
};

// 3-by-5 digits
/*
 |  #  |  #  | ##  | ### | # # | ### | ### | ### | ### | ### |
 | # # | ##  |   # |   # | # # | #   | #   |   # | # # | # # |
 | # # |  #  |  #  | ### | ### | ### | ### |   # | ### | ### |
 | # # |  #  | #   |   # |   # |   # | # # |  #  | # # |   # |
 |  #  | ### | ### | ### |   # | ### | ### |  #  | ### | ##  |
*/
extern const bool DIGITS_35[10][15];

// 3-by-4 letters and digits
/*
 |  #  | #   |  ## | ##  |  ## |  ## |
 | # # | ### | #   | # # | ### | #   |
 | ### | # # | #   | # # | #   | ### |
 | # # | ### |  ## | ##  |  ## | #   |

 |  ## | # # | ### | ### | # # | #   |
 | #   | # # |  #  |  #  | ##  | #   |
 | # # | ### |  #  |  #  | # # | #   |
 | ### | # # | ### | #   | # # | ### |

 | # # |     | ### | ### |  ## | ### |
 | ### | ### | # # | # # | # # | # # |
 | # # | # # | # # | ### | ##  | ##  |
 | # # | # # | ### | #   |   # | # # |

 | ### | ### | # # | # # |     | # # |
 | #   |  #  | # # | # # | # # |  #  |
 |   # |  #  | # # | # # | ### |  #  |
 | ### |  #  | ### |  #  | ### | # # |

 | # # | ### |  #  |  #  | ##  | ### |
 | # # |  ## | # # | ##  |   # |   # |
 |  #  | ##  | # # |  #  | #   |   # |
 |  #  | ### |  #  | ### | ### | ### |

 | # # | ### | ### | ### | ### | ### |
 | # # | #   | #   |   # | ### | ### |
 | ### |   # | ### |  #  | # # |   # |
 |   # | ### | ### |  #  | ### | ### |
*/
extern const bool DEFAULT_FONT_3_4[36][12];
} // namespace LMG
#endif
