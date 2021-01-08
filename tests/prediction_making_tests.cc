#include <core/images_stats.h>
#include <core/prediction_making.h>

#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <string>

using naivebayes::ImagesStats;
using naivebayes::PredictionMaking;
using namespace Catch::literals;

const int kK = 1;

const std::string kTrainingLabels =
    "../../../../../../data/mnistdatatraining/traininglabels";
const std::string kTrainingImages =
    "../../../../../../data/mnistdatatraining/trainingimages";
const std::string kValidationTestImages =
    "../../../../../../data/mnistdatavalidation/testimages";
const std::string kValidationTestLabels =
    "../../../../../../data/mnistdatavalidation/testlabels";
const std::string kSaveClassifiedLabels =
    "../../../../../../data/classified_labels";

const std::string kTestTrainingImages =
    "../../../../../../data/test_training_images_side_length_28";
const std::string kTestTrainingLabels =
    "../../../../../../data/test_training_labels_side_length_28";
const std::string kTestTestImages = "../../../../../../data/test_test_images";
const std::string kTestSaveClassifiedLabels =
    "../../../../../../data/test_classified_labels";

TEST_CASE("Model accuracy in making predictions") {
  ImagesStats images_stats = ImagesStats(kK);
  images_stats.ReadFilesAndTrainModel(kTrainingLabels, kTrainingImages);

  PredictionMaking prediction_making;
  prediction_making.ReadAndClassifyTestImages(
      kValidationTestImages, images_stats, kSaveClassifiedLabels);

  std::ifstream file(kValidationTestLabels);
  std::string label;
  std::vector<int> test_labels;

  while (!file.eof()) {
    getline(file, label);
    if (label == "") {
      break;
    }
    test_labels.push_back(std::stoi(label));
  }
  file.close();

  // compare classified labels with correct test labels associated with test
  // images
  size_t correct_predictions = 0;
  for (size_t i = 0; i < test_labels.size(); i++) {
    if (test_labels.at(i) == prediction_making.get_classified_labels().at(i)) {
      correct_predictions++;
    }
  }

  REQUIRE(double(correct_predictions) / test_labels.size() > 0.7);
}

TEST_CASE("Accuracy of likelihood scores") {
  ImagesStats images_stats = ImagesStats(kK);
  images_stats.ReadFilesAndTrainModel(kTestTrainingLabels, kTestTrainingImages);
  PredictionMaking prediction_making;
  prediction_making.ReadAndClassifyTestImages(kTestTestImages, images_stats,
                                              kTestSaveClassifiedLabels);
  SECTION("Likelihood score for label 0") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(0) == Approx(-202.07).margin(0.01));
  }
  SECTION("Likelihood score for label 1") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(1) == Approx(-133.47).margin(0.01));
  }
  SECTION("Likelihood score for label 2") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(2) == Approx(-171.98).margin(0.01));
  }
  SECTION("Likelihood score for label 3") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(3) == Approx(-138.07).margin(0.01));
  }
  SECTION("Likelihood score for label 4") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(4) == Approx(-160.27).margin(0.01));
  }
  SECTION("Likelihood score for label 5") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(5) == Approx(-160.90).margin(0.01));
  }
  SECTION("Likelihood score for label 6") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(6) == Approx(-170.82).margin(0.01));
  }
  SECTION("Likelihood score for label 7") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(7) == Approx(-196.47).margin(0.01));
  }
  SECTION("Likelihood score for label 8") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(8) == Approx(-191.05).margin(0.01));
  }
  SECTION("Likelihood score for label 9") {
    REQUIRE(prediction_making.get_label_to_likelihood_score().at(9) == Approx(-165.17).margin(0.01));
  }
}