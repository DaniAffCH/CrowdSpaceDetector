#pragma once

#include <vector>

#include "forest.h"
#include "utils.h"
#include "segmentation.h"

std::vector<node_pointer> segmentImage(cv::Mat img_rgb, cv::Mat img_d, float gaussianBlur, float kValue, int minimumComponentSize);
void segmentCore(const std::vector<edge_pointer> &edges, int minimumComponentSize, float kValue);