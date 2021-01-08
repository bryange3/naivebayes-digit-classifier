//
// Created by Bryan Ge on 10/10/20.
//

#pragma once

#include <map>
#include <string>
#include <vector>

#include "label_statistics.h"
#include "pixel_position_statistics.h"

namespace naivebayes {

/**
 * Holds statistics for images, including pixel position and label
 * probabilities.
 */
class ImagesStats {
 public:
  ImagesStats() = default;

  /**
   * Gets statistics of a single pixel position.
   *
   * @param label label value associated with the pixel position
   * @param row row the pixel position is located in
   * @param col column the pixel position is located in
   * @return pixel position statistics for the specified label and location
   */
  PixelPositionStatistics& GetPixelPositionStatistic(int label, size_t row,
                                                     size_t col);

  /**
   * Initializes ImagesStats object with specified Laplace smoothing value.
   *
   * @param k Laplace smoothing k value
   */
  // https://www.geeksforgeeks.org/when-do-we-use-initializer-list-in-c/
  explicit ImagesStats(int k) : k_(k) {};

  /**
   * Calculates all probabilities for labels and for pixel positions.
   */
  void CalculateProbabilities();

  /**
   * Determines image pixel position and label probabilities.
   *
   * @param kLabelsFile labels file
   * @param kImagesFile images file
   */
  void ReadFilesAndTrainModel(const std::string& kLabelsFile,
                              const std::string& kImagesFile);

  /**
   * Saves model into specified file path.
   *
   * @param kSavedFile file path for saved model
   */
  void SaveModel(const std::string& kSavedFile);

  /**
   * Loads model given a file path.
   *
   * @param kSavedFile file path for the saved model to be loaded
   */
  void LoadModel(const std::string& kSavedFile);

  int get_k() const;
  void set_k(int k);
  size_t get_image_side_length() const;
  void set_image_side_length(size_t image_side_length);
  const std::vector<int>& get_labels() const;
  void set_labels(const std::vector<int>& labels);
  const std::map<int, std::vector<std::vector<PixelPositionStatistics>>>&
      get_label_to_pixel_positions() const;
  void set_label_to_pixel_positions(
      const std::map<int, std::vector<std::vector<PixelPositionStatistics>>>&
          label_to_pixel_positions);
  const std::map<int, LabelStatistics>& get_label_value_to_label_statistics() const;
  void set_label_value_to_label(
      const std::map<int, LabelStatistics>& label_value_to_label);

 private:
  int k_;
  size_t image_side_length_;
  std::vector<int> labels_;
  std::map<int, std::vector<std::vector<PixelPositionStatistics>>>
      label_to_pixel_positions_;
  std::map<int, LabelStatistics> label_value_to_label_statistics_;

  /**
   * Determines how many characters are in a line for an image, also known
   * as the side length of the image.
   *
   * @param kImagesFile images file name
   */
  void GetImageSideLength(const std::string& kImagesFile);

  /**
   * Creates initial map of labels to their pixel positions.
   *
   * @param kImageSideLength image side length
   */
  void SetLabelToPixelPositions(const size_t& kImageSideLength);

  /**
   * Overloaded operator used for updating pixel position statistics for the
   * label corresponding to the current image.
   *
   * @param input provided input file
   * @param input_pixel_positions pixel positions to be updated
   */
  friend std::istream& operator>>(
      std::istream& input,
      std::vector<std::vector<PixelPositionStatistics>>& input_pixel_positions);

  /**
   * Loads file information into images stats object.
   *
   * @param is input
   * @param images_stats statistics of image to be updated with file's info
   */
  friend std::istream& operator>>(std::istream& is, ImagesStats& images_stats);

  /**
   * Saves file information from images stats into a file.
   *
   * @param os output
   * @param images_stats images stats to be saved
   */
  friend std::ostream& operator<<(std::ostream& os,
                                  const ImagesStats& images_stats);

  /**
   * Reads in all labels from the labels file.
   *
   * @param kLabelsFile labels file name
   */
  void ReadLabelsFile(const std::string& kLabelsFile);

  /**
   * Reads in all images from the images file.
   *
   * @param kImagesFile
   */
  void ReadImagesFile(const std::string& kImagesFile);
};

}  // namespace naivebayes