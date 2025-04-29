#pragma once
#ifndef GRAPH_COLORING_COLORER_H
#define GRAPH_COLORING_COLORER_H

#include "graph/node.h"

class Colorer
{
    public:
    int greedyColoring(const List<Node>& graph);

    int naiveColoring(const List<Node>& graph);
};

#endif // GRAPH_COLORING_COLORER_H