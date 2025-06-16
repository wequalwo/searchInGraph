#pragma once
#ifndef MONTE_CARLO_TREES
#define MONTE_CARLO_TREES

#include "monte_carlo/monte_carlo.h"
#include "logger/spanning_logger.h"
#include "graph/tree/spanning_tree.h"

class MonteCarloTrees : public MonteCarlo {
public:
    MonteCarloTrees(int numGraphs, int numVertices, int numRuns, const List<double>& densities, std::unique_ptr<SpanningLogger>&& pLogger)
        : MonteCarlo(numGraphs, numVertices, numRuns,  densities, std::move(pLogger))
    {}

    virtual void runMonteCarlo() override;
    virtual void clear() override;

    void setParaps(double theta1, double theta2) { m_theta1 = theta1; m_theta2 = theta2; }

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
    double m_theta1 = 0; // параметры распределения вершин некоторого двупараметрического распределения P(theta1, theta2)
    double m_theta2 = 10;
    void logResults() override;
};

#endif // MONTE_CARLO_TREES