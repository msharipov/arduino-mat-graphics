#ifndef MATRIX_PRINTING_H
#define MATRIX_PRINTING_H

#include <stdint.h>
#include <cstddef>

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
extern const bool SYMBOLS_34[36][12];

void matrix_set_bit(uint32_t f[],
                    const int8_t row,
                    const int8_t col,
                    const bool bit);

void dig35_to_mat(uint32_t f[],
                  const int8_t row,
                  const int8_t col, 
                  const uint8_t dig);

void sym34_to_mat(uint32_t f[],
                  const int8_t row,
                  const int8_t col,
                  const bool symbols[][12],
                  const uint8_t sym);

void sym34_to_mat_bnd(uint32_t f[],
                      const int8_t row,
                      const int8_t col,
                      const bool symbols[][12],
                      const uint8_t sym,
                      const int8_t end,
                      const int8_t start);

// Draws a [width]-by-[height] symbol to the LED matrix with its bottom
// right corner at coordinates given by [row] and [column]. All LEDs
// used by the symbol must lie within the matrix.
void LMG_put_sym(uint32_t f[],
                 const bool * symbols,
                 const int8_t row,
                 const int8_t col,
                 const int8_t width,
                 const int8_t height);

// Works like LMG_put_sym() but skips every position that is not within the
// confines of the LED matrix.
void LMG_put_sym_bnd(uint32_t f[],
                     const bool * symbols,
                     const int8_t row,
                     const int8_t col,
                     const int8_t width,
                     const int8_t height);

void mat_fill_rect(uint32_t f[],
                   const int8_t row_l,
                   const int8_t col_l,
                   const int8_t row_h,
                   const int8_t col_h,
                   const bool bit);

void mat_text_34(uint32_t f[],
                 const bool symbols[][12],
                 int8_t col_h,
                 int8_t col_l,
                 const int8_t row,
                 const uint8_t msg[],
                 const std::size_t msg_len,
                 const uint8_t spacing,
                 const int8_t step);

#endif
