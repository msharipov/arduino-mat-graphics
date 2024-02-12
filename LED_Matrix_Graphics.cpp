#include "LED_Matrix_Graphics.h"
#include <stdint.h>

const bool DIGITS_35[10][15] = {
  {0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0},
  {1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0},
  {1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1},
  {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1},
  {1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
  {0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
  {0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1}
};

const bool SYMBOLS_34[36][12] = {
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


void matrix_set_bit(uint32_t f[],
                    const int8_t row,
                    const int8_t col,
                    const bool bit) {
  
  uint8_t q, r, pos;
  
  pos = row*12 + col;

  if (pos < 32) {
    q = 2;
  } else if (pos < 64) {
    q = 1;
  } else {
    q = 0;
  }
  r = pos % 32;
  
  if (bit) {
    f[q] |= (1UL << r);
  } else {
    f[q] &= ~(1UL << r);
  }
}


void dig35_to_mat(uint32_t f[],
                  const int8_t row,
                  const int8_t col, 
                  const uint8_t dig) {

  for (std::size_t add_row = 0; add_row < 5; add_row++) {
    for (std::size_t add_col = 0; add_col < 3; add_col++) {
      matrix_set_bit(f, row + add_row, col + add_col,
                     DIGITS_35[dig][add_col + add_row*3]);
    }
  }
}


void sym34_to_mat(uint32_t f[],
                  const int8_t row,
                  const int8_t col,
                  const bool * symbols,
                  const uint8_t sym) {
  
  LMG_put_sym(f, symbols + static_cast<size_t>(12*sym), row, col, 3, 4);
}


void sym34_to_mat_bnd(uint32_t f[],
                      const int8_t row,
                      const int8_t col,
                      const bool * symbols,
                      const uint8_t sym,
                      const int8_t end,
                      const int8_t start) {
  
  LMG_put_sym_bnd(f, symbols + static_cast<size_t>(12*sym), row, col, 3, 4,
                  row, start, row + 3, end);
}


void LMG_put_sym(uint32_t f[],
                 const bool * symbol,
                 const int8_t row,
                 const int8_t col,
                 const int8_t width,
                 const int8_t height) {
  
  for (int8_t add_col = 0; add_col < width; add_col++) {
    for (int8_t add_row = 0; add_row < height; add_row++) {
      
      matrix_set_bit(f, row + add_row, col + add_col, 
                     *(symbol + sizeof(bool)*(add_col + add_row*width)));
    }
  }
}


void LMG_put_sym_bnd(uint32_t f[],
                     const bool * symbol,
                     const int8_t row,
                     const int8_t col,
                     const int8_t width,
                     const int8_t height,
                     const int8_t row_l,
                     const int8_t col_l,
                     const int8_t row_h,
                     const int8_t col_h) {

  for (int8_t add_col = 0; add_col < width; add_col++) {

    if (col + add_col > col_h || col + add_col < col_l) {
      continue;
    }

    for (int8_t add_row = 0; add_row < height; add_row++) {

      if (row + add_row > row_h || row + add_row < row_l) {
        continue;
      }
      
      matrix_set_bit(f, row + add_row, col + add_col, 
                     *(symbol + sizeof(bool)*(add_col + add_row*width)));
    }
  }
}



void mat_fill_rect(uint32_t f[],
                   const int8_t row_l,
                   const int8_t col_l,
                   const int8_t row_h,
                   const int8_t col_h,
                   const bool bit) {

  for (int8_t col = col_l; col <= col_h; col++) {
    for (int8_t row = row_l; row <= row_h; row++) {
      matrix_set_bit(f, row, col, bit);
    }
  }
}


void mat_text_34(uint32_t f[],
                 const bool symbols[][12],
                 int8_t col_h,
                 int8_t col_l,
                 const int8_t row,
                 const uint8_t msg[], 
                 const std::size_t msg_len,
                 const uint8_t spacing,
                 const int8_t step) {

  col_l = (col_l < 0) ? 0 : col_l;
  col_h = (col_h > 11) ? 11 : col_h;

  const int8_t block_width = 3 + spacing;
  int8_t col = col_h - (2 - step);

  // Skip the printing if the text is completely out of bounds.
  // Warning: Removing the type cast causes incorrect integer promotion!
  if (step >= (int16_t)(msg_len*block_width - spacing)) {
    mat_fill_rect(f, row, col_l, row + 3, col_h, 0);
    return;
  }

  // Left-side spacing
  if (step < 0) {
    mat_fill_rect(f, row, col + 3, row + 3, col_h, 0);
  }

  for (int8_t sym = 0; sym < msg_len; sym++) {

    if (col > 11) {
      col -= block_width;
      continue;
    }

    if (col < -2) {
      break;
    }

    // Tail spacing before first symbol
    if (spacing != 0 && col_h - col >= 2 && col_h - col < block_width) {
      mat_fill_rect(f, row, col + 3, row + 3, col_h, 0);
    }

    LMG_put_sym_bnd(f, symbols[msg[sym]], row, col, 3, 4,
                    row, col_l, row + 3, col_h);
    
    // Tail spacing
    if (spacing != 0 && col > col_l) {

      int8_t spc_end = col - spacing;
      if (spc_end < col_l) {
        spc_end = col_l;
      }

      mat_fill_rect(f, row, spc_end, row + 3, col - 1, 0);
    }

    col -= block_width;
  }

  // Right-side space
  if (col > col_l) {
    mat_fill_rect(f, row, col_l, row + 3, col - 1, 0);
  }
}
