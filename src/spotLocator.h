#pragma once
#include "forest.h"
#include "utils.h"
#include <bits/stdc++.h>

using uMatrix = std::vector<std::vector<unsigned>>;

std::pair<int, int> spotLocator(std::vector<node_pointer> image, int width, int height, cv::Mat cvmat);