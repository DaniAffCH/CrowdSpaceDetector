#include <opencv2/opencv.hpp>
#include "segmentation.h"
#include "spotLocator.h"

int main(int argc, char *argv[])
{

    const cv::String rgbPath = "../img/frame_rgb_000000.jpg";
    const cv::String dPath = "../img/frame_d_000000.jpg";

    float gaussianBlur = 1.5;
    float kValue = 8000.f;
    int minimumComponentSize = 300;

    cv::Mat rgbImage = cv::imread(rgbPath);
    cv::Mat dImage = cv::imread(dPath, cv::IMREAD_ANYDEPTH);

    std::vector<node_pointer> i = segmentImage(rgbImage, dImage, gaussianBlur, kValue, minimumComponentSize);

    std::pair<int, int> spot = spotLocator(i, rgbImage.cols, rgbImage.rows, rgbImage);

    cv::Point topLeft(spot.second, spot.first);
    cv::Point bottomRight(spot.second + 1, spot.first + 1);

    cv::Scalar color(0, 255, 0);
    int thickness = 2;

    cv::rectangle(rgbImage, topLeft, bottomRight, color, thickness);

#ifdef DEBUG
    cv::imshow("Heatmap", rgbImage);
    cv::waitKey(0);
#endif

    return 0;
}
