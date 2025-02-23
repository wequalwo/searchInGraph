
#pragma once
#ifndef NODE_H
#define NODE_H

#include <memory>

#include "common/common.h"

// Объявление структуры узла графа
struct Node;

template <class T>
using SharedPtr = std::shared_ptr<T>;

// Определение псевдонима для указателя на Node
using PNode = SharedPtr<Node>;

// Структура узла графа
struct Node
{
	// Конструктор принимает ID узла и список инцидентных узлов
	Node(SizeType id, List<PNode> nodes) : data(id), incident(nodes)
		{ }

    SizeType data;               // Значение узла
	List<PNode> incident;   // Список соседних узлов (смежность)
};


#endif // NODE_H
