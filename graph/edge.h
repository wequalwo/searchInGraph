#pragma once
#ifndef GRAPH_EDGE_H
#define GRAPH_EDGE_H

#include "graph/node.h"

// Заполняем множество номеров ребер, которые уже присутствуют в дереве
// Потом это может быть использовано для больших плотностей, чтобы не исключить нужное ребро
Set<EdgeType> getTreeEdges(const List<Node>& tree);
// добавляем ребро из первой вершины во вторую и обратно
void insertEdge(List<Node>& graph, SizeType firstInd, SizeType secondInd);

// исключаем уже существующие ребра и петли
bool checkEdgeInsertable(const List<Node>& graph, SizeType first, SizeType second);

#endif // GRAPH_EDGE_H