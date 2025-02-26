#include <iostream>

#include "randomizer/rand.h"
#include "graph/node.h"
#include "graph/tree.h"
#include "graph/edge.h"


int main()
{
	int N = 10'000; // Количество вершин
	List<Node> nodes = get_tree(N);

	setGraphDensity(nodes, 0.1);
	return 0;
}