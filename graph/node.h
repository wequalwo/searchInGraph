#pragma once
#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "common/common.h"

// Структура узла графа
struct Node
{
	// Конструктор принимает ID узла и множество смежных узлов
	Node(SizeType id, Set<SizeType> nodes) : data(id), incident(nodes)
		{ }

    SizeType data;            // Значение узла
	Set<SizeType> incident;   // Список соседних узлов (смежность)
};

struct WNode
{
	// Конструктор принимает ID узла и множество инцидентых ребер с весами
	WNode(SizeType id, Map<SizeType, double> nodes) : data(id), incident(nodes)
		{ }

    SizeType data;            		  // Значение узла
	Map<SizeType, double> incident;   // Веса инцидентных ребер
};

#endif // GRAPH_NODE_H
