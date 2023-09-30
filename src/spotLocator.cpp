#include "spotLocator.h"

inline bool isOutOfBound(int h, int w, int i, int j)
{
    return i < 0 || j < 0 || i >= h || j >= w;
}

inline void propagate(int from_r, int from_c, int to_r, int to_c, uMatrix &regionsMatrix, std::queue<std::pair<int, int>> &q, int height, int width)
{
    if (!isOutOfBound(height, width, to_r, to_c) && regionsMatrix[to_r][to_c] > regionsMatrix[from_r][from_c] + 1)
    {
        regionsMatrix[to_r][to_c] = regionsMatrix[from_r][from_c] + 1;
        q.push({to_r, to_c});
    }
}

void clearRegionsMatrixAndPushToQueue(component_node_pointer iter, uMatrix &regionsMatrix, std::queue<std::pair<int, int>> &q, bool forward)
{
    while (iter)
    {
        for (node_pointer p : iter->component->pixels)
        {
            regionsMatrix[p->position.y][p->position.x] = 0;
            q.push({p->position.y, p->position.x});
        }
        iter = forward ? iter->nextComponentNode : iter->previousComponentNode;
    }
}

std::pair<int, int> spotLocator(std::vector<node_pointer> image, int width, int height, cv::Mat cvmat)
{
    component_pointer largest_ptr = getLargestComponent(image);

    uMatrix regionsMatrix(height, std::vector<unsigned>(width, UINT_MAX));
    std::queue<std::pair<int, int>> q;

    clearRegionsMatrixAndPushToQueue(largest_ptr->parentComponentNode->nextComponentNode, regionsMatrix, q, true);
    clearRegionsMatrixAndPushToQueue(largest_ptr->parentComponentNode->previousComponentNode, regionsMatrix, q, false);

    std::pair<int, int> node;
    std::pair<int, int> bestPos;
    unsigned bestValue = 0;

    while (!q.empty())
    {
        node = q.front();
        int i = node.first;
        int j = node.second;
        q.pop();

        propagate(i, j, i - 1, j, regionsMatrix, q, height, width);
        propagate(i, j, i + 1, j, regionsMatrix, q, height, width);
        propagate(i, j, i, j - 1, regionsMatrix, q, height, width);
        propagate(i, j, i, j + 1, regionsMatrix, q, height, width);
    }

#ifdef DEBUG
    cvmat = addHeatmap(cvmat, regionsMatrix);
#endif

    return node;
}