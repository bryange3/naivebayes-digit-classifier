//
// Created by Bryan Ge on 10/11/20.
//

#include "core/label_statistics.h"

namespace naivebayes {

void LabelStatistics::CalculateProbability(int k, int total_num_training_images) {
  probability_ = (k + static_cast<double>(total_num_images_)) /
                 (10 * k + static_cast<double>(total_num_training_images));
}

size_t LabelStatistics::get_total_num_images() const {
  return total_num_images_;
}

void LabelStatistics::set_total_num_images(size_t total_num_images) {
  total_num_images_ = total_num_images;
}

int LabelStatistics::get_label() const {
  return label_value_;
}

void LabelStatistics::set_label(int label) {
  label_value_ = label;
}

double LabelStatistics::get_probability() const {
  return probability_;
}

void LabelStatistics::set_probability(double probability) {
  probability_ = probability;
}

}  // namespace naivebayes