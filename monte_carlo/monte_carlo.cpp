#include <iostream>
#include "monte_carlo.h"
#include "graph/rand_graph.h"
#include "prufer_graph/prufer.h"
#include "prufer_graph/random_graph.h"

void MonteCarlo::initHilbert()
{
    m_graphType = GraphType::eHilbert;
}

void MonteCarlo::initErdosRenyi()
{
    m_graphType = GraphType::eHilbert;
}

void MonteCarloTraversal::clear() {
    m_graph.clear();
    m_bfsResults.clear();
    m_dfsResults.clear();
    m_dist.clear();
}

const List<int>& MonteCarloTraversal::getBFSResults() const {
    return m_bfsResults;
}

const List<int>& MonteCarloTraversal::getDFSResults() const {
    return m_dfsResults;
}

// Инициализация алгоритма
void MonteCarloTraversal::runMonteCarlo() {

    if (m_graphType == GraphType::eNone)
        throw std::logic_error("monte carlo traverse experiment not inited");
    using Clock = std::chrono::steady_clock;
    std::cerr << "starting RANDOM SEARCH process on " + std::to_string(m_densities.size()) + " densities with "
                 + std::to_string(m_numGraphs) + (m_graphType == GraphType::eErdosRenyi ? " Erdos-Renyi" : " Hilbert")
                 + " graphs each\n";
    Clock::time_point begin = Clock::now();
    Clock::time_point iter = begin;
    Clock::time_point persearch = begin;
    float avg = 0;
    //Clock::time_point end = iter;
    double logDenisty;

    for (double curDensity : m_densities)
    {
        std::cerr << std::string("density: ") + std::to_string(curDensity) + '\n';
        iter = Clock::now();
        avg = 0;
        for (int graphIndex = 0; graphIndex < m_numGraphs; ++graphIndex)
        {
            // TODO разделить методы: надо получать не только эти данные
            if (m_graphType == GraphType::eErdosRenyi)
            {
                try
                {
                    m_graph = buildGraph(m_numVertices, curDensity);
                    logDenisty = curDensity;
                }
                catch (std::exception& exc)
                {
                    static_cast<TraversalLogger*>(m_pLogger.get())->errBuild(exc.what(), m_numVertices, curDensity);
                }
            }
            else if (m_graphType == GraphType::eHilbert)
            {
                bool isConnected = false;
                int edgesNum;
                while (!isConnected)
                {
                    try
                    {
                        m_graph = hilbert_graph(m_numVertices, curDensity, edgesNum);
                        logDenisty = double(2*edgesNum)/(m_numVertices*(m_numVertices-1));
                    }
                    catch (std::exception& exc)
                    {
                        static_cast<TraversalLogger*>(m_pLogger.get())->errBuild(exc.what(), m_numVertices, curDensity);
                    }
                    try
                    {
                        isConnected = Traverser::checkConnected(m_graph);
                    }
                    catch (std::exception& exc)
                    {
                        static_cast<TraversalLogger*>(m_pLogger.get())->errSearch(exc.what(), m_numVertices, logDenisty, 0, 0, "connected check");
                    }
                }
            }                
                
            persearch = Clock::now();
            for (int searchIndex = 0; searchIndex < m_numRuns; ++searchIndex) {
                // Выполняем поиск пути и обновляем результаты
                searchPath(logDenisty);

                // Логируем результаты после каждого поиска
                logResults(graphIndex, logDenisty, searchIndex);
            }
            avg += std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - persearch).count();
            if ((graphIndex + 1) % 100 == 0) {
                std::cerr << graphIndex + 1 << " graphs processed\n";
                std::cerr << "Avg search time per " << m_numRuns << "searches = " << avg / (100) << "[mcs] = " << avg / (100) / 1'000'000.0 << " sec" << '\n';
                std::cerr << "dt from start = " << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - begin).count() << "[mcs] = "
                            << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - begin).count() / 1'000'000.0 << " sec" << '\n';
                std::cerr << "dt from last graph iteration = " << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - iter).count() << "[mcs] = "
                            << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - iter).count() / 1'000'000.0 << " sec" << '\n';
                iter = Clock::now();
                avg = 0;
            }
            clear();
        }
        std::cerr << "\n";
    }
}

List<Node> MonteCarloTraversal::buildGraph(double density) {

    // TODO : переделать на вызов наиболее оптимального метода
    //List<Node> nodes = get_tree(numEdges);

    List<Node> nodes = transform(prufer_unpack(prufer_gen(m_numVertices), m_numVertices), m_numVertices);
    setGraphDensity(nodes, density);

    return nodes;
}

// Поиск пути на графе (в текущем графе)
void MonteCarloTraversal::searchPath(double curDensity, bool auto_inv) {

    Randomizer rand;
    Traverser traverser(&m_graph, auto_inv);

    SizeType from = rand.uRand(0, m_graph.size() - 1);
    SizeType to = from;

    while (to == from)
        to = rand.uRand(0, m_graph.size() - 1);

    try
    {
        traverser.traverse<std::queue<SizeType>>(from, to, curDensity);  // BFS
        m_bfsResults.push_back(traverser.getTraverseOrder().size());
        m_dist.push_back(traverser.getPath().size() - 1); // -1, чтобы не учитывать первую вершину
        traverser.clear();
    }
    catch (std::exception& exc)
    {
        static_cast<TraversalLogger*>(m_pLogger.get())->errSearch(exc.what(), m_numVertices, curDensity, from, to, "BFS");
        static_cast<TraversalLogger*>(m_pLogger.get())->logErrGraph(m_graph);
    }
    try
    {
        traverser.traverse<std::stack<SizeType>>(from, to, curDensity);  // DFS
        m_dfsResults.push_back(traverser.getTraverseOrder().size());
    }
    catch (std::exception& exc)
    {
        static_cast<TraversalLogger*>(m_pLogger.get())->errSearch(exc.what(), m_numVertices, curDensity, from, to, "DFS");
        static_cast<TraversalLogger*>(m_pLogger.get())->logErrGraph(m_graph);
    }
}

// Логирование результатов
void MonteCarloTraversal::logResults(int graphIndex, double density, int searchIndex) {
    static_cast<TraversalLogger*>(m_pLogger.get())->log(m_graph.size(), density, m_dist.back(), getBFSResults().back(), getDFSResults().back());
    // TODO правильное логирование с ипользование геттеров
}