#include "utils.h"

int getSingleIndex(const int row, const int col, const int totalColumns)
{
    return (row * totalColumns) + col;
}

component_pointer getLargestComponent(std::vector<node_pointer> image)
{
    node_pointer pixel = image[0];
    component_pointer comp = pixel->parentTree;
    size_t largest = comp->pixels.size();
    component_pointer largest_ptr = comp;
    component_node_pointer comp_ll = comp->parentComponentNode;
    size_t tmp_size;

    while (comp_ll)
    {
        tmp_size = comp_ll->component->pixels.size();
        if (tmp_size > largest)
        {
            largest = tmp_size;
            largest_ptr = comp_ll->component;
        }
        comp_ll = comp_ll->previousComponentNode;
    }

    comp_ll = comp->parentComponentNode;

    while (comp_ll)
    {
        tmp_size = comp_ll->component->pixels.size();
        if (tmp_size > largest)
        {
            largest = tmp_size;
            largest_ptr = comp_ll->component;
        }
        comp_ll = comp_ll->nextComponentNode;
    }

    return largest_ptr;
}

int getEdgeArraySize(const int rows, const int columns)
{
    int firstColumn = 3 * (rows - 1);
    int lastColumn = 2 * (rows - 1);
    int middleValues = 4 * (rows - 1) * (columns - 2);
    int lastRow = columns - 1;
    return firstColumn + lastColumn + middleValues + lastRow;
}

std::vector<node_pointer> constructImagePixels(const cv::Mat &img_rgb, const cv::Mat &img_d)
{
    std::vector<node_pointer> pixels(img_rgb.rows * img_rgb.cols);

    cv::Vec3b pv = img_rgb.at<cv::Vec3b>(0, 0);
    uchar pv_d = img_d.at<uchar>(0,0);
    cv::Vec4b extended(pv[0], pv[1], pv[2], pv_d);
    component_pointer firstComponent = makeComponent(0, 0, extended);
    component_node_pointer firstComponentStruct = std::make_shared<ComponentNode>();
    firstComponentStruct->component = firstComponent;
    auto previousComponentStruct = firstComponentStruct;
    int index;

    for (int row = 0; row < img_rgb.rows; row++)
    {
        for (int column = 0; column < img_rgb.cols; column++)
        {
            cv::Vec3b pv = img_rgb.at<cv::Vec3b>(row, column);
            uchar pv_d = img_d.at<uchar>(row, column);
            cv::Vec4b extended(pv[0], pv[1], pv[2], pv_d);

            component_pointer component = makeComponent(row, column, extended);
            component_node_pointer newComponentStruct = std::make_shared<ComponentNode>();
            newComponentStruct->component = component;
            newComponentStruct->previousComponentNode = previousComponentStruct;
            previousComponentStruct->nextComponentNode = newComponentStruct;
            component->parentComponentNode = newComponentStruct;
            previousComponentStruct = newComponentStruct;
            index = getSingleIndex(row, column, img_rgb.cols);
            pixels[index] = component->pixels.at(0);
        }
    }
    firstComponentStruct = firstComponentStruct->nextComponentNode;
    firstComponentStruct->previousComponentNode = nullptr;
    return pixels;
}

std::vector<edge_pointer> setEdges(const std::vector<node_pointer> &pixels, const int rows, const int columns)
{
    int edgeArraySize = getEdgeArraySize(rows, columns);
    std::vector<edge_pointer> edges(edgeArraySize);

    int edgeCount = 0;
    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0; column < columns; ++column)
        {
            node_pointer presentPixel = pixels[getSingleIndex(row, column, columns)];
            if (row < rows - 1)
            {
                if (column == 0)
                {
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row, column + 1, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row + 1, column + 1, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row + 1, column, columns)]);
                }
                else if (column == columns - 1)
                {
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row + 1, column, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row + 1, column - 1, columns)]);
                }
                else
                {
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row, column + 1, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row + 1, column + 1, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row + 1, column, columns)]);
                    edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row + 1, column - 1, columns)]);
                }
            }
            else if (row == rows - 1 && column != columns - 1)
                edges[edgeCount++] = createEdge(presentPixel, pixels[getSingleIndex(row, column + 1, columns)]);
        }
    }
    return edges;
}

int getRandomNumber(const int min, const int max)
{
    static constexpr double fraction{1.0 / (RAND_MAX + 1.0)};
    return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
}

cv::Mat addColorToSegmentation(component_node_pointer componentStruct, const int rows, const int columns)
{
    cv::Mat segmentedImage(rows, columns, CV_8UC3);
    do
    {
        uchar r = getRandomNumber(0, 255);
        uchar b = getRandomNumber(0, 255);
        uchar g = getRandomNumber(0, 255);
        cv::Vec3b pixelColor = {b, g, r};
        for (const node_pointer &pixel : componentStruct->component->pixels)
        {
            segmentedImage.at<cv::Vec3b>(pixel->position) = pixelColor;
        }
        componentStruct = componentStruct->nextComponentNode;
    } while (componentStruct);

    return segmentedImage;
}

cv::Scalar heatMapToRGB(float maximum, float value)
{
    float ratio = value / maximum;

    int r = static_cast<int>(255 * (1 - ratio));
    int g = 0;
    int b = static_cast<int>(255 * ratio);

    return cv::Scalar(r, g, b);
}

cv::Mat addHeatmap(cv::Mat originalImage, uMatrix heat)
{
    unsigned maxValue = 0;
    int thickness = 2;
    cv::Scalar color;

    cv::Mat overlay;
    double alpha = 0.5;

    originalImage.copyTo(overlay);

    for (int i = 0; i < heat.size(); ++i)
        for (int j = 0; j < heat[0].size(); ++j)
            maxValue = std::max(maxValue, heat[i][j]);

    for (int i = 0; i < heat.size(); ++i)
    {
        for (int j = 0; j < heat[0].size(); ++j)
        {
            cv::Point point(j, i);
            color = heatMapToRGB(maxValue, heat[i][j]);
            cv::circle(overlay, point, 0, color, cv::FILLED);
        }
    }

    cv::addWeighted(overlay, alpha, originalImage, 1 - alpha, 0, originalImage);

    return originalImage;
}