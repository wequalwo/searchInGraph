#pragma once
#ifndef GRAPH_TREE_SPANNING_TREE_H
#define GRAPH_TREE_SPANNING_TREE_H

#include "graph/node.h"

class Spanner
{
    public:
    List<WNode> pSpan(const List<WNode>& graph); // Prim algorithm
    List<WNode> kSpan(const List<WNode>& graph); // Kruskal algorithm


    List<WNode> primSpan(const List<WNode>& graph); // Prim algorithm (optimized)
    List<WNode> kruskalSpan(const List<WNode>& graph); // Kruskal algorithm
    
    

    private:
    void initComps(List<Set<SizeType>*>& comps, SizeType size);
    void mergeComps(List<Set<SizeType>*>& comps, SizeType first, SizeType second);
    void clearComps(List<Set<SizeType>*>& comps);
};

#endif // GRAPH_TREE_SPANNING_TREE_H