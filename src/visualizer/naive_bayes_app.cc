#include <visualizer/naive_bayes_app.h>
#include "core/images_stats.h"
#include "core/prediction_making.h"

namespace naivebayes {

namespace visualizer {

const int kK = 1;
const std::string kTrainingLabels =
    "../../../../../../data/mnistdatatraining/traininglabels";
const std::string kTrainingImages =
    "../../../../../../data/mnistdatatraining/trainingimages";

NaiveBayesApp::NaiveBayesApp()
    : sketchpad_(glm::vec2(kMargin, kMargin), kImageDimension,
                 kWindowSize - 2 * kMargin) {
  ci::app::setWindowSize((int) kWindowSize, (int) kWindowSize);
}

void NaiveBayesApp::draw() {
  ci::Color8u background_color(255, 246, 148);  // light yellow
  ci::gl::clear(background_color);

  sketchpad_.Draw();

  ci::gl::drawStringCentered(
      "Press Delete to clear the sketchpad. Press Enter to make a prediction.",
      glm::vec2(kWindowSize / 2, kMargin / 2), ci::Color("black"));

  ci::gl::drawStringCentered(
      "Prediction: " + std::to_string(current_prediction_),
      glm::vec2(kWindowSize / 2, kWindowSize - kMargin / 2), ci::Color("blue"));
}

void NaiveBayesApp::mouseDown(ci::app::MouseEvent event) {
  sketchpad_.HandleBrush(event.getPos());
}

void NaiveBayesApp::mouseDrag(ci::app::MouseEvent event) {
  sketchpad_.HandleBrush(event.getPos());
}

void NaiveBayesApp::keyDown(ci::app::KeyEvent event) {
  ImagesStats images_stats = ImagesStats(kK);
  PredictionMaking prediction_making;
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_RETURN:
      // classify the image that's currently drawn on the sketchpad and
      // update current_prediction_
      images_stats.ReadFilesAndTrainModel(kTrainingLabels, kTrainingImages);
      current_prediction_ = prediction_making.ClassifyImage(images_stats, sketchpad_.get_pixels());
      break;

    case ci::app::KeyEvent::KEY_DELETE:
      sketchpad_.Clear();
      break;
  }
}

}  // namespace visualizer

}  // namespace naivebayes