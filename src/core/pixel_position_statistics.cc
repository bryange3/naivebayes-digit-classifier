//
// Created by Bryan Ge on 10/10/20.
//

#include "core/pixel_position_statistics.h"

namespace naivebayes {

void PixelPositionStatistics::IncrementShadedCount() {
  shaded_count_++;
}

void PixelPositionStatistics::IncrementUnshadedCount() {
  unshaded_count_++;
}

void PixelPositionStatistics::CalculateProbabilities(
    int k, int total_num_images) {
  shaded_probability_ =
      (k + static_cast<double>(shaded_count_)) / (2 * k + total_num_images);
  unshaded_probability_ =
      (k + static_cast<double>(unshaded_count_)) / (2 * k + total_num_images);
}

size_t PixelPositionStatistics::CalculateTotalCount() {
  return shaded_count_ + unshaded_count_;
}

size_t PixelPositionStatistics::get_shaded_count() const {
  return shaded_count_;
}

size_t PixelPositionStatistics::get_unshaded_count() const {
  return unshaded_count_;
}

double PixelPositionStatistics::get_shaded_probability() const {
  return shaded_probability_;
}

double PixelPositionStatistics::get_unshaded_probability() const {
  return unshaded_probability_;
}

void PixelPositionStatistics::set_shaded_count(size_t shaded_count) {
  shaded_count_ = shaded_count;
}

void PixelPositionStatistics::set_unshaded_count(size_t unshaded_count) {
  unshaded_count_ = unshaded_count;
}

void PixelPositionStatistics::set_shaded_probability(
    double shaded_probability) {
  shaded_probability_ = shaded_probability;
}

void PixelPositionStatistics::set_unshaded_probability(
    double unshaded_probability) {
  unshaded_probability_ = unshaded_probability;
}

}  // namespace naivebayes
