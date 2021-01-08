//
// Created by Bryan Ge on 10/10/20.
//

#pragma once

#include <iostream>

namespace naivebayes {

/**
 * Holds pixel position statistics.
 */
class PixelPositionStatistics {
 public:
  /**
   * Creates initial pixel position statistics and sets shaded count and
   * unshaded count to the starting value of 0.
   *
   * @param starting_value value that the shaded and unshaded counts start at
   */
  explicit PixelPositionStatistics(int starting_value = 0)
      : shaded_count_(starting_value), unshaded_count_(starting_value) {
  }

  /**
   * Increments the pixel position's shaded count.
   */
  void IncrementShadedCount();

  /**
   * Increments the pixel position's unshaded count.
   */
  void IncrementUnshadedCount();

  /**
   * Calculates the probabilities of the pixel position being shaded and
   * unshaded given a k value and the total number of images belonging to the
   * label.
   *
   * @param k Laplace smoothing k value
   * @param total_num_images total number of images belonging to the label
   */
  void CalculateProbabilities(int k, int total_num_images);

  /**
   * Calculates the sum of shaded and unshaded counts.
   *
   * @return total count
   */
  size_t CalculateTotalCount();

  size_t get_shaded_count() const;
  size_t get_unshaded_count() const;
  double get_shaded_probability() const;
  double get_unshaded_probability() const;
  void set_shaded_count(size_t shaded_count);
  void set_unshaded_count(size_t unshaded_count);
  void set_shaded_probability(double shaded_probability);
  void set_unshaded_probability(double unshaded_probability);

 private:
  size_t shaded_count_;
  size_t unshaded_count_;
  double shaded_probability_;
  double unshaded_probability_;
};

}  // namespace naivebayes