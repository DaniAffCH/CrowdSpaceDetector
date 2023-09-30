#include "forest.h"

component_pointer makeComponent(const int row, const int column, const cv::Vec4b &pixelValues)
{
    component_pointer component = std::make_shared<Component>();
    node_pointer pixel = std::make_shared<Node>();
    pixel->pixel = pixelValues;
    pixel->position = cv::Point2i(column, row);

    pixel->parent = static_cast<node_pointer>(pixel);
    pixel->parentTree = static_cast<component_pointer>(component);
    component->representative = static_cast<node_pointer>(pixel);
    component->pixels.push_back(std::move(pixel));
    return component;
}

void setParentTree(const component_pointer &childTreePointer, const component_pointer &parentTreePointer)
{
    for (const node_pointer &nodePointer : childTreePointer->pixels)
    {
        parentTreePointer->pixels.push_back(nodePointer);
        nodePointer->parentTree = parentTreePointer;
    }
}

double pixelsDifferenceScore(const node_pointer &node1, const node_pointer &node2)
{
    cv::Vec4i n1 = cv::Vec4i(node1->pixel[0], node1->pixel[1], node1->pixel[2], node1->pixel[3]);
    cv::Vec4i n2 = cv::Vec4i(node2->pixel[0], node2->pixel[1], node2->pixel[2], node2->pixel[3]);
    cv::Vec4i tmp = n1 - n2;
    tmp[3] *= 2;
    return sqrt(tmp.dot(tmp));
}

edge_pointer createEdge(const node_pointer &pixel1, const node_pointer &pixel2)
{
    edge_pointer edge = std::make_shared<Edge>();
    edge->weight = pixelsDifferenceScore(pixel1, pixel2);
    edge->n1 = pixel1;
    edge->n2 = pixel2;
    return edge;
}

void mergeComponents(const component_pointer &x, const component_pointer &y, const double MSTMaxEdgeValue)
{
    if (x != y)
    {
        component_node_pointer componentStruct;
        if (x->rank < y->rank)
        {
            x->representative->parent = y->representative;
            y->MSTMaxEdge = MSTMaxEdgeValue;
            setParentTree(x, y);
            componentStruct = x->parentComponentNode;
        }
        else
        {
            if (x->rank == y->rank)
                ++x->rank;

            y->representative->parent = x->representative->parent;
            x->MSTMaxEdge = MSTMaxEdgeValue;
            setParentTree(y, x);
            componentStruct = y->parentComponentNode;
        }
        if (componentStruct->previousComponentNode)
            componentStruct->previousComponentNode->nextComponentNode = componentStruct->nextComponentNode;

        if (componentStruct->nextComponentNode)
            componentStruct->nextComponentNode->previousComponentNode = componentStruct->previousComponentNode;
    }
}