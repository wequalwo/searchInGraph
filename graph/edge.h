#pragma once

#ifndef EDGE_H
#define EDGE_H
#include <iostream>
#include <utility>
#include <cmath>
#include <chrono>

#include "common/common.h"
#include "graph/node.h"
#include "randomizer/rand.h"

// псевдоним для пары индексов
using EdgeType = std::pair<SizeType, SizeType>;

// отображение пары индексов вершин в номер ребра
// ребра a,b и b,a совпадают, поэтому рассматриваем упорядоченные пары
SizeType edgeToInd(SizeType treeSize, SizeType firstInd, SizeType secondInd)
{
    return treeSize*std::min(firstInd, secondInd) + std::max(firstInd, secondInd);
}

SizeType edgeToInd(SizeType treeSize, const EdgeType& edgeInd)
{
    return edgeToInd(treeSize, edgeInd.first, edgeInd.second);
}

// отображение номер ребра в пару индексов вершин
EdgeType indToEdge(SizeType treeSize, SizeType edgeInd)
{
    return std::make_pair(edgeInd / treeSize, edgeInd % treeSize);
}

// Заполняем множество номеров ребер, которые уже присутствуют в дереве. Не используется.
// Можно попробовать применить при генерации — вероятно, получится быстрее, если проверять не ребро, а наличие числа в Set.
Set<SizeType> getTreeEdges(const List<Node>& tree)
{
    Set<SizeType> edgesInds;
    edgesInds.reserve(tree.size() - 1);
    for (const auto& elem : tree) //< для каждой вершины
        for (const auto& inc : elem.incident) //< идем по списку смежности
            edgesInds.insert(edgeToInd(tree.size(), elem.data, tree[inc].data)); //< и запоминаем номера ребер
    return edgesInds;
}

// добавляем ребро из первой вершины во вторую и обратно
void insertEdge(List<Node>& graph, const EdgeType& edgeInd)
{
    auto first = graph[edgeInd.first];
    auto second = graph[edgeInd.second];
    
    first.incident.insert(edgeInd.second);
    second.incident.insert(edgeInd.first);
}

// исключаем уже существующие ребра и петли 
bool checkEdgeInsertable(const List<Node>& graph, const EdgeType& edgeInd)
{
    if (edgeInd.first == edgeInd.second)
        return false;

    const auto& firstIncident = graph.at(edgeInd.first).incident;
    // if elem is absent, count == 0, so we can insert it and vice versa
    return firstIncident.count(edgeInd.second) == 0;
}

// генерируем случайное число и отображаем его в ребро
void addEdgesToTreeByOne(List<Node>& tree, SizeType edgesToAdd)
{
    Randomizer rand;
    while (edgesToAdd > 0)
    {
        SizeType edgeInd = rand.uRand(0, tree.size()*(tree.size() - 1) - 1);
        EdgeType newEdge = indToEdge(tree.size(), edgeInd);
        if (checkEdgeInsertable(tree, newEdge)) // пропускаем петли и уже существующие ребра
        {
            insertEdge(tree, newEdge);
            if (--edgesToAdd == 0)
                return;
        }
    }
}


void setGraphDensity(List<Node>& tree, double density)
{
    SizeType curEdges = tree.size() - 1;
    SizeType maxEdges = tree.size()*(tree.size() - 1)/2;
    SizeType needEdges = std::round(maxEdges * density) - curEdges; // считаем, сколько ребер добавить

    using Clock = std::chrono::steady_clock;

    std::cerr << "starting edges generating\n";
    Clock::time_point begin = Clock::now();
    addEdgesToTreeByOne(tree, needEdges);
    Clock::time_point end = Clock::now();
    std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mcs] = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';
}

#endif //EDGE_H