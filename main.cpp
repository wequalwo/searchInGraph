#include <iostream>
#include <string>   // Для std::stod
#include <thread>


#include "monte_carlo/monte_carlo_traversal.h"
#include "monte_carlo/monte_carlo_trees.h"
#include "monte_carlo/monte_carlo_colorer.h"
#include "monte_carlo/monte_carlo_degs.h"

void print_usage(const std::string& path)
{
    std::cerr << "Usage: " << path << " -n <n> -s <s> -g <g> [-j <j>] { -col | -sp | -tr } {-h | -er } <d0> <d1> ... <dn>" << std::endl;
    std::cerr << "<n> = vertices number for experiment\n";
    std::cerr << "<s> = number of searches run on each graph\n";
    std::cerr << "<g> = total number of graphs to generate for Hilbert model or for each density for ER model\n";
    std::cerr << "<j> = number of threads, optional\n";
    std::cerr << "-col: run coloring experiment\n";
    std::cerr << "-sp: run spanning trees experiment\n";
    std::cerr << "-tr: run traversal experiment\n";
    std::cerr << "-deg: run counting degrees experiment\n";
    std::cerr << "-h: select Hilbert model for experiment with independent probabilities\n";
    std::cerr << "-er: select Erdos-Renyi model for experiment with fixed number of edges\n";
    std::cerr << "<di> = densities in (0, 1] for Hilbert/Erdos-Renyi experiment\n";
}

bool check_valid_args(int argc, char *argv[])
{
    if (argc < 9)
        return false;
    
    if (std::string(argv[1]) != "-n" || std::string(argv[3]) != "-s" || std::string(argv[5]) != "-g")
        return false;

    return true;
}

bool check_densities(const List<double>& d)
{
    if (d.size() == 0)
    {
        std::cerr << "Ошибка: необходимо указать хотя бы одно значение плотности.\n";
        return false;
    }
    for (double density : d)
        if (density <= 0 || density > 1)
        {
            std::cerr << "Ошибка: каждая плотность должна быть в диапазоне (0, 1].\n";
            return false;
        }
    return true;
}

bool check_graph_exp_types(const std::string& gr, const std::string& exp)
{
    if (!(gr == "-er" || gr == "-h"))
        return false;
    if (!(exp == "-col" || exp == "-sp" || exp == "-tr" || exp == "-deg"))
        return false;
    return true;
}

List<std::unique_ptr<MonteCarlo>> prepare_experiments(unsigned int g, unsigned int n, unsigned int s,
                                                      unsigned int j, const std::string& expT, const std::string& grT,
                                                      const List<double>& d)
{
    std::string log = "logs/log_" + expT.substr(1), err = "logs/err_" + expT.substr(1);
    // counting graphs for each thread
    List<unsigned int> gs(j, 0);
    for (unsigned int i = 0; i < j; ++i)
        gs[i] = g / j;
    for (unsigned int i = 0; i < g % j; ++i)
        ++gs[i];

    List<std::unique_ptr<MonteCarlo>> res;
    double theta1 = 0;
    double theta2 = n;
    if (expT == "-sp")
    {   
        std::cout << "Enter theta1 and theta2 distribution parameters: ";
        std::cin >> theta1 >> theta2;
        std::cout << '\n';
    }
    for (unsigned int i = 0; i < j; ++i)
    {
        if (expT == "-col")
        {
            res.push_back(std::make_unique<MonteCarloColorer>(
                              gs[i], n, s, d, std::make_unique<ColoringLogger>(log, err, i)));
        }
        else if (expT == "-tr")
        {
            res.push_back(std::make_unique<MonteCarloTraversal>(
                              gs[i], n, s, d, std::make_unique<TraversalLogger>(log, err, i)));
        }
        else if (expT == "-sp")
        {
            res.push_back(std::make_unique<MonteCarloTrees>(
                              gs[i], n, s, d, std::make_unique<SpanningLogger>(log, err, i)));
            try
            {
                dynamic_cast<MonteCarloTrees*>(res.back().get())->setParaps(theta1, theta2);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        else if (expT == "-deg")
        {
            res.push_back(std::make_unique<MonteCarloDegs>(
                              gs[i], n, s, d, std::make_unique<DegreeLogger>(log, err, i)));
        }
    }
    for (auto& mc : res)
        if (grT == "-h")
            mc->initHilbert();
        else if (grT == "-er")
            mc->initErdosRenyi();
    return res;
}

void run_experiments(List<std::unique_ptr<MonteCarlo>>& mcs)
{
    if (mcs.size() == 1)
        mcs[0]->runMonteCarlo();
    else
    {
        List<std::thread> threads;
        for (unsigned int i = 0; i < mcs.size(); ++i)
            threads.push_back(std::thread(&MonteCarlo::runMonteCarlo, mcs[i].get()));

        for (unsigned int i = 0; i < mcs.size(); ++i)
            threads[i].join();
    }
}

int main(int argc, char *argv[])
{
    if (!check_valid_args(argc, argv))
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

    std::string expType = argv[argIdx++];
    std::string graphType = argv[argIdx++];
    if (!check_graph_exp_types(graphType, expType))
    {
        print_usage(argv[0]);
        return 1;
    }
    else
    {
        List<double> densities;
        for (unsigned i = argIdx; i < static_cast<unsigned>(argc); ++i)
        {
            std::cout << std::string("Density: ") + argv[i] + '\n';
            densities.push_back(std::stod(argv[i]));
        }
        if (!check_densities(densities))
        {
            print_usage(argv[0]);
            return 1;
        }

        auto mcs = prepare_experiments(graphs, n, searches, jobs, expType, graphType, densities);
        run_experiments(mcs);
    }

    return 0;
}