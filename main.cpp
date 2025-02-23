#include <iostream>

#include "randomizer/rand.h"
#include "graph/node.h"
#include "graph/tree.h"

// Функция обхода графа: TODO
template<class T>
List<int> traverse(const List<PNode>& nodes)
{
	Set<PNode> visited; // Множество посещенных узлов
	// TOTDO: написать код
}


int main()
{
	int N = 1000; // Количество вершин
	List<PNode> nodes = get_tree(N);
	
	return 0;
}