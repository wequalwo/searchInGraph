
#pragma once
#ifndef TREE_H
#define TREE_H

#include "graph/node.h"
#include "randomizer/rand.h"

// Функция для генерации случайного дерева
List<Node> get_tree(SizeType size)
{
	if (size == 0)
		return {};

	Randomizer rand;
	List<Node> nodes;
	nodes.reserve(size); // Резервируем память под узлы
	nodes.push_back(Node(0, Set<SizeType>{})); // Корневой узел

	// Создание дерева
	for (SizeType i = 1; i < size; ++i)
	{
		SizeType ancestorNode = rand.uRand(0, i - 1); // Выбираем случайного предка
		nodes.push_back(Node(i, Set<SizeType>{ancestorNode})); // "подвешиваем" к предку
		nodes[ancestorNode].incident.insert(i); // Добавляем связь от предка
	}

	return nodes;
}

#endif // TREE_H