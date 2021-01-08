//
// Created by Bryan Ge on 10/10/20.
//

#include "core/images_stats.h"

#include <fstream>
#include <map>
#include <vector>

#include "core/pixel_position_statistics.h"

namespace naivebayes {

using std::ifstream;
using std::istream;
using std::map;
using std::ofstream;
using std::string;
using std::vector;

PixelPositionStatistics& ImagesStats::GetPixelPositionStatistic(int label,
                                                                size_t row,
                                                                size_t col) {
  return label_to_pixel_positions_.at(label).at(row).at(col);
}

void ImagesStats::ReadLabelsFile(const string& kLabelsFile) {
  ifstream file(kLabelsFile);
  string label;

  while (!file.eof()) {
    getline(file, label);
    if (label == "") {
      break;
    }
    labels_.push_back(std::stoi(label));
  }
  file.close();
}

void ImagesStats::GetImageSideLength(const string& kImagesFile) {
  ifstream file(kImagesFile);
  string first_line;
  getline(file, first_line);
  file.close();
  image_side_length_ = first_line.size();
}

void ImagesStats::SetLabelToPixelPositions(const size_t& kImageSideLength) {
  const size_t kNumSingleDigitNumbers = 10;
  const PixelPositionStatistics kInitialPixelPosition;

  // https://stackoverflow.com/questions/13121469/initializing-a-vector-of-vectors-having-a-fixed-size-with-boost-assign
  const vector<vector<PixelPositionStatistics>> kInitialPixelPositions(
      kImageSideLength,
      vector<PixelPositionStatistics>(kImageSideLength, kInitialPixelPosition));

  for (size_t i = 0; i < kNumSingleDigitNumbers; i++) {
    label_to_pixel_positions_.insert({i, kInitialPixelPositions});
    label_value_to_label_statistics_.insert({i, LabelStatistics(i)});
  }
}

istream& operator>>(
    istream& input,
    vector<vector<PixelPositionStatistics>>& input_pixel_positions) {
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
      input_pixel_positions.at(0).at(col).IncrementUnshadedCount();
    } else if ((pixel == kGrayPixel) || (pixel == kBlackPixel)) {
      input_pixel_positions.at(0).at(col).IncrementShadedCount();
    }
  }

  // read in all following lines of image
  for (size_t row = 1; row < kImageSideLength; row++) {
    string line;
    getline(input, line);
    for (size_t col = 0; col < kImageSideLength; col++) {
      char pixel = line.at(col);
      if (pixel == kWhitePixel) {
        input_pixel_positions.at(row).at(col).IncrementUnshadedCount();
      } else if ((pixel == kGrayPixel) || (pixel == kBlackPixel)) {
        input_pixel_positions.at(row).at(col).IncrementShadedCount();
      }
    }
  }

  return input;
}

// https://stackoverflow.com/questions/2376193/how-to-write-an-object-to-file-in-c
istream& operator>>(std::istream& is, ImagesStats& images_stats) {
  int k;
  is >> k;
  images_stats.set_k(k);

  size_t image_side_length;
  is >> image_side_length;
  images_stats.set_image_side_length(image_side_length);

  size_t labels_size;
  is >> labels_size;
  vector<int> labels;
  for (size_t i = 0; i < labels_size; i++) {
    int label;
    is >> label;
    labels.push_back(label);
  }
  images_stats.set_labels(labels);

  size_t label_to_pixel_positions_size;
  is >> label_to_pixel_positions_size;
  map<int, vector<vector<PixelPositionStatistics>>> label_to_pixel_positions;
  for (size_t entry = 0; entry < label_to_pixel_positions_size; entry++) {
    int label_value;
    is >> label_value;

    size_t pixel_positions_size;
    is >> pixel_positions_size;
    size_t pixel_position_row_size;
    is >> pixel_position_row_size;

    // https://www.geeksforgeeks.org/2d-vector-in-cpp-with-user-defined-size/
    vector<vector<PixelPositionStatistics>> pixel_positions(
        pixel_positions_size,
        vector<PixelPositionStatistics>(pixel_position_row_size));

    // loads pixel position statistics at each position of the image
    for (vector<PixelPositionStatistics>& pixel_position_row :
         pixel_positions) {
      for (PixelPositionStatistics& current_pixel_position :
           pixel_position_row) {
        size_t shaded_count;
        is >> shaded_count;
        current_pixel_position.set_shaded_count(shaded_count);

        size_t unshaded_count;
        is >> unshaded_count;
        current_pixel_position.set_unshaded_count(unshaded_count);

        double shaded_probability;
        is >> shaded_probability;
        current_pixel_position.set_shaded_probability(shaded_probability);

        double unshaded_probability;
        is >> unshaded_probability;
        current_pixel_position.set_unshaded_probability(unshaded_probability);
      }
    }
    label_to_pixel_positions.insert({label_value, pixel_positions});
  }
  images_stats.set_label_to_pixel_positions(label_to_pixel_positions);

  size_t label_value_to_label_size;
  is >> label_value_to_label_size;
  map<int, LabelStatistics> label_value_to_label;
  for (size_t entry = 0; entry < label_value_to_label_size; entry++) {
    int label_value;
    is >> label_value;

    LabelStatistics label;

    int value_of_label;
    is >> value_of_label;
    label.set_label(value_of_label);

    size_t total_num_images;
    is >> total_num_images;
    label.set_total_num_images(total_num_images);

    double probability;
    is >> probability;
    label.set_probability(probability);

    label_value_to_label.insert({label_value, label});
  }
  images_stats.set_label_value_to_label(label_value_to_label);

  return is;
}

std::ostream& operator<<(std::ostream& os, const ImagesStats& images_stats) {
  os << images_stats.get_k() << " ";
  os << images_stats.get_image_side_length() << " ";

  const vector<int>& labels = images_stats.get_labels();
  os << labels.size() << " ";
  for (int label : labels) {
    os << label << " ";
  }

  const map<int, vector<vector<PixelPositionStatistics>>>&
      label_to_pixel_positions = images_stats.get_label_to_pixel_positions();
  os << label_to_pixel_positions.size() << " ";
  for (const auto& entry : label_to_pixel_positions) {
    os << entry.first << " ";

    const vector<vector<PixelPositionStatistics>>& pixel_positions =
        entry.second;
    os << pixel_positions.size() << " ";
    os << pixel_positions.at(0).size() << " ";
    for (const auto& pixel_position_row : pixel_positions) {
      for (const auto& current_pixel_position : pixel_position_row) {
        os << current_pixel_position.get_shaded_count() << " ";
        os << current_pixel_position.get_unshaded_count() << " ";
        os << current_pixel_position.get_shaded_probability() << " ";
        os << current_pixel_position.get_unshaded_probability() << " ";
      }
    }
  }

  const map<int, LabelStatistics>& label_value_to_label =
      images_stats.get_label_value_to_label_statistics();
  os << label_value_to_label.size() << " ";
  for (const auto& entry : label_value_to_label) {
    os << entry.first << " ";
    LabelStatistics label = entry.second;
    os << label.get_label() << " ";
    os << label.get_total_num_images() << " ";
    os << label.get_probability() << " ";
  }

  return os;
}

void ImagesStats::ReadImagesFile(const string& kImagesFile) {
  // http://www.cplusplus.com/doc/tutorial/files/
  GetImageSideLength(kImagesFile);
  SetLabelToPixelPositions(image_side_length_);

  ifstream file(kImagesFile);
  for (size_t current_image_count = 0; current_image_count < labels_.size() - 1;
       current_image_count++) {
    int label = labels_.at(current_image_count);
    file >> label_to_pixel_positions_.at(label);
  }
  file.close();
}

void ImagesStats::CalculateProbabilities() {
  int label_value;
  size_t total_num_images;

  // https://www.geeksforgeeks.org/traversing-a-map-or-unordered_map-in-cpp-stl/
  for (auto& entry : label_to_pixel_positions_) {
    label_value = entry.first;
    total_num_images =
        GetPixelPositionStatistic(label_value, 0, 0).CalculateTotalCount();
    for (size_t row = 0; row < image_side_length_; row++) {
      for (size_t col = 0; col < image_side_length_; col++) {
        GetPixelPositionStatistic(label_value, row, col)
            .CalculateProbabilities(k_, total_num_images);
      }
    }
    label_value_to_label_statistics_.at(label_value)
        .set_total_num_images(total_num_images);
    label_value_to_label_statistics_.at(label_value)
        .CalculateProbability(k_, labels_.size());
  }
}

void ImagesStats::SaveModel(const string& kSavedFile) {
  std::ofstream out;
  out.open(kSavedFile);
  out << *this;
  out.close();
}

void ImagesStats::LoadModel(const string& kSavedFile) {
  std::ifstream in(kSavedFile);
  in >> *this;
  in.close();
}

void ImagesStats::ReadFilesAndTrainModel(const string& kLabelsFile,
                                         const string& kImagesFile) {
  ReadLabelsFile(kLabelsFile);
  ReadImagesFile(kImagesFile);
  CalculateProbabilities();
}

int ImagesStats::get_k() const {
  return k_;
}

const vector<int>& ImagesStats::get_labels() const {
  return labels_;
}

const std::map<int, std::vector<std::vector<PixelPositionStatistics>>>&
ImagesStats::get_label_to_pixel_positions() const {
  return label_to_pixel_positions_;
}

const std::map<int, LabelStatistics>& ImagesStats::get_label_value_to_label_statistics()
    const {
  return label_value_to_label_statistics_;
}

void ImagesStats::set_k(int k) {
  k_ = k;
}

void ImagesStats::set_labels(const vector<int>& labels) {
  labels_ = labels;
}

void ImagesStats::set_label_to_pixel_positions(
    const map<int, std::vector<std::vector<PixelPositionStatistics>>>&
        label_to_pixel_positions) {
  label_to_pixel_positions_ = label_to_pixel_positions;
}

void ImagesStats::set_label_value_to_label(
    const map<int, LabelStatistics>& label_value_to_label) {
  label_value_to_label_statistics_ = label_value_to_label;
}

size_t ImagesStats::get_image_side_length() const {
  return image_side_length_;
}

void ImagesStats::set_image_side_length(size_t image_side_length) {
  image_side_length_ = image_side_length;
}

}  // namespace naivebayes