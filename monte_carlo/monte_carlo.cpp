#include "monte_carlo.h"
#include "graph/rand_graph.h"
#include "prufer_graph/prufer.h"
#include "prufer_graph/random_graph.h"

MonteCarlo::MonteCarlo(int numVertices, int numSearches, Logger& log)
    : m_numVertices(numVertices), m_numSearches(numSearches), m_logger(log)
{}

void MonteCarlo::clear() {
    m_graph.clear();
    m_bfsResults.clear();
    m_dfsResults.clear();
    m_dist.clear();
}

const List<int>& MonteCarlo::getBFSResults() const {
    return m_bfsResults;
}

const List<int>& MonteCarlo::getDFSResults() const {
    return m_dfsResults;
}

// Инициализация алгоритма
void MonteCarlo::initErdosRenyi(const List<double>& densities, int numGraphs) {
    using Clock = std::chrono::steady_clock;
    std::cerr << "starting ER process on " + std::to_string(densities.size()) + " densities with "
                 + std::to_string(numGraphs) + " vertices each\n";
    Clock::time_point begin = Clock::now();
    Clock::time_point iter = begin;
    Clock::time_point persearch = begin;
    float avg = 0;
    //Clock::time_point end = iter;

    for (double curDensity : densities)
    {
        std::cerr << "density: " << curDensity << "\n";
        iter = Clock::now();
        avg = 0;
        for (int graphIndex = 0; graphIndex < numGraphs; ++graphIndex)
        {

            // TODO разделить методы: надо получать не только эти данные
            try
            {
                m_graph = buildGraph(m_numVertices, curDensity);
            }
            catch (std::exception& exc)
            {
                m_logger.errBuild(exc.what(), m_numVertices, curDensity);
            }

            persearch = Clock::now();
            for (int searchIndex = 0; searchIndex < m_numSearches; ++searchIndex) {
                // Выполняем поиск пути и обновляем результаты
                searchPath(curDensity);

                // Логируем результаты после каждого поиска
                logResults(graphIndex, curDensity, searchIndex);
            }
            avg += std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - persearch).count();
            if ((graphIndex + 1) % 100 == 0) {
                std::cerr << graphIndex + 1 << " graphs processed\n";
                std::cerr << "Avg search time per " << m_numSearches << "searches = " << avg / (100) << "[mcs] = " << avg / (100) / 1'000'000.0 << " sec" << '\n';
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

void MonteCarlo::initHilbert(int numGraphs) {
    using Clock = std::chrono::steady_clock;
    std::cerr << "starting Hilbert process on " + std::to_string(numGraphs) + " graphs\n";
    Clock::time_point begin = Clock::now();
    Clock::time_point iter = begin;
    Clock::time_point persearch = begin;
    float avg = 0;
    //Clock::time_point end = iter;

    for (int graphIndex = 0; graphIndex < numGraphs; ++graphIndex)
    {
        List<Node> graph;
        bool isConnected = false;
        while (!isConnected)
        {
            try
            {
                m_graph = hilbert_graph(m_numVertices);
            }
            catch (std::exception& exc)
            {
                m_logger.errBuild(exc.what(), m_numVertices, 0.5);
            }
            try
            {
                isConnected = Traverser::checkConnected(m_graph);
            }
            catch (std::exception& exc)
            {
                m_logger.errSearch(exc.what(), m_numVertices, 0.5, 0, 0, "connected check");
            }
        }

        persearch = Clock::now();
        for (int searchIndex = 0; searchIndex < m_numSearches; ++searchIndex) {
            // Выполняем поиск пути и обновляем результаты
            // костыль: пока что граф в любом случае на списках смежности, поэтому считаем плотность 0
            searchPath(0);

            // Логируем результаты после каждого поиска
            logResults(graphIndex, 0, searchIndex);
        }
        avg += std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - persearch).count();
        if ((graphIndex + 1) % 100 == 0) {
            std::cerr << graphIndex + 1 << " graphs processed\n";
            std::cerr << "Avg search time per " << m_numSearches << "searches = " << avg / (100) << "[mcs] = " << avg / (100) / 1'000'000.0 << " sec" << '\n';
            std::cerr << "dt from start = " << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - begin).count() << "[mcs] = "
                        << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - begin).count() / 1'000'000.0 << " sec" << '\n';
            std::cerr << "dt from last graph iteration = " << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - iter).count() << "[mcs] = "
                        << std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - iter).count() / 1'000'000.0 << " sec" << '\n';
            iter = Clock::now();
            avg = 0;
        }
        clear();
    }
}

List<Node> MonteCarlo::buildGraph(int numEdges, double density) {

    // TODO : переделать на вызов наиболее оптимального метода
    //List<Node> nodes = get_tree(numEdges);

    List<Node> nodes = transform(prufer_unpack(prufer_gen(numEdges), numEdges), numEdges);
    setGraphDensity(nodes, density);

    return nodes;
}

// Поиск пути на графе (в текущем графе)
void MonteCarlo::searchPath(double curDensity) {

    Randomizer rand;
    Traverser traverser(&m_graph);

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
        m_logger.errSearch(exc.what(), m_numVertices, curDensity, from, to, "BFS");
        m_logger.logErrGraph(m_graph);
    }

    try
    {
        traverser.traverse<std::stack<SizeType>>(from, to, curDensity);  // DFS
        m_dfsResults.push_back(traverser.getTraverseOrder().size());
    }
    catch (std::exception& exc)
    {
        m_logger.errSearch(exc.what(), m_numVertices, curDensity, from, to, "DFS");
        m_logger.logErrGraph(m_graph);
    }
}

// Логирование результатов
void MonteCarlo::logResults(int graphIndex, double density, int searchIndex) {
    m_logger.log(m_graph.size(), density, m_dist.back(), getBFSResults().back(), getDFSResults().back());
    // TODO правильное логирование с ипользование геттеров
}