#pragma once
#ifndef RANG_GRAPH
#define RANG_GRAPH

#include "graph/edge.h"

// генерируем пары случайных чисел и переводим их в ребра
void erdos_renyi_graph(List<Node>& tree, unsigned int edgesToAdd)
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

void inverseGraph(List<Node>& tree, double density)
{
    auto edges = getTreeEdges(tree);
    for (auto& elem : tree)
        elem.incident.clear();

    unsigned int maxEdges = tree.size() * (tree.size() - 1)/2;
    unsigned int edgesToRemove = std::round(maxEdges * (1-density));
    Randomizer rand;
    while (edgesToRemove > 0)
    {
        SizeType firstInd = rand.uRand(0, tree.size() - 1);
        SizeType secondInd = rand.uRand(0, tree.size() - 1);
        if (edges.count({firstInd, secondInd}) == 0) // удаляем ребра, которых изначально не было
            if (checkEdgeInsertable(tree, firstInd, secondInd)) // пропускаем петли и уже удаленные ребра
            {
                insertEdge(tree, firstInd, secondInd);
                if (--edgesToRemove == 0)
                    return;
            }
    }
}

void setGraphDensity(List<Node>& tree, double density)
{
    if (density >= MIN_INVERSE_DENSITY)
    {
        inverseGraph(tree, density);
        return;
    }
    SizeType curEdges = tree.size() - 1;
    unsigned int maxEdges = tree.size()*(tree.size() - 1)/2;
    unsigned int needMinEdges = std::round(maxEdges * density);
    if (curEdges >= needMinEdges)
        return;
    unsigned int needEdges = needMinEdges - curEdges; // считаем, сколько ребер добавить

    using Clock = std::chrono::steady_clock;

    //std::cerr << "starting edges generating\n";
    Clock::time_point begin = Clock::now();
    erdos_renyi_graph(tree, needEdges);
    Clock::time_point end = Clock::now();
    //std::cerr << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[mcs] = "
    //          << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1'000'000.0 << " sec" << '\n';
}

List<Node> hilbert_graph(SizeType size)
{
    List<Node> graph;
    graph.reserve(size);
    for (SizeType i = 0; i < size; ++i)
        graph.push_back(Node{i, Set<SizeType>{}});

    Randomizer rand;
    for (SizeType i = 0; i < size - 1; ++i)
        for (SizeType j = i + 1; j < size; ++j)
            if (rand.randBool())
            {
                graph[i].incident.insert(j);
                graph[j].incident.insert(i);
            }

    return graph;
}

#endif // RANG_GRAPH