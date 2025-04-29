#include <algorithm>

#include "graph/coloring/colorer.h"

int Colorer::greedyColoring(const List<Node>& graph)
{
    SizeType size = graph.size();
    List<SizeType> degs, order, colors(size, 0);
    degs.reserve(size);
    order.reserve(size);
    Set<SizeType> colored;
    for (SizeType i = 0; i < size; ++i)
    {
        degs.push_back(graph[i].incident.size()); //< degs[i] = degree of vertex with index i
        order.push_back(i);
    }
    // sort indices according to nodes degrees in descending order
    // i.e. order[0] = index of vertex with max degree 
    std::sort(order.begin(), order.end(), [&degs](SizeType left, SizeType right)
                                                { return degs[left] > degs[right]; });
    
    SizeType color = 1;
    while (colored.size() < size)
    {
        for (SizeType vInd : order) // starting from vertex with max degree
            if (colored.count(vInd) == 0)
            {
                bool colorUsable = true;
                const auto& adjVs = graph[vInd].incident;
                for (auto it = adjVs.begin(); it != adjVs.end() && colorUsable == true; ++it)
                    colorUsable = colors[*it] != color;
                if (colorUsable)
                {
                    colored.insert(vInd);
                    colors[vInd] = color;
                }
            }
        ++color;
    }
    return color - 1;
}

int Colorer::naiveColoring(const List<Node>& graph)
{
    SizeType size = graph.size();
    List<SizeType> colors(size, 0);
    for (SizeType i = 0; i < size; ++i)
    {
        Set<SizeType> candidateColors;
        for (SizeType j = 1; j <= size; ++j)
            candidateColors.insert(i);
        for (const auto& adjVert : graph[i].incident)
            candidateColors.erase(colors[adjVert]);

        colors[i] = *std::min_element(candidateColors.begin(), candidateColors.end());
    }
    return *std::max_element(colors.begin(), colors.end());
}
