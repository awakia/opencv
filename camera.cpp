#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main(int argc, char *argv[]) {
  VideoCapture cap(0);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  if (!cap.isOpened()) return -1;

  namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  for(;;) {
    Mat frame;
    cap >> frame;
    // write image processing here ...
    imshow("Capture", frame);
    int key = waitKey(30);
    if (key == 'c') {
      imwrite("cap.png", frame);
    } else if (key >= 0) {
      break;
    }
  }
  return 0;
}
