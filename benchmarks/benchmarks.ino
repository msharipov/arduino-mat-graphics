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

#include <cstdint>
#include <tuple>
#include <array>
#include <numeric>
#include <LED_Matrix_Graphics.h>

LMG::Frame frame {};
constexpr uint8_t TOTAL_LED_COUNT {
  LMG::LED_MATRIX_HEIGHT * LMG::LED_MATRIX_WIDTH
};

constexpr std::array<uint32_t, 3> SET_LED_SAMPLES{1000, 10000, 100000};
constexpr uint32_t SET_LED_RUNS{10};
constexpr std::array<uint32_t, 3> INVERT_LED_SAMPLES{1000, 10000, 100000};
constexpr uint32_t INVERT_LED_RUNS{10};
constexpr std::array<uint32_t, 3> FILL_RECT_SAMPLES{100, 1000, 10000};
constexpr uint32_t FILL_RECT_RUNS{10};

/// Prints the results of a benchmark.
/**
 * @param result A tuple where the first value is the mean length of the
 *               benchmark in microseconds and the second value is the
 *               standard deviation.
 * @param size The number of iterations in a single run.
 * @param runs The number of runs in the benchmark.
 */
void printResults(const std::tuple<double, double> &result,
                  const uint32_t size,
                  const uint32_t runs) {
    Serial.print("sample size: ");
    Serial.print(size);
    Serial.print(", runs: ");
    Serial.print(runs);
    Serial.print(", mean: ");
    Serial.print(std::get<0>(result), 4);
    Serial.print("e-6 s, stdev: ");
    Serial.print(std::get<1>(result), 4);
    Serial.print("e-6 s\n");
}

/// Computes the mean and the standard deviation of an array.
template <std::size_t N>
const std::tuple<double, double> getStats(const std::array<double, N> &arr) {
  const double mean = std::accumulate(arr.begin(), arr.end(), 0.0) / N;
  double variance = 0.0;
  for (auto x : arr) {
    variance += (x - mean) * (x - mean);
  }
  variance /= N;
  double stdev = sqrt(variance);

  return std::make_tuple(mean, stdev);
}

/// Runs a single benchmark for Frame::setLED.
const std::tuple<double, double> timeSetLED(const uint32_t iterations) {
  std::array<double, SET_LED_RUNS> times {};
  for (uint32_t run = 0; run < SET_LED_RUNS; run++) {
    const uint32_t start_time = micros();

    // Time how long it takes to generate the row-col combinations
    volatile uint8_t pos {};
    volatile uint8_t row {};
    volatile uint8_t col {};
    volatile bool consumer {};
    for (uint32_t count = 0; count < iterations; count++) {
      pos = count % TOTAL_LED_COUNT;
      row = pos % LMG::LED_MATRIX_HEIGHT;
      col = pos / LMG::LED_MATRIX_WIDTH;
      consumer = static_cast<bool>(rand() % 2);
    }
    const uint32_t generation_time = micros() - start_time;

    for (uint32_t count = 0; count < iterations; count++) {
      pos = count % TOTAL_LED_COUNT;
      row = pos % LMG::LED_MATRIX_HEIGHT;
      col = pos / LMG::LED_MATRIX_WIDTH;
      frame.setLED(row, col, static_cast<bool>(rand() % 2));
    }
    const uint32_t final_time = micros();

    // Time in microseconds
    const double total = static_cast<double>(
      final_time - 2 * generation_time - start_time);

    // Time per iteration in microseconds
    const double per_iter = total / static_cast<double>(iterations);
    times[run] = per_iter;
  }

  return getStats(times);
}

/// Runs all benchmarks for Frame::setLED and prints the results over serial.
void runBenchmarksSetLED() {
  Serial.print("Running benchmarks for Frame::setLED!\n");
  for (auto sample_size : SET_LED_SAMPLES) {
    const std::tuple<double, double> result = timeSetLED(sample_size);
    printResults(result, sample_size, SET_LED_RUNS);
  }
}

/// Runs a single benchmark for Frame::invertLED.
const std::tuple<double, double> timeInvertLED(const uint32_t iterations) {
  std::array<double, INVERT_LED_RUNS> times {};
  for (uint32_t run = 0; run < INVERT_LED_RUNS; run++) {
    const uint32_t start_time = micros();

    // Time how long it takes to generate the row-col combinations
    volatile uint8_t pos {};
    volatile uint8_t row {};
    volatile uint8_t col {};
    for (uint32_t count = 0; count < iterations; count++) {
      pos = count % TOTAL_LED_COUNT;
      row = pos % LMG::LED_MATRIX_HEIGHT;
      col = pos / LMG::LED_MATRIX_WIDTH;
    }
    const uint32_t generation_time = micros() - start_time;

    for (uint32_t count = 0; count < iterations; count++) {
      pos = count % TOTAL_LED_COUNT;
      row = pos % LMG::LED_MATRIX_HEIGHT;
      col = pos / LMG::LED_MATRIX_WIDTH;
      frame.invertLED(row, col);
    }
    const uint32_t final_time = micros();

    // Time in microseconds
    const double total = static_cast<double>(
      final_time - 2 * generation_time - start_time);

    // Time per iteration in microseconds
    const double per_iter = total / static_cast<double>(iterations);
    times[run] = per_iter;
  }

  return getStats(times);
}

/// Runs all benchmarks for Frame::invertLED and prints the results over serial.
void runBenchmarksInvertLED() {
  Serial.print("Running benchmarks for Frame::invertLED!\n");
  for (auto sample_size : INVERT_LED_SAMPLES) {
    const std::tuple<double, double> result = timeInvertLED(sample_size);
    printResults(result, sample_size, INVERT_LED_RUNS);
  }
}

/// Runs a single benchmark for Frame::fillRect.
const std::tuple<double, double> timeFillRect(const uint32_t iterations) {
  std::array<double, FILL_RECT_RUNS> times {};
  const LMG::Rect area{1, 6, 1, 8};
  for (uint32_t run = 0; run < FILL_RECT_RUNS; run++) {
    const uint32_t start_time = micros();
    volatile bool consumer{0};

    // Time how long it takes to generate the random bits
    for (uint32_t count = 0; count < iterations; count++) {
      consumer = static_cast<bool>(rand() % 2);
    }
    const uint32_t generation_time = micros() - start_time;

    for (uint32_t count = 0; count < iterations; count++) {
      frame.fillRect(area, static_cast<bool>(rand() % 2));
    }
    const uint32_t final_time = micros();

    // Time in microseconds
    const double total = static_cast<double>(
      final_time - 2 * generation_time - start_time);

    // Time per iteration in microseconds
    const double per_iter = total / static_cast<double>(iterations);
    times[run] = per_iter;
  }

  return getStats(times);
}

/// Runs all benchmarks for Frame::fillRect and prints the results over serial.
void runBenchmarksFillRect() {
  Serial.print("Running benchmarks for Frame::fillRect!\n");
  for (auto sample_size : FILL_RECT_SAMPLES) {
    const std::tuple<double, double> result = timeFillRect(sample_size);
    printResults(result, sample_size, FILL_RECT_RUNS);
  }
}

void setup() {
  Serial.begin(9600);
  runBenchmarksSetLED();
  runBenchmarksInvertLED();
  runBenchmarksFillRect();
}

void loop() {}
