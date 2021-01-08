//
// Created by Bryan Ge on 10/17/20.
//

#include "core/prediction_making.h"

#include <fstream>
#include <iosfwd>
#include <string>

namespace naivebayes {

using std::endl;
using std::ifstream;
using std::istream;
using std::map;
using std::string;
using std::vector;

const int PredictionMaking::GetTestImagePixel(
    size_t row, size_t col, const vector<vector<int>>& test_image) {
  return test_image.at(row).at(col);
}

double PredictionMaking::GetPixelPositionProbability(
    const ImagesStats& images_stats, int label_value, size_t row, size_t col,
    bool is_shaded) const {
  const map<int, vector<vector<PixelPositionStatistics>>>&
      label_to_pixel_positions = images_stats.get_label_to_pixel_positions();

  if (is_shaded) {
    return label_to_pixel_positions.at(label_value)
        .at(row)
        .at(col)
        .get_shaded_probability();
  } else {
    return label_to_pixel_positions.at(label_value)
        .at(row)
        .at(col)
        .get_unshaded_probability();
  }
}

void PredictionMaking::SetupLabelToLikelihoodScore() {
  const size_t kNumSingleDigitNumbers = 10;
  for (int i = 0; i < kNumSingleDigitNumbers; i++) {
    label_to_likelihood_score_.insert({i, 0.0});
  }
}

void PredictionMaking::CalculateLikelihoodScore(
    ImagesStats& images_stats, const vector<vector<int>>& test_image) {
  int label_value;
  size_t image_side_length = images_stats.get_image_side_length();

  // calculates a likelihood score for each label
  for (const auto& entry : images_stats.get_label_to_pixel_positions()) {
    label_value = entry.first;
    // https://www.geeksforgeeks.org/log-function-cpp/
    label_to_likelihood_score_.at(label_value) =
        log10(images_stats.get_label_value_to_label_statistics()
                .at(label_value)
                .get_probability());

    for (size_t row = 0; row < image_side_length; row++) {
      for (size_t col = 0; col < image_side_length; col++) {
        if (GetTestImagePixel(row, col, test_image) == kShadedPixel) {
          label_to_likelihood_score_.at(label_value) +=
              log10(GetPixelPositionProbability(images_stats, label_value, row,
                                              col, true));
        } else if (GetTestImagePixel(row, col, test_image) == kUnshadedPixel) {
          label_to_likelihood_score_.at(label_value) +=
              log10(GetPixelPositionProbability(images_stats, label_value, row,
                                              col, false));
        }
      }
    }
  }
}

void PredictionMaking::GetTestImageSideLength(const string& kTestImagesFile) {
  ifstream file(kTestImagesFile);
  string first_line;
  getline(file, first_line);
  file.close();
  test_image_side_length_ = first_line.size();
}

istream& operator>>(istream& input, vector<vector<int>>& test_image) {
  const char kWhitePixel = ' ';
  const char kGrayPixel = '+';
  const char kBlackPixel = '#';

  string first_line;
  // first line is read to determine the image side length
  getline(input, first_line);
  size_t kImageSideLength = first_line.size();

  // separate reading for first line of pixels because already read in above
  for (size_t col = 0; col < kImageSideLength; col++) {
    char pixel = first_line.at(col);
    if (pixel == kWhitePixel) {
      test_image.at(0).at(col) = 0;
    } else if ((pixel == kGrayPixel) || (pixel == kBlackPixel)) {
      test_image.at(0).at(col) = 1;
    }
  }

  // read in all following lines of image
  for (size_t row = 1; row < kImageSideLength; row++) {
    string line;
    getline(input, line);
    for (size_t col = 0; col < kImageSideLength; col++) {
      char pixel = line.at(col);
      if (pixel == kWhitePixel) {
        test_image.at(row).at(col) = 0;
      } else if ((pixel == kGrayPixel) || (pixel == kBlackPixel)) {
        test_image.at(row).at(col) = 1;
      }
    }
  }

  return input;
}

void PredictionMaking::ReadAndClassifyTestImages(
    const string& kTestImagesFile, ImagesStats& images_stats,
    const string& kSaveClassifiedLabelsFile) {
  // http://www.cplusplus.com/doc/tutorial/files/
  GetTestImageSideLength(kTestImagesFile);
  vector<vector<int>> test_image(test_image_side_length_,
                                 vector<int>(test_image_side_length_, 0));
  ifstream file(kTestImagesFile);

  while (!file.eof()) {
    file >> test_image;
    classified_labels_.push_back(ClassifyImage(images_stats, test_image));
  }

  // removes the last element added to classified_labels_ because it's actually
  // a new line (empty string "")
  classified_labels_.erase(classified_labels_.end() - 1);
  file.close();

  SaveClassifiedLabels(kSaveClassifiedLabelsFile);
}

int PredictionMaking::ClassifyImage(ImagesStats& images_stats,
                                    const vector<vector<int>>& test_image) {
  SetupLabelToLikelihoodScore();
  CalculateLikelihoodScore(images_stats, test_image);

  double max_likelihood_score = label_to_likelihood_score_.at(0);
  int label = 0;

  // determine the maximum likelihood score
  for (size_t label_value = 1; label_value < label_to_likelihood_score_.size(); label_value++) {
    if (label_to_likelihood_score_.at(label_value) > max_likelihood_score) {
      max_likelihood_score = label_to_likelihood_score_.at(label_value);
      label = label_value;
    }
  }

  return label;
}

std::ostream& operator<<(std::ostream& os,
                         const PredictionMaking& prediction_making) {
  for (int i : prediction_making.get_classified_labels()) {
    os << i << endl;
  }
  return os;
}

void PredictionMaking::SaveClassifiedLabels(const string& kSavedFile) {
  std::ofstream out;
  out.open(kSavedFile);
  out << *this;
  out.close();
}

const map<int, double>& PredictionMaking::get_label_to_likelihood_score()
    const {
  return label_to_likelihood_score_;
}

const std::unordered_multimap<int, std::vector<std::vector<int>>>&
PredictionMaking::get_classified_label_to_image() const {
  return classified_label_to_image_;
}

const std::vector<int>& PredictionMaking::get_classified_labels() const {
  return classified_labels_;
}

}  // namespace naivebayes