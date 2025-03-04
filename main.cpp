#include <iostream>

#include "randomizer/rand.h"
#include "graph/node.h"
#include "graph/tree.h"
#include "graph/edge.h"
#include "graph/traversal.h"


int main(int argc, char *argv[])
{
	if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        return 1;
    }

    int n = std::atoi(argv[1]);          // Получаем значение N из аргументов командной строки
    double density = std::atof(argv[2]); // Получаем значение плотности из аргументов командной строки
	List<Node> nodes = get_tree(n);

	setGraphDensity(nodes, density);

	std::cout << "Try path find\n";

    Traverser traverser(&nodes);
	traverser.traverseRand<std::queue<SizeType>>(density >= MIN_INVERSE_DENSITY);
	traverser.getPath();
    SizeType first = traverser.getFirst();
    SizeType last = traverser.getLast();
    std::cout << "\nfirst = " << first << ", last = " << last << "\n";
    List<SizeType> path = traverser.getPath();
    for (auto id : path)
    {
        std::cout << id << " ";
    }
    std::cout << "\n"; 

	traverser.clear();
	traverser.traverseRand<std::stack<SizeType>>(density >= MIN_INVERSE_DENSITY);
	traverser.getPath();
    first = traverser.getFirst();
    last = traverser.getLast();
    std::cout << "first = " << first << ", last = " << last << "\n";
    path = traverser.getPath();
    for (auto id : path)
    {
        std::cout << id << " ";
    }
    std::cout << "\n"; 
        
    for (const auto &node : nodes) {
        std::cout << "Node " << node.data << ": ";
        for (SizeType neighbor : node.incident) {
            std::cout << neighbor << " ";
        }
        std::cout << "\n";
    }
	//std::cout << traverser.getDist();

	return 0;
}