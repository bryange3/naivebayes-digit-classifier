#include <core/images_stats.h>

#include <catch2/catch.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

using naivebayes::ImagesStats;
using namespace Catch::literals;

const int kK = 1;

const std::string kLabelsFileSideLength28 =
    "../../../../../../data/test_training_labels_side_length_28";
const std::string kImagesFileSideLength28 =
    "../../../../../../data/test_training_images_side_length_28";
const std::string kSaveFileSideLength28 =
    "../../../../../../data/saved_model_side_length_28";
const std::string kSaveFileSideLength28New =
    "../../../../../../data/saved_model_side_length_28_new";

const std::string kLabelsFileSideLength10 =
    "../../../../../../data/test_training_labels_side_length_10";
const std::string kImagesFileSideLength10 =
    "../../../../../../data/test_training_images_side_length_10.txt";
const std::string kSaveFileSideLength10 =
    "../../../../../../data/saved_model_side_length_10";

std::string file_to_string(std::string file_name) {
  std::ifstream file(file_name);
  std::string substring;
  std::string file_as_string = "";
  while(!file.eof()) {
    getline(file, substring);
    file_as_string.append(substring);
  }
  file.close();
  return file_as_string;
}

TEST_CASE("Statistics for one image size") {
  ImagesStats images_stats = ImagesStats(kK);
  images_stats.ReadFilesAndTrainModel(kLabelsFileSideLength28,
                                      kImagesFileSideLength28);
  SECTION("Pixel position statistics") {
    SECTION("Statistics of always unshaded pixel position") {
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(2)
                  .at(0)
                  .at(0)
                  .get_shaded_probability() == Approx(0.25));
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(2)
                  .at(0)
                  .at(0)
                  .get_unshaded_probability() == Approx(0.75));
    }

    SECTION("Statistics of always shaded pixel position") {
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(3)
                  .at(8)
                  .at(17)
                  .get_shaded_probability() == Approx(0.8));
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(3)
                  .at(8)
                  .at(17)
                  .get_unshaded_probability() == Approx(0.2));
    }

    SECTION(
        "Statistics of sometimes shaded sometimes unshaded pixel position") {
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(5)
                  .at(8)
                  .at(10)
                  .get_shaded_probability() == Approx(0.5));
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(5)
                  .at(8)
                  .at(10)
                  .get_unshaded_probability() == Approx(0.5));
    }
  }

  SECTION("Label statistics") {
    REQUIRE(images_stats.get_label_value_to_label_statistics().at(1).get_probability() ==
            Approx(0.151515));
  }
}

TEST_CASE("Statistics for different image size") {
  ImagesStats images_stats = ImagesStats(kK);
  images_stats.ReadFilesAndTrainModel(kLabelsFileSideLength10,
                                      kImagesFileSideLength10);
  SECTION("Pixel position statistics") {
    SECTION("Statistics of always unshaded pixel position") {
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(2)
                  .at(0)
                  .at(0)
                  .get_shaded_probability() == Approx(0.25));
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(2)
                  .at(0)
                  .at(0)
                  .get_unshaded_probability() == Approx(0.75));
    }

    SECTION("Statistics of always shaded pixel position") {
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(2)
                  .at(1)
                  .at(2)
                  .get_shaded_probability() == Approx(0.75));
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(2)
                  .at(1)
                  .at(2)
                  .get_unshaded_probability() == Approx(0.25));
    }

    SECTION(
        "Statistics of sometimes shaded sometimes unshaded pixel position") {
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(2)
                  .at(0)
                  .at(2)
                  .get_shaded_probability() == Approx(0.5));
      REQUIRE(images_stats.get_label_to_pixel_positions()
                  .at(2)
                  .at(0)
                  .at(2)
                  .get_unshaded_probability() == Approx(0.5));
    }
  }

  SECTION("Label statistics") {
    REQUIRE(images_stats.get_label_value_to_label_statistics().at(1).get_probability() ==
            Approx(0.095238));
  }
}

TEST_CASE("Saving and loading") {
  ImagesStats images_stats = ImagesStats(kK);
  images_stats.ReadFilesAndTrainModel(kLabelsFileSideLength28,
                                      kImagesFileSideLength28);
  SECTION("Saved and loaded is equal") {
    images_stats.SaveModel(kSaveFileSideLength28);

    ImagesStats new_images_stats;
    new_images_stats.LoadModel(kSaveFileSideLength28);
    new_images_stats.SaveModel(kSaveFileSideLength28New);

    std::string save_file_28 = file_to_string(kSaveFileSideLength28);
    std::string save_file_28_new = file_to_string(kSaveFileSideLength28New);
    REQUIRE(strcmp(save_file_28.c_str(), save_file_28_new.c_str()) == 0);
  }
}