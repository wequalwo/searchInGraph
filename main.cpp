#include <iostream>

#include "randomizer/rand.h"
#include "graph/node.h"
#include "graph/tree.h"
#include "graph/edge.h"
#include "graph/traversal.h"
#include <common/service.h>


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
	traverser.traverseRand<std::queue<SizeType>>();
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
	traverser.traverseRand<std::stack<SizeType>>();
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
    std::cout << "Process has been completed\n";
	/*
    for (const auto &node : nodes) {
        std::cout << "Node " << node.data << ": ";
        for (SizeType neighbor : node.incident) {
            std::cout << neighbor << " ";
        }
        std::cout << "\n";
    }
	*/
	//std::cout << traverser.getDist();

    // #######################################
    // Построение гистограммы степеней вершин
    int trials = 1000;
    std::vector<int> hist(n, 0); 
    for (int i = 0; i < trials; i++)
    {
        nodes = get_tree(n);
	    setGraphDensity(nodes, density);
        for (auto &node : nodes)
        {
            ++hist.at(node.incident.size());
        }
    }
    write_vector_to_file(hist, "histogram.bin");

	return 0;
}