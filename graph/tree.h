
#pragma once
#ifndef TREE_H
#define TREE_H

#include "graph/node.h"
#include "randomizer/rand.h"

// Функция для генерации случайного дерева
List<PNode> get_tree(SizeType size)
{
	if (size == 0)
		return {};

	Randomizer rand;
	List<PNode> nodes;
	nodes.reserve(size); // Резервируем память под узлы
	nodes.push_back(std::make_shared<Node>(0, List<PNode>{})); // Корневой узел

	// Создание дерева
	for (SizeType i = 1; i < size; ++i)
	{
		SizeType ancestorNode = rand.uRand(0, i - 1); // Выбираем случайного предка
		nodes.push_back(std::make_shared<Node>(i, List<PNode>{nodes[ancestorNode]})); // "подвешиваем" к предку
		nodes[ancestorNode]->incident.push_back(nodes.back()); // Добавляем связь от предка
	}

	return nodes;
}

#endif // TREE_H