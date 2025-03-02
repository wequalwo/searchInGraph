#include <iostream>

#include "randomizer/rand.h"
#include "graph/node.h"
#include "graph/tree.h"
#include "graph/edge.h"
#include "graph/traversal.h"



int main()
{
	int N = 10'000; // Количество вершин
	List<Node> nodes = get_tree(N);

	setGraphDensity(nodes, 0.1);

	Traverser traverser(&nodes);
	traverser.traverseRand<std::queue<SizeType>>();
	std::cout << traverser.getDist();

	traverser.clear();
	traverser.traverseRand<std::stack<SizeType>>();
	std::cout << traverser.getDist();

	return 0;
}