#include <iostream>
#include <string>   // Для std::stod
#include <thread>

#include "logger/logger.h"
#include "monte_carlo/monte_carlo.h"

void print_usage(const std::string& path)
{
    std::cerr << "Usage: " << path << " -n <n> -s <s> -g <g> [-j <j>] {-h | -er } <d0> <d1> ... <dn>" << std::endl;
    std::cerr << "<n> = vertices number for experiment\n";
    std::cerr << "<s> = number of searches run on each graph\n";
    std::cerr << "<g> = total number of graphs to generate for Hilbert model or for each density for ER model\n";
    std::cerr << "<j> = number of threads, optional\n";
    std::cerr << "-h: select Hilbert model for experiment with independent probabilities\n";
    std::cerr << "-er: select Erdos-Renyi model for experiment with fixed number of edges\n";
    std::cerr << "<di> = densities in (0, 1] for Hilbert/Erdos-Renyi experiment\n";
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
    if (!(mod == "-er" || mod == "-h"))
    {
        print_usage(argv[0]);
        return 1;
    }
    else
    {
        List<double> densities;
        for (unsigned i = ++argIdx; i < static_cast<unsigned>(argc); ++i)
        {
            std::cout << std::string("Density: ") + argv[i] + '\n';
            densities.push_back(std::stod(argv[i]));
        }
        if (densities.size() == 0)
        {
            std::cerr << "Ошибка: необходимо указать хотя бы одно значение плотности.\n";
            print_usage(argv[0]);
            return 1;
        }
        for (double density : densities)
            if (density <= 0 || density > 1)
            {
                std::cerr << "Ошибка: каждая плотность должна быть в диапазоне (0, 1].\n";
                print_usage(argv[0]);
                return 1;
            }
        if (jobs == 1)
        {
            MonteCarloTraversal mc(graphs, n, searches, densities, std::make_unique<TraversalLogger>(log, err, 0));
            if (mod == "-er")
                mc.initErdosRenyi();
            else
                mc.initHilbert();
            mc.runMonteCarlo();
        }
        else
        {
            List<unsigned int> graphsCount(jobs, graphs / jobs);
            for (unsigned int i = 0; i < graphs % jobs; ++i)
                ++graphsCount[i];

            List<MonteCarloTraversal> mcs;
            for (unsigned int i = 0; i < jobs; ++i)
            {
                mcs.push_back(MonteCarloTraversal(graphsCount[i], n, searches, densities,
                                                  std::make_unique<TraversalLogger>(log, err, i)));
                if (mod == "-er")
                    mcs[i].initErdosRenyi();
                else
                    mcs[i].initHilbert();
            }
            List<std::thread> threads;
            for (unsigned int i = 0; i < jobs; ++i)
                threads.push_back(std::thread(&MonteCarloTraversal::runMonteCarlo, &mcs[i]));

            for (unsigned int i = 0; i < jobs; ++i)
                threads[i].join();
        }
    }

    return 0;
}