#include "monte_carlo.h"
#include "monte_carlo_trees.h"
#include "graph/rand_graph.h"
#include "prufer_graph/prufer.h"
#include "prufer_graph/random_graph.h"
#include "rand_weight.h"
#include <chrono>
#include <tree/spanning_tree.h>

void MonteCarloTrees::clear() {
    m_wgraph.clear();
    m_primSpanResults.clear();
    m_kruskalSpanResults.clear();
}

const List<int>& MonteCarloTrees::getPrimSpanResults() const {
    return m_primSpanResults;
}

const List<int>& MonteCarloTrees::getKruskalSpanResults() const {
    return m_kruskalSpanResults;
}

// Преобразование неориентированного графа в список WNode (весов по умолчанию)
List<WNode> toWeighted(const List<Node>& nodes, double defaultWeight = 1.0) {
    SizeType n = nodes.size();
    List<WNode> weightedNodes(n);

    for (SizeType u = 0; u < n; ++u) {
        weightedNodes[u].data = u;
    }

    for (SizeType u = 0; u < n; ++u) {
        for (SizeType v : nodes[u].incident) {
            if (u < v) {
                weightedNodes[u].incident[v] = defaultWeight;
                weightedNodes[v].incident[u] = defaultWeight;
            }
        }
    }

    return weightedNodes;
}
// TODO:
//i bet we should move this method to the base class
List<Node> MonteCarloTrees::buildGraph(double density) {

    // TODO : переделать на вызов наиболее оптимального метода
    //List<Node> nodes = get_tree(numEdges);

    List<Node> nodes = transform(prufer_unpack(prufer_gen(m_numVertices), m_numVertices), m_numVertices);
    setGraphDensity(nodes, density);

    return nodes;
}


List<WNode> MonteCarloTrees::enrichmentGraph() {

    SizeType edgeCount = 0;
    for (const auto& node : m_graph) {
        edgeCount += node.incident.size();
    }
    edgeCount /= 2;

    List<double> weights = normal_weights(10.0, 2.0, edgeCount);

    List<WNode> WGraph(m_graph.size());
    WGraph = to_rand_wgraph(m_graph, weights);

    // for (auto& node : WGraph) {
    //     for (const auto& [v, w] : node.incident) {
    //         if (node.data < v) {
    //             std::cout << node.data << " -- " << v << " [weight=" << w << "]\n";
    //         }
    //     }
    // }
    return WGraph;
}

// Логирование результатов
void MonteCarloTrees::logResults(int graphIndex, double density, int trialIndex) {
    static_cast<TraversalLogger*>(m_pLogger.get())->log(m_wgraph.size(), density, trialIndex, getKruskalSpanResults().back(), getPrimSpanResults().back());
}

void MonteCarloTrees::makeTree() {
    try {
        m_wgraph = enrichmentGraph(); 
        // TODO: make recalculation of weights
        // instead of recalculating the whole graph

        SizeType wastePrim = 0, wasteKruskal = 0;
        m_spanner.primSpan(m_wgraph, wastePrim);
        m_primSpanResults.push_back(wastePrim);                

        m_spanner.kruskalSpan(m_wgraph, wasteKruskal); 
        m_kruskalSpanResults.push_back(wasteKruskal);
    }
    catch (const std::exception& e) {
        static_cast<TraversalLogger*>(m_pLogger.get())->errSearch(e.what(), m_numVertices, 0, 0, 0, "makeTree");
    }
}

void MonteCarloTrees::runMonteCarlo() {

    if (m_graphType == GraphType::eNone)
        throw std::logic_error("monte carlo traverse experiment not inited");

    using Clock = std::chrono::steady_clock;
    std::cerr << "starting SPANNING TREE process on " + std::to_string(m_densities.size()) + " densities with "
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
                    m_graph = buildGraph(curDensity);
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
            for (int trialIndex = 0; trialIndex < m_numRuns; ++trialIndex) {

                makeTree(); // здесь зашито обогащение графа весами!

                logResults(graphIndex, logDenisty, trialIndex);
            }
            avg += std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - persearch).count();
            if ((graphIndex + 1) % 100 == 0) {
                std::cerr << graphIndex + 1 << " graphs processed\n";
                std::cerr << "Avg span time per " << m_numRuns << "trials = " << avg / (100) << "[mcs] = " << avg / (100) / 1'000'000.0 << " sec" << '\n';
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