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

Rect::Rect(int8_t row_a, int8_t row_b, int8_t col_a, int8_t col_b)
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

int8_t Rect::getLowRow() { return low_row; }

int8_t Rect::getLowCol() { return low_col; }

int8_t Rect::getHighRow() { return high_row; }

int8_t Rect::getHighCol() { return high_col; }

void Rect::shiftRows(int8_t shift) {
  low_row += shift;
  high_row += shift;
}

void Rect::shiftColumns(int8_t shift) {
  low_col += shift;
  high_col += shift;
}

const uint32_t *Frame::getData() { return data.data(); }

Frame Frame::operator+(const Frame &other) {
  Frame sum = Frame();
  for (size_t i = 0; i < 3; i++) {
    sum.data[i] = data[i] | other.data[i];
  }
  return sum;
}

Frame Frame::operator&(const Frame &other) {
  Frame intersection = Frame();
  for (size_t i = 0; i < 3; i++) {
    intersection.data[i] = data[i] & other.data[i];
  }
  return intersection;
}

Frame::operator bool() { return data[0] || data[1] || data[2]; }

void Frame::fillRect(const Rect &area, const bool bit) {
  for (int8_t col = area.low_col; col <= area.high_col; col++) {
    for (int8_t row = area.low_row; row <= area.high_row; row++) {
      setLED(row, col, bit);
    }
  }
}

void Frame::invertRect(const Rect &area) {
  for (int8_t col = area.low_col; col <= area.high_col; col++) {
    for (int8_t row = area.low_row; row <= area.high_row; row++) {
      invertLED(row, col);
    }
  }
}

void Frame::drawSprite(const bool *data, const Rect &area) {
  const int8_t width = area.high_col - area.low_col + 1;
  const int8_t height = area.high_row - area.low_row + 1;
  for (int8_t sprite_col = 0; sprite_col < width; sprite_col++) {
    for (int8_t sprite_row = 0; sprite_row < height; sprite_row++) {
      setLED(area.low_row + sprite_row, area.low_col + sprite_col,
             data[sprite_row * width + sprite_col]);
    }
  }
}

const bool DEFAULT_FONT_3x5[39][15] = {
    {0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1}, // A
    {1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1}, // B
    {0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1}, // C
    {1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0}, // D
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1}, // E
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0}, // F
    {0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1}, // G
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1}, // H
    {1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1}, // I
    {1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0}, // J
    {1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1}, // K
    {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1}, // L
    {1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0}, // M left
    {0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1}, // M right
    {1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0}, // N left
    {0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1}, // N right
    {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1}, // O
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0}, // P
    {0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1}, // Q
    {1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1}, // R
    {0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0}, // S
    {1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0}, // T
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1}, // U
    {1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1}, // V
    {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0}, // W left
    {0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0}, // W right
    {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1}, // X
    {1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0}, // Y
    {1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1}, // Z
    {0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0}, // 0
    {0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1}, // 1
    {1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1}, // 2
    {1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0}, // 3
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1}, // 4
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1}, // 5
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0}, // 7
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0}  // 9
};

const bool DEFAULT_FONT_3x4[36][12] = {
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1}, // A
    {1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1}, // B
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1}, // C
    {0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1}, // D
    {0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1}, // E
    {0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0}, // F
    {0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0}, // G
    {1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1}, // H
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0}, // I
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0}, // J
    {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1}, // K
    {1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0}, // L
    {0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1}, // M
    {0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1}, // N
    {0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1}, // O
    {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0}, // P
    {0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1}, // Q
    {0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0}, // R
    {0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0}, // S
    {0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0}, // T
    {0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1}, // U
    {0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0}, // V
    {0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1}, // W
    {0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}, // X
    {1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0}, // Y
    {1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1}, // Z
    {0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0}, // 0
    {0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1}, // 1
    {1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1}, // 2
    {1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1}, // 3
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1}, // 4
    {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1}, // 5
    {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1}  // 9
};

} // namespace LMG
