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

namespace std
{
    template<>
    struct std::hash<EdgeType>
    {
        inline size_t operator()(const EdgeType& e) const {
            std::hash<SizeType> hasher;
            return hasher(e.first) ^ hasher(e.second);
        }
    };
}

// Заполняем множество номеров ребер, которые уже присутствуют в дереве
// Потом это может быть использовано для больших плотностей, чтобы не исключить нужное ребро
Set<EdgeType> getTreeEdges(const List<Node>& tree)
{
    Set<EdgeType> edgesInds;
    edgesInds.reserve(tree.size() - 1);
    for (const auto& elem : tree) //< для каждой вершины
        for (const auto& inc : elem.incident) //< идем по списку смежности
            edgesInds.insert(std::make_pair(elem.data, tree[inc].data)); //< и запоминаем номера ребер
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

// генерируем пары случайных чисел и переводим их в ребра 
void addEdgesToTreeByOne(List<Node>& tree, SizeType edgesToAdd)
{
    Randomizer rand;
    while (edgesToAdd > 0)
    {
        SizeType firstInd = rand.uRand(0, tree.size() - 1);
        SizeType secondInd = rand.uRand(0, tree.size() - 1);

        if (checkEdgeInsertable(tree, firstInd, secondInd)) // пропускаем петли и уже существующие ребра
        {
            insertEdge(tree, firstInd, secondInd);
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