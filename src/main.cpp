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

#ifdef DEBUG
    cv::Point point(spot.second, spot.first);
    cv::Scalar color(0, 255, 0);
    int thickness = 3;
    cv::circle(rgbImage, point, thickness, color, cv::FILLED);
    cv::imshow("Heatmap", rgbImage);
    cv::waitKey(0);
#endif

    return 0;
}
