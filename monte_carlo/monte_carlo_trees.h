#pragma once

#include "monte_carlo.h"
#include <memory>
#include "common/common.h"
#include "graph/tree/rand_tree.h"
#include "graph/traversal/traversal.h"
#include "logger/logger.h"
#include "tree/spanning_tree.h"
#include <string>
#include <fstream>

class MonteCarloTrees : public MonteCarlo {
public:
    MonteCarloTrees(int numGraphs, int numVertices, int numRuns, const List<double>& densities, std::unique_ptr<TraversalLogger>&& pLogger)
        : MonteCarlo(numGraphs, numVertices, numRuns,  densities, std::move(pLogger))
    {}

    virtual void runMonteCarlo() override;
    virtual void clear() override;

    // Геттеры для результатов
    const List<int>& getPrimSpanResults() const;
    const List<int>& getKruskalSpanResults() const;

private:

    List<Node> m_graph;;            // Граф
    List<WNode> m_wgraph;;           // Граф с весами

    List<int> m_primSpanResults;     // Результаты поиска в ширину
    List<int> m_kruskalSpanResults;  // Результаты поиска в глубину
    Spanner m_spanner;
    List<Node> buildGraph(double density);
    void makeTree();
    List<WNode> enrichmentGraph();
    void logResults(int graphIndex, double density, int searchIndex);
};