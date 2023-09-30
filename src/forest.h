#pragma once
#include <vector>
#include <functional>
#include <memory>
#include <cmath>
#include <opencv2/opencv.hpp>

struct Component;
struct ComponentNode;
struct Node;
struct Edge;

using component_pointer = std::shared_ptr<Component>;
using component_node_pointer = std::shared_ptr<ComponentNode>;
using node_pointer = std::shared_ptr<Node>;
using edge_pointer = std::shared_ptr<Edge>;

struct Node
{
    component_pointer parentTree;
    node_pointer parent;
    cv::Vec4b pixel;      // RGBD format
    cv::Point2i position; // <Column, Row>
};

struct Component
{
    component_node_pointer parentComponentNode;
    std::vector<node_pointer> pixels;
    int rank = 0;
    node_pointer representative;
    double MSTMaxEdge = 0;
};

struct Edge
{
    double weight;
    node_pointer n1;
    node_pointer n2;
};

struct ComponentNode
{
    component_node_pointer previousComponentNode = nullptr;
    component_pointer component{};
    component_node_pointer nextComponentNode = nullptr;
};

edge_pointer createEdge(const node_pointer &pixel1, const node_pointer &pixel2);
double pixelsDifferenceScore(const node_pointer &node1, const node_pointer &node2);
void mergeComponents(const component_pointer &x, const component_pointer &y, double MSTMaxEdgeValue);
component_pointer makeComponent(int row, int column, const cv::Vec4b &pixelValues);