#include <iostream>
#include <cstdlib>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
  VideoCapture cap(0);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  if (!cap.isOpened()) return -1;

  namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  for(int t = 0;; t++) {
    Mat img, frame;
    cap >> frame;
    // write image processing here ...
    cv::medianBlur(frame, img, t / 2 * 2 + 1);

    imshow("Capture", img);
    int key = waitKey(30);
    if (key == 'c') {
      imwrite("cap.png", img);
    } else if (key >= 0) {
      break;
    }
  }
  return 0;
}
