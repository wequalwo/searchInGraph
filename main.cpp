#include <iostream>
#include <string>   // Для std::stod
#include <thread>

#include "logger/logger.h"
#include "monte_carlo/monte_carlo.h"

void print_usage(const std::string& path)
{
    std::cerr << "Usage: " << path << " -n <n> -s <s> -g <g> [-j <j>] {-h <pi> | -er <d0> <d1> ... <dn>}" << std::endl;
    std::cerr << "<n> = vertices number for experiment\n";
    std::cerr << "<s> = number of searches run on each graph\n";
    std::cerr << "<g> = total number of graphs to generate for Hilbert model or for each density for ER model\n";
    std::cerr << "<j> = number of threads, optional\n";
    std::cerr << "-h <pi>: select Hilbert model for experiment with independent probabilities\n";
    std::cerr << "         <pi> = probability in (0, 1) for the Hilbert model\n";
    std::cerr << "-er: select Erdos-Renyi model for experiment with fixed number of edges\n";
    std::cerr << "     <di> = densities for Erdos-Renyi experiment\n";
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

    unsigned int n = std::atoi(argv[2]);          // Количество вершин
    unsigned int searches = std::atoi(argv[4]);   // Количество запусков на граф
    unsigned int graphs = std::atoi(argv[6]);     // Количество графов
    unsigned int jobs = 1;                        // Количество потоков
    unsigned int argIdx = 7;

    if (std::string(argv[argIdx]) == "-j")
    {
        jobs = std::atoi(argv[argIdx + 1]);
        argIdx += 2;
    }

    if (argIdx >= static_cast<unsigned int>(argc))
    {
        print_usage(argv[0]);
        return 1;
    }

    std::string mod = argv[argIdx];
    std::string log = "logger/log", err = "logger/err";

    if (mod == "-er")
    {
        List<double> densities;
        for (unsigned i = ++argIdx; i < static_cast<unsigned>(argc); ++i)
        {
            std::cout << "Density: " << std::stod(argv[i]) << std::endl;
            densities.push_back(std::stod(argv[i]));
        }

        if (jobs == 1)
        {
            MonteCarlo mc(n, searches, log, err, 0);
            mc.initErdosRenyi(densities, graphs);
        }
        else
        {
            List<unsigned int> graphsCount(jobs, graphs / jobs);
            for (unsigned int i = 0; i < graphs % jobs; ++i)
                ++graphsCount[i];

            List<MonteCarlo> mcs;
            for (unsigned int i = 0; i < jobs; ++i)
                mcs.push_back(MonteCarlo(n, searches, log, err, i));

            List<std::thread> threads;
            for (unsigned int i = 0; i < jobs; ++i)
                threads.push_back(std::thread(&MonteCarlo::initErdosRenyi, &mcs[i], densities, graphsCount[i]));

            for (unsigned int i = 0; i < jobs; ++i)
                threads[i].join();
        }
    }
    else if (mod == "-h")
    {
        if (argIdx + 1 >= static_cast<unsigned int>(argc))
        {
            std::cerr << "Ошибка: для модели Гильберта необходимо указать параметр pi.\n";
            print_usage(argv[0]);
            return 1;
        }

        double pi = std::stod(argv[++argIdx]);
        if (pi <= 0.0 || pi >= 1.0)
        {
            std::cerr << "Ошибка: параметр pi должен быть в диапазоне (0, 1).\n";
            return 1;
        }

        if (jobs == 1)
        {
            MonteCarlo mc(n, searches, log, err, 0);
            mc.initHilbert(graphs, pi);
        }
        else
        {
            List<unsigned int> graphsCount(jobs, graphs / jobs);
            for (unsigned int i = 0; i < graphs % jobs; ++i)
                ++graphsCount[i];

            List<MonteCarlo> mcs;
            for (unsigned int i = 0; i < jobs; ++i)
                mcs.push_back(MonteCarlo(n, searches, log, err, i));

            List<std::thread> threads;
            for (unsigned int i = 0; i < jobs; ++i)
                threads.push_back(std::thread(&MonteCarlo::initHilbert, &mcs[i], graphsCount[i], pi));

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