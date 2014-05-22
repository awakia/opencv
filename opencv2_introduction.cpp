// from: http://opencv.jp/opencv-2svn/cpp/introduction.html

//
// 輝度チャンネルにノイズを加え，彩度チャンネルの輝度値を減らすことで，
// 簡単なレトロ写真風の効果を与えます．
//

// 以前と同様に，標準の OpenCV ヘッダをインクルードします．
#include "cv.h"
#include "highgui.h"

// 新しいAPIは，"cv" という名前空間内に存在します．
using namespace cv;

// 以下のコードで，API の混合利用の有効と無効を切り替えます．
#define DEMO_MIXED_API_USE 1

int main( int argc, char** argv )
{
    const char* imagename = argc > 1 ? argv[1] : "lena.jpg";
#if DEMO_MIXED_API_USE
    // Ptr<T> は，参照カウントを行う安全なポインタクラスです．
    Ptr<IplImage> iplimg = cvLoadImage(imagename);

    // cv::Mat は CvMat と IplImage を置き換えますが，
    // 新旧のデータ構造間の変換は簡単です．
    // （デフォルトでは，ヘッダのみが変換されデータは共有されます）
    Mat img(iplimg);
#else
    // cvLoadImage に代わる新しい関数は MATLAB 形式の名前です．
    Mat img = imread(imagename);
#endif

    if( !img.data ) // 画像が適切に読み込まれたかどうかをチェックします．
        return -1;

    Mat img_yuv;
    // 画像を，YUV 色空間に変換します．
    // 出力画像のメモリは，自動的に確保されます．
    cvtColor(img, img_yuv, CV_BGR2YCrCb);

    // 画像を色平面毎に分割します．
    vector<Mat> planes;
    split(img_yuv, planes);

    // 別の形式の Mat コンストラクタです．
    // 指定されたサイズ，型の行列を確保します．
    Mat noise(img.size(), CV_8U);

    // 行列を正規分布する乱数値で埋めます．
    // 一様分布の乱数を使う場合は， randu() ．
    // CvScalar は Scalar に，cvScalarAll() は Scalar::all() に置き換わります．
    randn(noise, Scalar::all(128), Scalar::all(20));

    // ノイズを少し平滑化します．
    // カーネルサイズは 3x3 ，シグマは共に 0.5 に設定します．
    GaussianBlur(noise, noise, Size(3, 3), 0.5, 0.5);

    const double brightness_gain = 0;
    const double contrast_gain = 1.7;
#if DEMO_MIXED_API_USE
    // IplImage または CvMat に対してのみ動作する関数でも，
    // そこに新しい形式の行列を渡すのは簡単です．
    // ステップ１） - ヘッダを変換，データはコピーされません．
    IplImage cv_planes_0 = planes[0], cv_noise = noise;
    // ステップ２） - 関数の呼び出し．ポインタを渡すので，単項演算子 "&" を忘れないように．
    cvAddWeighted(&cv_planes_0, contrast_gain, &cv_noise, 1,
                 -128 + brightness_gain, &cv_planes_0);
#else
    addWeighted(planes[0], constrast_gain, noise, 1,
                -128 + brightness_gain, planes[0]);
#endif
    const double color_scale = 0.5;
    // cvConvertScale は， Mat::convertTo() に置き換わります．
    // 出力行列の型（ここではそのまま，つまり planes[1].type() を渡します）
    // を明示的に指定する必要があります．
    planes[1].convertTo(planes[1], planes[1].type(),
                        color_scale, 128*(1-color_scale));

    // コンパイル時にデータ型が分かっている場合（ここでは， "uchar" ）に
    // 利用できる， convertTo の別の形式です．
    // この表記法は，テンポラリな配列を作成せず，上述の表記法とほぼ同じ速度です．
    planes[2] = Mat_<uchar>(planes[2]*color_scale + 128*(1-color_scale));

    // cvMul() は， Mat::mul に置き換わります．このシンプルな表記法でも，
    // テンポラリな配列は作成されません．
    planes[0] = planes[0].mul(planes[0], 1./255);

    // 結果をマージして元に戻します．
    merge(planes, img_yuv);
    // そして，出力 RGB 画像を作成します．
    cvtColor(img_yuv, img, CV_YCrCb2BGR);

    // これは cvNamedWindow に相当します．
    namedWindow("image with grain", CV_WINDOW_AUTOSIZE);
#if DEMO_MIXED_API_USE
    // これは img と iplimg が本当にデータを共有していることを示します．
    // これまでの処理結果は img に格納されており，したがって iplimg 内にも存在します．
    cvShowImage("image with grain", iplimg);
#else
    imshow("image with grain", img);
#endif
    waitKey();

    return 0;
    // vector<>, Mat, Ptr<> のデストラクタにより，
    // すべてのメモリは自動的に解放されます．
}
