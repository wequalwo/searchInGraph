#include <iostream>

#include "logger/logger.h"
#include "monte_carlo/monte_carlo.h"

int main(int argc, char *argv[])
{
	if (argc < 5)
    {
        std::cerr << "Usage: " << argv[0] << " <n> <g> <s> <d0> <d1> ... <dn>" << std::endl;
        std::cerr << "<n> = vertices number for experiment\n";
        std::cerr << "<g> = number of graphs to generate for experiment\n";
        std::cerr << "<s> = number of searches run on each graph\n";
        std::cerr << "<di> = densities for experiment\n";
        return 1;
    }


    int n = std::atoi(argv[1]);          // Получаем количество вершин из аргументов командной строки
    int graphs = std::atoi(argv[2]);     // Количество графов для каждой плотности
    int searches = std::atoi(argv[3]);   // Количество запусков для каждого графа

    List<double> densities;
    for (unsigned i = 4; i < argc; ++i)
        densities.push_back(std::atof(argv[i])); // Получаем значение плотности из аргументов командной строки

	Logger log("logger/log.txt", "logger/err.txt");
    MonteCarlo mc(densities, n, graphs, searches, log);

    mc.initialize();

	return 0;
}