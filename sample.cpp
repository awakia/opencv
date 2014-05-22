// compile: g++ `pkg-config --cflags --libs opencv` sample.cpp

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;

const double scale = 0.1;

int main(int argc, char *argv[])
{
  cv::Mat src_img = cv::imread("images/faces/IMG_2245.png", 1);
  if (src_img.empty()) {
    cerr << "cannot open such file" << endl;
    return -1;
  }
  cv::Mat dst_img1;
  cv::Mat dst_img2(src_img.rows*scale, src_img.cols*scale, src_img.type());

  // INTER_LINER（バイリニア補間）でのサイズ変更
  cv::resize(src_img, dst_img1, cv::Size(), scale, scale);
  // INTER_CUBIC（バイキュービック補間）でのサイズ変更
  cv::resize(src_img, dst_img2, dst_img2.size(), cv::INTER_CUBIC);

  cv::namedWindow("resize image1", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("resize image2", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::imshow("resize image1", dst_img1);
  cv::imshow("resize image2", dst_img2);
  cv::waitKey(0);
}
