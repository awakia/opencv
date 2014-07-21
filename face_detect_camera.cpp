#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
  const double scale = 2.0;
  // 分類器の読み込み
  std::string cascadeName = "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";
  cv::CascadeClassifier cascade;
  if(!cascade.load(cascadeName)) {
    cerr << "cannot load face detector" << endl;
    return -1;
  }

  //std::string nested_cascadeName = "/usr/local/share/OpenCV/haarcascades/haarcascade_eye.xml";
  std::string nested_cascadeName = "/usr/local/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
  cv::CascadeClassifier nested_cascade;
  if(!nested_cascade.load(nested_cascadeName)) {
    cerr << "cannot load eye detector" << endl;
    return -1;
  }

  VideoCapture cap(0);
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  if (!cap.isOpened()) {
    cerr << "cannot use camera" << endl;
    return -1;
  }

  namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  for(;;) {
    Mat img;
    cap >> img;

    cv::Mat gray, smallImg(cv::saturate_cast<int>(img.rows/scale), cv::saturate_cast<int>(img.cols/scale), CV_8UC1);
    // グレースケール画像に変換
    cv::cvtColor(img, gray, CV_BGR2GRAY);
    // 処理時間短縮のために画像を縮小
    cv::resize(gray, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
    cv::equalizeHist(smallImg, smallImg);

    std::vector<cv::Rect> faces;
    /// マルチスケール（顔）探索
    // 画像，出力矩形，縮小スケール，最低矩形数，（フラグ），最小矩形
    cascade.detectMultiScale(smallImg, faces,
                             1.1, 2,
                             CV_HAAR_SCALE_IMAGE,
                             cv::Size(30, 30) );

    for(std::vector<cv::Rect>::const_iterator r = faces.begin(); r != faces.end(); ++r) {

      // 検出結果（顔）の描画
      cv::Point face_center;
      int face_radius;
      face_center.x = cv::saturate_cast<int>((r->x + r->width*0.5)*scale);
      face_center.y = cv::saturate_cast<int>((r->y + r->height*0.5)*scale);
      face_radius = cv::saturate_cast<int>((r->width + r->height)*0.25*scale);
      // cout << face_center << " " << face_radius << endl;
      cv::circle(img, face_center, face_radius, cv::Scalar(80,80,255), 3, 8, 0 );


      cv:: Mat smallImgROI = smallImg(*r);
      std::vector<cv::Rect> nestedObjects;
      /// マルチスケール（目）探索
      // 画像，出力矩形，縮小スケール，最低矩形数，（フラグ），最小矩形
      nested_cascade.detectMultiScale(smallImgROI, nestedObjects,
                                      1.1, 3,
                                      CV_HAAR_SCALE_IMAGE,
                                      cv::Size(10,10));


      // 検出結果（目）の描画
      std::vector<cv::Rect>::const_iterator nr = nestedObjects.begin();
      for(; nr != nestedObjects.end(); ++nr) {
        cv::Point center;
        int radius;
        center.x = cv::saturate_cast<int>((r->x + nr->x + nr->width*0.5)*scale);
        center.y = cv::saturate_cast<int>((r->y + nr->y + nr->height*0.5)*scale);
        radius = cv::saturate_cast<int>((nr->width + nr->height)*0.25*scale);
        cv::circle( img, center, radius, cv::Scalar(80,255,80), 3, 8, 0 );
      }
    }

    imshow("Capture", img);

    int key = waitKey(30);
    if (key == 'c') {
      imwrite("face_detect.png", img);
    } else if (key >= 0) {
      break;
    }
  }
  return 0;
}
