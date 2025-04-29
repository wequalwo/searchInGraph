#pragma once
#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "common/common.h"

// Структура узла графа
struct Node
{
	// Конструктор принимает ID узла и список инцидентных узлов
	Node(SizeType id, Set<SizeType> nodes) : data(id), incident(nodes)
		{ }

    SizeType data;            // Значение узла
	Set<SizeType> incident;   // Список соседних узлов (смежность)
};


#endif // GRAPH_NODE_H
