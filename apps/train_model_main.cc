#include <iostream>

#include "core/images_stats.h"
#include "core/prediction_making.h"

using std::cout;
using std::endl;

/**
 * Handles the following commands:
 *  train model + save model
 *  load model + test images
 *  train model
 *  load model
 *  train model + save model + test images
 *  train model + test images + save model
 *  train model + test images
 *
 * train model arguments:
 *  train <training_labels_file> <training_images_file>
 *
 * save model arguments:
 *  save <save_model_file>
 *
 * test images arguments:
 *  test <test_images_file> <save_classified_labels_file>
 */
int main(int argc, char* argv[]) {
  if (strcmp(argv[1], "./train_model") == 0) {
    if (argc == 7) {
      if (strcmp(argv[2], "train") == 0 && strcmp(argv[5], "save") == 0) {
        naivebayes::ImagesStats images_stats = naivebayes::ImagesStats(1);
        images_stats.ReadFilesAndTrainModel(argv[3], argv[4]);
        images_stats.SaveModel(argv[6]);
        cout << "Finished training model and saved to \"" << argv[6] << "\"" << endl;
      } else if (strcmp(argv[2], "load") == 0 && strcmp(argv[4], "test") == 0) {
        naivebayes::ImagesStats images_stats;
        images_stats.LoadModel(argv[3]);
        naivebayes::PredictionMaking prediction_making;
        prediction_making.ReadAndClassifyTestImages(argv[5], images_stats, argv[6]);
        cout << "Finished loading model and testing images; classified labels saved to \"" << argv[6] << "\"" << endl;
      } else {
        cout << "Incorrect arguments" << endl;
      }
    } else if (argc == 4) {
      if (strcmp(argv[2], "train") == 0) {
        naivebayes::ImagesStats images_stats = naivebayes::ImagesStats(1);
        images_stats.ReadFilesAndTrainModel(argv[3], argv[4]);
        cout << "Finished training model" << endl;
      } else if (strcmp(argv[2], "load") == 0) {
        naivebayes::ImagesStats images_stats;
        images_stats.LoadModel(argv[3]);
        cout << "Finished loading model" << endl;
      } else {
        cout << "Incorrect arguments" << endl;
      }
    } else if (argc == 10) {
      if (strcmp(argv[2], "train") == 0 && strcmp(argv[5], "save") == 0 && strcmp(argv[7], "test") == 0) {
        naivebayes::ImagesStats images_stats = naivebayes::ImagesStats(1);
        images_stats.ReadFilesAndTrainModel(argv[3], argv[4]);
        images_stats.SaveModel(argv[6]);
        naivebayes::PredictionMaking prediction_making;
        prediction_making.ReadAndClassifyTestImages(argv[8], images_stats, argv[9]);
        cout << "Finished training model, saving model, and testing images; classified labels saved to \"" << argv[9] << "\"" << endl;
      } else if (strcmp(argv[2], "train") == 0 && strcmp(argv[5], "test") == 0 && strcmp(argv[8], "save") == 0) {
        naivebayes::ImagesStats images_stats = naivebayes::ImagesStats(1);
        images_stats.ReadFilesAndTrainModel(argv[3], argv[4]);
        naivebayes::PredictionMaking prediction_making;
        prediction_making.ReadAndClassifyTestImages(argv[6], images_stats, argv[7]);
        images_stats.SaveModel(argv[9]);
        cout << "Finished training model, testing images, and saving model; classified labels saved to \"" << argv[9] << "\"" << endl;
      } else {
        cout << "Incorrect arguments" << endl;
      }
    } else if (argc == 8) {
      if (strcmp(argv[2], "train") == 0 && strcmp(argv[5], "test") == 0) {
        naivebayes::ImagesStats images_stats = naivebayes::ImagesStats(1);
        images_stats.ReadFilesAndTrainModel(argv[3], argv[4]);
        naivebayes::PredictionMaking prediction_making;
        prediction_making.ReadAndClassifyTestImages(argv[6], images_stats, argv[7]);
        cout << "Finished training model and testing images; classified labels saved to \"" << argv[7] << "\"" << endl;
      }
    } else {
        cout << "Incorrect number of arguments" << endl;
    }
  } else {
    cout << "Incorrect arguments for this program" << endl;
  }
}