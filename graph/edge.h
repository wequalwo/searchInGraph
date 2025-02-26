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

// заполняем множество номеров ребер, которые уже присутствуют в дереве
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
    // if elem is absent, std::find == std::end, so we can insert it and vice versa
    return firstIncident.find(edgeInd.second) == firstIncident.end();
}

// генерация списка случайных ребер
// берем 2 случайных перестановки чисел от 0 до treeSize - 1
// пары чисел с одинаковым индексом — добавляемые ребра
List<EdgeType> randEdgesList(SizeType treeSize)
{
    List<SizeType> x(treeSize), y(treeSize);
    Randomizer rand;
    for (SizeType i = 0; i < treeSize; ++i)
    {
        x[i] = i;
        y[i] = i;
    }
    rand.shuffle(x);
    rand.shuffle(y);

    List<EdgeType> res;
    res.reserve(treeSize);

    for (SizeType i = 0; i < treeSize; ++i)
        res.push_back(std::make_pair(x[i], y[i]));
    
    return res;
}

// генерация списка случайных ребер
// берем 2 случайных перестановки чисел от 0 до treeSize - 1
// пары чисел с одинаковым индексом — добавляемые ребра
List<EdgeType> randEdgesShuffleList(SizeType treeSize)
{
    List<SizeType> x(treeSize), y(treeSize);
    Randomizer rand;
    for (SizeType i = 0; i < treeSize; ++i)
    {
        x[i] = i;
        y[i] = i;
    }
    rand.shuffle(x);
    rand.shuffle(y);

    List<EdgeType> res;
    res.reserve(treeSize);

    for (SizeType i = 0; i < treeSize; ++i)
        res.push_back(std::make_pair(x[i], y[i]));
    
    return res;
}

List<EdgeType> randEdgesRandIndsList(SizeType treeSize)
{
    List<EdgeType> res;
    res.reserve(treeSize);
    Randomizer rand;
    SizeType maxInd = treeSize * (treeSize - 1) / 2;
    for (SizeType i = 0; i < treeSize; ++i)
        res.push_back(indToEdge(treeSize, rand.rand(0, maxInd)));

    return res;
}

void addEdgesToTreeByRandList(List<Node>& tree, SizeType edgesToAdd)
{
    Randomizer rand;
    //SizeType diff = 0;
    while (edgesToAdd > 0)
    {
        //SizeType prev = edgesToAdd;
        auto newEdges = randEdgesList(tree.size());
        for (auto& newEdge : newEdges)
        {
            if (checkEdgeInsertable(tree, newEdge)) // пропускаем петли и уже существующие ребра
            {
                insertEdge(tree, newEdge);
                if (--edgesToAdd == 0)
                    return;
            }
        }
        /*diff += prev - edgesToAdd;
        if (diff >= 50'000)
        {
            std::cerr << "left edges to add: " << edgesToAdd << '\n';
            diff = 0;    
        }*/
    }
}

void addEdgesToTreeByOne(List<Node>& tree, SizeType edgesToAdd)
{
    Randomizer rand;
    while (edgesToAdd > 0)
    {
        //if (edgesToAdd % 50000 == 0)
          //  std::cerr << "left edges to add: " << edgesToAdd << '\n';
        SizeType edgeInd = rand.uRand(0, tree.size()*(tree.size() - 1)/2);
        EdgeType newEdge = indToEdge(tree.size(), edgeInd);
        if (checkEdgeInsertable(tree, newEdge)) // пропускаем петли и уже существующие ребра
        {
            insertEdge(tree, newEdge);
            if (--edgesToAdd == 0)
                return;
        }
    }
}

void addEdgesToTreeShuffleLists(List<Node>& tree, SizeType edgesToAdd)
{
    //std::cerr << "total edges to add: " << edgesToAdd << '\n';
    //SizeType diff = 0;
    while (edgesToAdd > 0)
    {
        //SizeType prev = edgesToAdd;
        auto newEdges = randEdgesList(tree.size());
        for (auto& newEdge : newEdges)
        {
            if (checkEdgeInsertable(tree, newEdge)) // пропускаем петли и уже существующие ребра
            {
                insertEdge(tree, newEdge);
                if (--edgesToAdd == 0)
                    return;
            }
        }
        /*diff += prev - edgesToAdd;
        if (diff >= 50'000)
        {
            std::cerr << "left edges to add: " << edgesToAdd << '\n';
            diff = 0;    
        }*/
    }
}

void setGraphDensity(List<Node>& tree, double density)
{
    auto copyTree = tree;
    SizeType curEdges = tree.size() - 1;
    SizeType maxEdges = tree.size()*(tree.size() - 1)/2;
    SizeType needEdges = std::round(maxEdges * density) - curEdges; // считаем, сколько ребер добавить

    using Clock = std::chrono::steady_clock;
    std::cerr << "starting shuffleLists\n";
    Clock::time_point begin = Clock::now();
    addEdgesToTreeShuffleLists(tree, needEdges);
    Clock::time_point end = Clock::now();
    std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';

    tree = copyTree;
    std::cerr << "starting byOne\n";
    begin = Clock::now();
    addEdgesToTreeByOne(tree, needEdges);
    end = Clock::now();
    std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';

    tree = copyTree;
    std::cerr << "starting randLists\n";
    begin = Clock::now();
    addEdgesToTreeByRandList(tree, needEdges);
    end = Clock::now();
    std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs] = "
            << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';

}

#endif //EDGE_H