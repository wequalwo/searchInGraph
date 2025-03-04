#pragma once
#ifndef NODE_H
#define NODE_H

#include <memory>

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


#endif // NODE_H
