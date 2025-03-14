#include <iostream>

#include "logger/logger.h"
#include "monte_carlo/monte_carlo.h"

int main(int argc, char *argv[])
{
	if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <n> <d> <g> <s>" << std::endl;
        std::cerr << "<n> = vertices number for experiment\n";
        std::cerr << "<d> = density for experiment\n";
        std::cerr << "<g> = number of graphs to generate for experiment\n";
        std::cerr << "<s> = number of searches run on each graph\n";
        return 1;
    }

    int n = std::atoi(argv[1]);          // Получаем количество вершин из аргументов командной строки
    double density = std::atof(argv[2]); // Получаем значение плотности из аргументов командной строки
    int graphs = std::atoi(argv[3]);     // Количество графов для каждой плотности
    int searches = std::atoi(argv[4]);   // Количество запусков для каждого графа

	Logger log("logger/log.txt", "logger/err.txt");
    MonteCarlo mc({density}, n, graphs, searches, log);

    mc.initialize();


	return 0;
}