/*!
 *  Copyright 2024 Maxim Sharipov (msharipovr@gmail.com).
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

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

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
 *       ║  ▒▒▒▒▒     0:::::::::11 ••║
 *       ║  ▒▒▒▒▒ row :::::::::::: ••║
 *       ║  ▒▒▒▒▒   ↓ ::::::::::::   ║
 *   DC █████         7:::::::::::   ║
 * jack █████                       ○║
 *       ║    ○ ••• ◘◘◘◘◘◘◘◘ ◘◘◘◘◘◘ ╔╝
 *       ╚══════════════════════════╝
 */
namespace LMG {

/// Number of rows in the LED matrix.
constexpr uint8_t LED_MATRIX_HEIGHT{8};

/// Number of columns in the LED matrix.
constexpr uint8_t LED_MATRIX_WIDTH{12};

/// Represents a rectangular subregion of the LED matrix.
class Rect {
  friend class Frame;
  uint8_t low_row;
  uint8_t high_row;
  uint8_t low_col;
  uint8_t high_col;

  /// Default constructor is private to prevent invalid rectangles
  Rect() {}

public:
  /// Creates a rectangle that is bounded by two rows and two columns.
  /**
   * @param row_a,row_b   Rows that contain horizontal boundaries of the
   *                      rectangle.
   * @param col_a,col_b   Columns that contain vertical boundaries of the
   *                      rectangle.
   *
   * The ranges are inclusive. For example, `Rect(5, 2, 3, 7)` contains all
   * points for which 2 <= row <= 5 and 3 <= column <= 7.
   */
  Rect(uint8_t row_a, uint8_t row_b, uint8_t col_a, uint8_t col_b);

  /// Returns the intersection of two rectangles.
  /**
   * @param other The other rectangle.
   * @returns A rectangle that corresponds to the intersection of the two
   *          operands, if it exists. If the rectangles do not intersect,
   *          returns std::nullopt.
   */
  std::optional<Rect> operator&(const Rect &other);

  /// Returns the lower of the two bounding rows.
  /**
   * @returns The lowest row in the rectangle.
   */
  uint8_t getLowRow();

  /// Returns the lower of the two bounding columns.
  /**
   * @returns The lowest column in the rectangle.
   */
  uint8_t getLowCol();

  /// Returns the higher of the two bounding rows.
  /**
   * @returns The highest row in the rectangle.
   */
  uint8_t getHighRow();

  /// Returns the higher of the two bounding columns.
  /**
   * @returns The highest column in the rectangle.
   */
  uint8_t getHighCol();

  /// Shifts the rectangle across rows.
  /**
   * @param shift How many rows to shift the rectangle by.
   *
   * Does not affect the columns of the rectangle. If the rectangle would be
   * shifted into the negative rows, sets the lowest row to 0 instead.
   * Similarly prevents overflow if the shift value is too high.
   */
  void shiftRows(int8_t shift);

  /// Shifts the rectangle across columns.
  /**
   * @param shift How many columns to shift the rectangle by.
   *
   * Does not affect the rows of the rectangle. If the rectangle would be
   * shifted into the negative columns, sets the lowest column to 0 instead.
   * Similarly prevents overflow if the shift value is too high.
   */
  void shiftColumns(int8_t shift);
};

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

  /// Overlays the two frames.
  /**
   * @param other The other frame.
   * @returns A new frame where an LED is on if the same LED is on in either of
   *          the two frames.
   */
  Frame operator+(const Frame &other);

  /// Compute the intersection of two frames.
  /**
   * @param other The other frame.
   * @returns A new frame where an LED is on only if the same LED is on in both
   *          of the two frames.
   */
  Frame operator&(const Frame &other);

  /// Checks if any LEDs are on in the frame.
  /**
   * @returns True, if at least one LED is on in the frame; false, otherwise.
   */
  explicit operator bool();

  /// Sets the state of a single LED.
  /**
   * @param row The row in which the LED is located.
   * @param col The column in which the LED is located.
   * @param bit Whether the LED should be on.
   *
   * If the LED position is out of bounds, this function does nothing.
   */
  void inline setLED(const uint8_t row, const uint8_t col, const bool bit) {
    if (row < LED_MATRIX_HEIGHT && col < LED_MATRIX_WIDTH) {
      constexpr uint32_t TOP_BIT = 1L << 31;
      const uint8_t pos = row * LED_MATRIX_WIDTH + col;

      // Each part of the data array has 32 bits, so we divide by 2^5 to
      // determine which part of the array to update.
      const uint8_t data_index = pos >> 5;

      // The remainder determines which bit should be updated.
      const uint8_t rem = pos % 32;

      if (bit) {
        data[data_index] |= (TOP_BIT >> rem);
      } else {
        data[data_index] &= ~(TOP_BIT >> rem);
      }
    }
  }

  /// Inverts the state of a single LED.
  /**
   * @param row The row in which the LED is located.
   * @param col The column in which the LED is located.
   *
   * If the LED position is out of bounds, this function does nothing.
   */
  void inline invertLED(const uint8_t row, const uint8_t col) {
    if (row < LED_MATRIX_HEIGHT && col < LED_MATRIX_WIDTH) {
      constexpr uint32_t TOP_BIT = 1L << 31;
      const uint8_t pos = row * LED_MATRIX_WIDTH + col;

      // Each part of the data array has 32 bits, so we divide by 2^5 = 32 to
      // determine which part of the array to update.
      const uint8_t data_index = pos >> 5;

      // The remainder determines which bit should be updated.
      const uint8_t rem = pos % 32;

      data[data_index] ^= (TOP_BIT >> rem);
    }
  }

  /// Sets the state of all LEDs within a rectangle.
  /**
   * @param area The rectangle of LEDs that will be modified.
   * @param bit  Determines whether the LEDs are switched on or off.
   */
  void fillRect(const Rect &area, const bool bit);

  /// Inverts the state of all LEDs within a rectangle.
  /**
   * @param area The rectangle of LEDs that will be flipped.
   */
  void invertRect(const Rect &area);

  /// Draws a sprite to the LED matrix.
  /**
   * @param sprite Pointer to the sprite data.
   * @param area   Area of the LED matrix where the sprite should be drawn.
   *
   * The sprite must be laid out sequentially in memory row-by-row. For
   * example, given a sprite with 3 rows and 4 columns, if our sprite array
   * looks like this:
   *
   *  `bool s[3*4] = {true, false, false, ... , true};`
   *
   *  then the sprite would be drawn as:
   *
   *  `s[0]  s[1]  s[2]  s[3]`
   *  `s[4]  s[5]  s[6]  s[7]`
   *  `s[8]  s[5] s[10] s[11]`
   */
  void drawSprite(const bool *sprite, const Rect &area);
};

// 3-by-5 digits
/*
 |  █  |  █  | ██  | ███ | █ █ | ███ | ███ | ███ | ███ | ███ |
 | █ █ | ██  |   █ |   █ | █ █ | █   | █   |   █ | █ █ | █ █ |
 | █ █ |  █  |  █  | ███ | ███ | ███ | ███ |   █ | ███ | ███ |
 | █ █ |  █  | █   |   █ |   █ |   █ | █ █ |  █  | █ █ |   █ |
 |  █  | ███ | ███ | ███ |   █ | ███ | ███ |  █  | ███ | ██  |
*/
extern const bool DIGITS_3x5[10][15];

// 3-by-4 letters and digits
/*
 |  █  | █   |  ██ | ██  |  ██ |  ██ |
 | █ █ | ███ | █   | █ █ | ███ | █   |
 | ███ | █ █ | █   | █ █ | █   | ███ |
 | █ █ | ███ |  ██ | ██  |  ██ | █   |

 |  ██ | █ █ | ███ | ███ | █ █ | █   |
 | █   | █ █ |  █  |  █  | ██  | █   |
 | █ █ | ███ |  █  |  █  | █ █ | █   |
 | ███ | █ █ | ███ | █   | █ █ | ███ |

 | █ █ |     | ███ | ███ |  ██ | ███ |
 | ███ | ███ | █ █ | █ █ | █ █ | █ █ |
 | █ █ | █ █ | █ █ | ███ | ██  | ██  |
 | █ █ | █ █ | ███ | █   |   █ | █ █ |

 | ███ | ███ | █ █ | █ █ |     | █ █ |
 | █   |  █  | █ █ | █ █ | █ █ |  █  |
 |   █ |  █  | █ █ | █ █ | ███ |  █  |
 | ███ |  █  | ███ |  █  | ███ | █ █ |

 | █ █ | ███ |  █  |  █  | ██  | ███ |
 | █ █ |  ██ | █ █ | ██  |   █ |   █ |
 |  █  | ██  | █ █ |  █  | █   |   █ |
 |  █  | ███ |  █  | ███ | ███ | ███ |

 | █ █ | ███ | ███ | ███ | ███ | ███ |
 | █ █ | █   | █   |   █ | ███ | ███ |
 | ███ |   █ | ███ |  █  | █ █ |   █ |
 |   █ | ███ | ███ |  █  | ███ | ███ |
*/
extern const bool DEFAULT_FONT_3x4[36][12];
} // namespace LMG
