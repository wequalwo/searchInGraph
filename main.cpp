#include <iostream>

#include "logger/logger.h"
#include "monte_carlo/monte_carlo.h"

int main(int argc, char *argv[])
{
	if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <n>" << std::endl;
        return 1;
    }

    int n = std::atoi(argv[1]);          // Получаем количество вершин из аргументов командной строки
    double density = std::atof(argv[2]); // Получаем значение плотности из аргументов командной строки
    int graphs = std::atoi(argv[3]);     // Количество графов
    int searches = std::atoi(argv[4]);   // Количество запусков

	Logger log("logger/log.txt", "logger/err.txt");
    MonteCarlo mc({density}, n, graphs, searches, log);

    mc.initialize();


	return 0;
}