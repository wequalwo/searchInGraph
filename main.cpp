#include <iostream>
#include <string>   // Для std::stod
#include <thread>

#include "logger/logger.h"
#include "monte_carlo/monte_carlo.h"

void print_usage(const std::string& path)
{
    std::cerr << "Usage: " << path << " -n <n> -s <s> -g <g> [-j <j>] {-h | -er <d0> <d1> ... <dn>}" << std::endl;
    std::cerr << "<n> = vertices number for experiment\n";
    std::cerr << "<s> = number of searches run on each graph\n";
    std::cerr << "<g> = total number of graphs to generate for Hilbert model or for each density for ER model\n";
    std::cerr << "<j> = number of threads, optional\n";
    std::cerr << "-h: select Hilbert model for experiment with independent probabilities\n";
    std::cerr << "-er: select Erdos-Renyi model for experiment with fixed number of edges\n";
    std::cerr << "    <di> = densities for Erdos-Renyi experiment\n";
}

int main(int argc, char *argv[])
{
	if (argc < 8)
    {
        print_usage(argv[0]);
        return 1;
    }

    if (std::string(argv[1]) != "-n" || std::string(argv[3]) != "-s" || std::string(argv[5]) != "-g")
    {
        print_usage(argv[0]);
        return 1;
    }

    unsigned int n = std::atoi(argv[2]);          // Получаем количество вершин из аргументов командной строки
    unsigned int searches = std::atoi(argv[4]);   // Количество запусков для каждого графа
    unsigned int graphs = std::atoi(argv[6]);     // Количество графов для каждой плотности или всего
    unsigned int jobs = 1;                        // Количество потоков  
    unsigned int argIdx = 7;
    if (std::string(argv[argIdx]) == "-j")
    {
        jobs = std::atoi(argv[argIdx + 1]);
        argIdx += 2;
    }

    std::string mod = argv[argIdx];
    Logger log("logger/log.txt", "logger/err.txt");
    if (mod == "-er")
    {
        List<double> densities;
        for (unsigned i = ++argIdx; i < argc; ++i)
        {
            std::cout << "Density: " << std::stod(argv[i]) << std::endl;
            densities.push_back(std::stod(argv[i])); // Получаем значение плотности из аргументов командной строки
        }

        if (jobs == 1)
        {
            MonteCarlo mc(n, searches, log);   
            mc.initErdosRenyi(densities, graphs);
        }
        else
        {
            List<unsigned int> graphsCount(jobs, graphs/jobs);
            for (unsigned int i = 0; i < graphs % jobs; ++i)
                ++graphsCount[i];
            
            List<MonteCarlo> mcs;
            for (unsigned int i = 0; i < jobs; ++i)
                mcs.push_back(MonteCarlo(n, searches, log));

            List<std::thread> threads;
            for (unsigned int i = 0; i < jobs; ++i)
                threads.push_back(std::thread(&MonteCarlo::initErdosRenyi, &mcs[i], densities, graphsCount[i]));
            
            for (unsigned int i = 0; i < jobs; ++i)
                threads[i].join();
        }
    }
    else if (mod == "-h")
    {

        if (jobs == 1)
        {
            MonteCarlo mc(n, searches, log);   
            mc.initHilbert(graphs);
        }
        else
        {
            List<unsigned int> graphsCount(jobs, graphs/jobs);
            for (unsigned int i = 0; i < graphs % jobs; ++i)
                ++graphsCount[i];
            
            List<MonteCarlo> mcs;
            for (unsigned int i = 0; i < jobs; ++i)
                mcs.push_back(MonteCarlo(n, searches, log));

            List<std::thread> threads;
            for (unsigned int i = 0; i < jobs; ++i)
                threads.push_back(std::thread(&MonteCarlo::initHilbert, &mcs[i], graphsCount[i]));
            
            for (unsigned int i = 0; i < jobs; ++i)
                threads[i].join();
        }
    }
    else
    {
        print_usage(argv[0]);
        return 1;
    }

	return 0;
}