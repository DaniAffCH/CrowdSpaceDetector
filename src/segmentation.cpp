#include "segmentation.h"

inline float thresholdFunction(const float componentSize, const float k)
{
    return k / componentSize;
}

std::vector<node_pointer> segmentImage(cv::Mat img_rgb, cv::Mat img_d, float gaussianBlur, float kValue, int minimumComponentSize)
{
    cv::GaussianBlur(img_rgb, img_rgb, cv::Size(3, 3), gaussianBlur);
    cv::GaussianBlur(img_d, img_d, cv::Size(3, 3), gaussianBlur);

    std::vector<node_pointer> pixels = constructImagePixels(img_rgb, img_d);
    std::vector<edge_pointer> edges = setEdges(pixels, img_rgb.rows, img_rgb.cols);

    auto sortingFz = [](const edge_pointer &e1, const edge_pointer &e2)
    {
        return e1->weight < e2->weight;
    };

    std::sort(edges.begin(), edges.end(), sortingFz);
    int totalComponents = img_rgb.rows * img_rgb.cols;
    segmentCore(edges, minimumComponentSize, kValue);

    auto firstComponentNode = pixels[0]->parentTree->parentComponentNode;

    while (firstComponentNode->previousComponentNode)
    {
        firstComponentNode = firstComponentNode->previousComponentNode;
    }

#ifdef DEBUG
    cv::Mat segmentedImage = addColorToSegmentation(firstComponentNode, img_rgb.rows, img_rgb.cols);
    cv::imshow("Image", segmentedImage);
    cv::waitKey(0);
#endif

    return pixels;
}

void segmentCore(const std::vector<edge_pointer> &edges, const int minimumComponentSize, const float kValue)
{
    node_pointer pixel1, pixel2;
    component_pointer component1, component2;
    double minimumInternalDifference;

    for (const edge_pointer &edge : edges)
    {
        pixel1 = edge->n1;
        pixel2 = edge->n2;

        component1 = pixel1->parentTree;
        component2 = pixel2->parentTree;

        if (component1 != component2)
        {
            minimumInternalDifference = std::min(component1->MSTMaxEdge + thresholdFunction(component1->pixels.size(), kValue),
                                                 component2->MSTMaxEdge + thresholdFunction(component2->pixels.size(), kValue));
            if (edge->weight < minimumInternalDifference)
                mergeComponents(component1, component2, edge->weight);
        }
    }

    for (const edge_pointer &edge : edges)
    {
        pixel1 = edge->n1;
        pixel2 = edge->n2;

        component1 = pixel1->parentTree;
        component2 = pixel2->parentTree;
        if (component1->representative != component2->representative && std::min(component1->pixels.size(), component2->pixels.size()) < minimumComponentSize)
            mergeComponents(component1, component2, edge->weight);
    }
}
