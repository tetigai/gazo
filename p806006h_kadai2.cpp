#include <opencv2/opencv.hpp>
#include <chrono>
#include <iostream>

using namespace cv;
using namespace std;
using namespace std::chrono;

/* 二重ループでネガポジ変換 */
void negative_loop(const Mat& src, Mat& dst)
{
    dst.create(src.size(), src.type());

    int bytes_per_row = src.cols * src.channels();

    for (int y = 0; y < src.rows; y++) {
        const uchar* s = src.ptr<uchar>(y);
        uchar* d = dst.ptr<uchar>(y);

        for (int x = 0; x < bytes_per_row; x++) {
            d[x] = 255 - s[x];
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "使い方: " << argv[0] << " input.png" << endl;
        return 1;
    }

    Mat src = imread(argv[1], IMREAD_UNCHANGED);

    if (src.empty()) {
        cerr << "画像を読み込めませんでした" << endl;
        return 1;
    }

    Mat dst1, dst2;

    /* 二重ループ版の時間計測 */
    auto start1 = steady_clock::now();
    negative_loop(src, dst1);
    auto end1 = steady_clock::now();

    /* bitwise_not版の時間計測 */
    auto start2 = steady_clock::now();
    bitwise_not(src, dst2);
    auto end2 = steady_clock::now();

    double time1 = duration<double, milli>(end1 - start1).count();
    double time2 = duration<double, milli>(end2 - start2).count();

    imwrite("negative_loop.png", dst1);
    imwrite("negative_bitwise.png", dst2);

    cout << "二重ループ版: " << time1 << " ms" << endl;
    cout << "bitwise_not版: " << time2 << " ms" << endl;

    return 0;
}
