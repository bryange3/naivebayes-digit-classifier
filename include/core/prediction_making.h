//
// Created by Bryan Ge on 10/17/20.
//

#pragma once

#include <map>
#include <string>
#include <unordered_map>

#include "core/images_stats.h"
#include "core/pixel_position_statistics.h"

namespace naivebayes {

/**
 * Determines likelihood scores, classifies images, and makes predictions.
 */
class PredictionMaking {
 public:
  /**
   * Calculates likelihood scores for every label from trained model for the
   * current test image.
   *
   * @param images_stats images statistics of trained model
   * @param test_image test image used for calculating likelihood scores
   */
  void CalculateLikelihoodScore(ImagesStats& images_stats, const std::vector<std::vector<int>>& test_image);

  /**
   * Classifies test image based on passed in trained model.
   *
   * @param images_stats images statistics of trained model
   * @param test_image image to be classified
   * @return number/label that the image is being classified as
   */
  int ClassifyImage(ImagesStats& images_stats, const std::vector<std::vector<int>>& test_image);

  /**
   * Reads the testing images file and classifies each image in the file.
   *
   * @param kTestImagesFile
   * @param images_stats
   */
  void ReadAndClassifyTestImages(const std::string& kTestImagesFile,
                                 ImagesStats& images_stats,
                                 const std::string& kSaveClassifiedLabelsFile);

  const std::map<int, double>& get_label_to_likelihood_score() const;
  const std::unordered_multimap<int, std::vector<std::vector<int>>>&
      get_classified_label_to_image() const;
  const std::vector<int>& get_classified_labels() const;

 private:
  const int kUnshadedPixel = 0;
  const int kShadedPixel = 1;
  std::map<int, double> label_to_likelihood_score_;
  std::unordered_multimap<int, std::vector<std::vector<int>>> classified_label_to_image_;
  size_t test_image_side_length_;
  std::vector<std::vector<int>> test_image_;
  std::vector<int> classified_labels_;

  /**
   * Overloaded operator used to read in image from a file and populate a test
   * image.
   *
   * @param input input file to be read
   * @param test_image test image to be populated
   * @return input
   */
  friend std::istream& operator>>(std::istream& input,
                                  std::vector<std::vector<int>>& test_image);

  /**
   * Overloaded operator used to output all classified labels into a file
   * for saving.
   *
   * @param os file for classified labels to be saved into
   * @param prediction_making prediction making information to be saved
   * @return os
   */
  friend std::ostream& operator<<(std::ostream& os, const PredictionMaking& prediction_making);

  /**
   * Gets information on the pixel at the specified row and column. The pixel of
   * the test image is either unshaded (0) or shaded (1).
   *
   * @param row row of the pixel
   * @param col column of the pixel
   * @param test_image image to get pixel from
   * @return number determining if the pixel is shaded or unshaded
   */
  const int GetTestImagePixel(size_t row, size_t col, const std::vector<std::vector<int>>& test_image);

  /**
   * Gets the shaded or unshaded pixel position probability.
   *
   * @param row row of the pixel position
   * @param col column of the pixel position
   * @param is_shaded true if getting shaded probability and false if getting
   * unshaded probability
   * @return pixel position probability
   */
  double GetPixelPositionProbability(const ImagesStats& images_stats,
                                     int label_value, size_t row, size_t col,
                                     bool is_shaded) const;

  /**
   * Sets up initial label to likelihood score map.
   */
  void SetupLabelToLikelihoodScore();

  /**
   * Gets test image side length for the test images file.
   *
   * @param kTestImagesFile
   */
  void GetTestImageSideLength(const std::string& kTestImagesFile);

  /**
   * Saves all classified labels into a file.
   *
   * @param kSavedFile
   */
  void SaveClassifiedLabels(const std::string& kSavedFile);
};

}  // namespace naivebayes
