#pragma once

#include "monte_carlo/monte_carlo.h"
#include "logger/spanning_logger.h"

class MonteCarloTrees : public MonteCarlo {
public:
    MonteCarloTrees(int numGraphs, int numVertices, int numRuns, const List<double>& densities, std::unique_ptr<SpanningLogger>&& pLogger)
        : MonteCarlo(numGraphs, numVertices, numRuns,  densities, std::move(pLogger))
    {}

    virtual void runMonteCarlo() override;
    virtual void clear() override;

    // Геттеры для результатов
    const List<uint32_t>& getPrimSpanResults() const;
    const List<uint32_t>& getKruskalSpanResults() const;

private:

    List<WNode> m_wgraph;     // Граф с весами

    List<uint32_t> m_primSpanResults;     // Результаты поиска в ширину
    List<uint32_t> m_kruskalSpanResults;  // Результаты поиска в глубину
    Spanner m_spanner;
    void makeTree();
    List<WNode> enrichmentGraph();
    void logResults() override;
};