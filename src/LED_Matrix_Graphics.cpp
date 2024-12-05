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

#include "LED_Matrix_Graphics.h"

namespace LMG {

Rect::Rect(uint8_t row_a, uint8_t row_b, uint8_t col_a, uint8_t col_b)
    : low_row(row_a), high_row(row_a), low_col(col_a), high_col(col_a) {

  if (row_b > row_a) {
    high_row = row_b;
  } else {
    low_row = row_b;
  }

  if (col_b > col_a) {
    high_col = col_b;
  } else {
    low_col = col_b;
  }
}

std::optional<Rect> Rect::operator&(const Rect &other) {
  const bool disjoint_vertically =
      low_row > other.high_row || other.low_row > high_row;
  const bool disjoint_horizontally =
      low_col > other.high_col || other.low_col > high_col;
  if (disjoint_vertically || disjoint_horizontally) {
    return std::nullopt;
  }

  return Rect{
      std::max(low_row, other.low_row), std::min(high_row, other.high_row),
      std::max(low_col, other.low_col), std::min(high_col, other.high_col)};
}

const uint32_t *Frame::getData() { return data.data(); }

Frame Frame::operator+(const Frame &other) {
  Frame sum = Frame();
  for (size_t i = 0; i < 3; i++) {
    sum.data[i] = data[i] | other.data[i];
  }
  return sum;
}

void Frame::fillRect(const Rect &area, const bool bit) {
  for (uint8_t col = area.low_col; col <= area.high_col; col++) {
    for (uint8_t row = area.low_row; row <= area.high_row; row++) {
      setLED(row, col, bit);
    }
  }
}

void Frame::invertRect(const Rect &area) {
  for (uint8_t col = area.low_col; col <= area.high_col; col++) {
    for (uint8_t row = area.low_row; row <= area.high_row; row++) {
      invertLED(row, col);
    }
  }
}

void Frame::drawSprite(const bool *data, const uint8_t row, const uint8_t col,
                       const uint8_t width, const uint8_t height) {
  for (uint8_t add_col = 0; add_col < width; add_col++) {
    for (uint8_t add_row = 0; add_row < height; add_row++) {
      setLED(row + add_row, col + add_col,
             *(data + sizeof(bool) * (add_col + add_row * width)));
    }
  }
}

const bool DIGITS_35[10][15] = {{0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0},
                                {1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
                                {1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1},
                                {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1},
                                {1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1},
                                {1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1},
                                {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
                                {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1},
                                {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
                                {0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1}};

const bool DEFAULT_FONT_3_4[36][12] = {
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0}, // A
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1}, // B
    {1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0}, // C
    {0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1}, // D
    {1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0}, // E
    {0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0}, // F
    {1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0}, // G
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1}, // H
    {1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1}, // I
    {0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1}, // J
    {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1}, // K
    {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1}, // L
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1}, // M
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0}, // N
    {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1}, // O
    {0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1}, // P
    {1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0}, // Q
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1}, // R
    {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1}, // S
    {0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1}, // T
    {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1}, // U
    {0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1}, // V
    {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0}, // W
    {1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1}, // X
    {0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1}, // Y
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1}, // Z
    {0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0}, // 0
    {1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0}, // 1
    {1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1}, // 3
    {1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1}, // 4
    {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1}, // 5
    {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}, // 6
    {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1}, // 7
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1}  // 9
};

} // namespace LMG
