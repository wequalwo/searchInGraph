#include "graph/edge.h"

Set<EdgeType> getTreeEdges(const List<Node>& tree)
{
    Set<EdgeType> edgesInds;
    edgesInds.reserve(tree.size() - 1);
    for (const auto& elem : tree) //< для каждой вершины
        for (const auto& inc : elem.incident) //< идем по списку смежности
            edgesInds.insert(EdgeType{elem.data, tree[inc].data}); //< и запоминаем номера ребер
    return edgesInds;
}

// добавляем ребро из первой вершины во вторую и обратно
void insertEdge(List<Node>& graph, SizeType firstInd, SizeType secondInd)
{
    auto& first = graph[firstInd];
    auto& second = graph[secondInd];
    first.incident.insert(secondInd);
    second.incident.insert(firstInd);
}

// исключаем уже существующие ребра и петли
bool checkEdgeInsertable(const List<Node>& graph, SizeType first, SizeType second)
{
    if (first == second)
        return false;
    const auto& firstIncident = graph.at(first).incident;
    // if elem is absent, count == 0, so we can insert it and vice versa
    return firstIncident.count(second) == 0;
}
