#pragma once
#ifndef GRAPH_RAND_GRAPH_H
#define GRAPH_RAND_GRAPH_H

#include "common/service.h"
#include "graph/edge.h"

// генерируем пары случайных чисел и переводим их в ребра
void erdos_renyi_graph(List<Node>& tree, unsigned int edgesToAdd);

void inverseGraph(List<Node>& tree, double density);

void setGraphDensity(List<Node>& tree, double density);
List<Node> hilbert_graph(SizeType size, double pi, int& num_edges);

// distribute weignts among edges in random order
List<WNode> to_rand_wgraph(const List<Node>& graph, const List<double>& weights);

List<WNode> to_rand_wgraph(const List<Node>& graph, double theta1 = 0, double theta2 = 10); //built-in weights generator

#endif // GRAPH_RAND_GRAPH_H