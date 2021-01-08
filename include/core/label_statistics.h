//
// Created by Bryan Ge on 10/11/20.
//

#pragma once

#include <iostream>

namespace naivebayes {

/**
 * Holds label statistics.
 */
class LabelStatistics {
 public:
  LabelStatistics() = default;

  /**
   * Creates initial label statistics object given the label value.
   *
   * @param label value of the label
   */
  explicit LabelStatistics(int label) : label_value_(label) {};

  /**
   * Calculates probability of selecting an image of the label.
   *
   * @param k Laplace smoothing k value
   * @param total_num_training_images total number of training images
   */
  void CalculateProbability(int k, int total_num_training_images);

  void set_total_num_images(size_t total_num_images);
  size_t get_total_num_images() const;
  void set_probability(double probability);
  double get_probability() const;
  int get_label() const;
  void set_label(int label);

 private:
  int label_value_;
  double probability_;
  size_t total_num_images_;
};

}  // namespace naivebayes