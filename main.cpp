#include <iostream>
#include <string>   // Для std::stod
#include <thread>

#include "logger/logger.h"
#include "monte_carlo/monte_carlo.h"

void print_usage(const std::string& path)
{
    std::cerr << "Usage: " << path << " -n <n> -g <g> -s <s> [-j <j>] <d0> <d1> ... <dn>" << std::endl;
    std::cerr << "<n> = vertices number for experiment\n";
    std::cerr << "<g> = number of graphs to generate for experiment\n";
    std::cerr << "<s> = number of searches run on each graph\n";
    std::cerr << "<j> = number of threads, optional\n";
    std::cerr << "<di> = densities for experiment\n";
}

int main(int argc, char *argv[])
{
	if (argc < 7)
    {
        print_usage(argv[0]);
        return 1;
    }

    if (std::string(argv[1]) != "-n" || std::string(argv[3]) != "-g" || std::string(argv[5]) != "-s")
    {
        print_usage(argv[0]);
        return 1;
    }

    unsigned int n = std::atoi(argv[2]);          // Получаем количество вершин из аргументов командной строки
    unsigned int graphs = std::atoi(argv[4]);     // Количество графов для каждой плотности
    unsigned int searches = std::atoi(argv[6]);   // Количество запусков для каждого графа
    unsigned int jobs = 1;                        // Количество потоков  
    unsigned int firstDensity = 7;
    if (std::string(argv[7]) == "-j")
    {
        jobs = std::atoi(argv[8]);
        firstDensity += 2;
    }

    List<double> densities;
    for (unsigned i = firstDensity; i < argc; ++i)
    {
        std::cout << "Density: " << std::stod(argv[i]) << std::endl;
        densities.push_back(std::stod(argv[i])); // Получаем значение плотности из аргументов командной строки
    }
	Logger log("logger/log.txt", "logger/err.txt");

    if (jobs == 1)
    {
        MonteCarlo mc(densities, n, graphs, searches, log);   
        mc.initialize();
    }
    else
    {
        List<unsigned int> graphsCount(jobs, graphs/jobs);
        for (unsigned int i = 0; i < graphs % jobs; ++i)
            ++graphsCount[i];
        
        List<MonteCarlo> mcs;
        for (unsigned int i = 0; i < jobs; ++i)
            mcs.push_back(MonteCarlo(densities, n, graphsCount[i], searches, log));

        List<std::thread> threads;
        for (unsigned int i = 0; i < jobs; ++i)
            threads.push_back(std::thread(&MonteCarlo::initialize, &mcs[i]));
        
        for (unsigned int i = 0; i < jobs; ++i)
            threads[i].join();
    }

	return 0;
}