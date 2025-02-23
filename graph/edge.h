#pragma once

#ifndef EDGE_H
#define EDGE_H
#include <utility>
#include <cmath>

#include "common/common.h"
#include "graph/node.h"
#include "randomizer/rand.h"


using EdgeType = std::pair<SizeType, SizeType>;

SizeType edgeToInd(SizeType treeSize, SizeType firstInd, SizeType secondInd)
{
    return treeSize*std::min(firstInd, secondInd) + std::max(firstInd, secondInd);
}

SizeType edgeToInd(SizeType treeSize, const EdgeType& edgeInd)
{
    return edgeToInd(treeSize, edgeInd.first, edgeInd.second);
}

EdgeType indToEdge(SizeType treeSize, SizeType edgeInd)
{
    return std::make_pair(edgeInd / treeSize, edgeInd % treeSize);
}

Set<SizeType> getTreeEdges(const List<PNode>& tree)
{
    Set<SizeType> edgesInds;
    edgesInds.reserve(tree.size() - 1);
    for (const auto& elem : tree)
        for (const auto& inc : elem->incident)
            edgesInds.insert(edgeToInd(tree.size(), elem->data, inc->data));
    return edgesInds;
}

void insertEdge(List<PNode>& graph, const EdgeType& edgeInd)
{
    auto first = graph[edgeInd.first];
    auto second = graph[edgeInd.second];
    
    first->incident.push_back(second);
    second->incident.push_back(first);
}

bool checkEdgeInsertable(const List<PNode>& graph, const EdgeType& edgeInd)
{
    if (edgeInd.first == edgeInd.second)
        return false;
    
    const auto second = graph.at(edgeInd.second);
    const auto& firstIncident = graph.at(edgeInd.first)->incident;
    // if elem is present, std::find != std::end, so we can not insert it and vice versa
    return std::find(firstIncident.begin(), firstIncident.end(), second) == std::end(firstIncident);
}

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

void addEdgesToTree(List<PNode>& tree, SizeType edgesToAdd)
{
    //std::cerr << "total edges to add: " << edgesToAdd << '\n';
    while (edgesToAdd > 0)
    {
        //if (edgesToAdd % 50000)
            //std::cerr << "left edges to add: " << edgesToAdd << '\n';
        auto newEdges = randEdgesList(tree.size());
        for (auto& newEdge : newEdges)
        {
            if (checkEdgeInsertable(tree, newEdge))
            {
                insertEdge(tree, newEdge);
                if (--edgesToAdd == 0)
                    return;
            }
        }
        //std::cerr << "left edges to add: " << edgesToAdd << '\n';
    }
}

void setGraphDensity(List<PNode>& tree, double density)
{
    SizeType curEdges = tree.size() - 1;
    SizeType maxEdges = tree.size()*(tree.size() - 1)/2;
    SizeType needEdges = std::round(maxEdges * density) - curEdges;
    addEdgesToTree(tree, needEdges);
}

#endif //EDGE_H