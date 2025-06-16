#include <chrono>

#include "monte_carlo/monte_carlo_trees.h"
#include "graph/rand_graph.h"
#include "prufer_graph/prufer.h"
#include "prufer_graph/random_graph.h"
#include "graph/tree/spanning_tree.h"
#include "graph/rand_weight.h"

void MonteCarloTrees::clear() {
    m_wgraph.clear();
    m_primSpanResults.clear();
    m_kruskalSpanResults.clear();
}

const List<uint32_t>& MonteCarloTrees::getPrimSpanResults() const {
    return m_primSpanResults;
}

const List<uint32_t>& MonteCarloTrees::getKruskalSpanResults() const {
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


List<WNode> MonteCarloTrees::enrichmentGraph() {
    int edgeCount = 0;
    for (const auto& node : m_graph) {
        edgeCount += node.incident.size();
    }
    edgeCount /= 2;
    std::cout << "initing weights\n";
    //допустимые распределения:
    //List<double> weights = sample_binomial(5000, 0.5, edgeCount);
    //sample_binomial(5000, 0.5, edgeCount);
    //sample_bernulli(0.95, edgeCount);
    //ones_weights(edgeCount);
    //uniform_weights(1, 1000, edgeCount);

    
    List<WNode> WGraph(m_graph.size());
    //WGraph = to_rand_wgraph(m_graph, weights);

    WGraph = to_rand_wgraph(m_graph); // перегруженная функция без весов (встроенная генерация)


    return WGraph;
}

// Логирование результатов
void MonteCarloTrees::logResults() {
    static_cast<SpanningLogger*>(m_pLogger.get())->log(m_wgraph.size(), m_curDensity, getKruskalSpanResults().back(), getPrimSpanResults().back());
}

void MonteCarloTrees::makeTree() {
    try {
        m_wgraph = enrichmentGraph(); 
    }
    catch (const std::exception& e) {
        static_cast<SpanningLogger*>(m_pLogger.get())->errSpanning(e.what(), m_numVertices, m_curDensity, "enrich");
    }
    unsigned int wastePrim = 0, wasteKruskal = 0;
    try {
        
        m_spanner.naivePrimSpan(m_wgraph, wastePrim);
        m_primSpanResults.push_back(wastePrim);                
    }
    catch (const std::exception& e) {
        static_cast<SpanningLogger*>(m_pLogger.get())->errSpanning(e.what(), m_numVertices, m_curDensity, "prim");
    }
    try {
        m_spanner.naiveKruskalSpan(m_wgraph, wasteKruskal); 
        m_kruskalSpanResults.push_back(wasteKruskal);
    }
    catch (const std::exception& e) {
        static_cast<SpanningLogger*>(m_pLogger.get())->errSpanning(e.what(), m_numVertices, m_curDensity, "prim");
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
            buildGraph(curDensity);     
            persearch = Clock::now();
            for (int trialIndex = 0; trialIndex < m_numRuns; ++trialIndex) {

                makeTree(); // здесь зашито обогащение графа весами!

                logResults();
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