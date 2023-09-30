#pragma once

#include "forest.h"
#include "spotLocator.h"

using uMatrix = std::vector<std::vector<unsigned>>;

int getSingleIndex(const int row, const int col, const int totalColumns);
std::vector<node_pointer> constructImagePixels(const cv::Mat &img_rgb, const cv::Mat &img_d);
std::vector<edge_pointer> setEdges(const std::vector<node_pointer> &pixels, int rows, int columns);
cv::Mat addColorToSegmentation(component_node_pointer componentStruct, const int rows, const int columns);
cv::Mat addHeatmap(cv::Mat originalImage, uMatrix heat);
component_pointer getLargestComponent(std::vector<node_pointer> image);
