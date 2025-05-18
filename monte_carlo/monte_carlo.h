#pragma once
#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H

#include <memory>

#include "common/common.h"
#include "logger/logger.h"

class MonteCarlo
{
public:
    MonteCarlo(int numGraphs, int numVertices, int numRuns, const List<double>& densities, std::unique_ptr<Logger>&& pLogger)
        : m_numGraphs(numGraphs), m_numVertices(numVertices), m_numRuns(numRuns),
        m_densities(densities), m_pLogger(std::move(pLogger))
    {}

    void initHilbert();
    void initErdosRenyi();

    virtual void runMonteCarlo() = 0;
    virtual void clear() = 0;
protected:
    // Метод для построения графа
    void buildGraph(double density);

    virtual void logResults() = 0;

    int m_numGraphs;                      // Количество графов
    int m_numVertices;                    // Количество вершин в графе
    int m_numRuns;                        // Количество экспериментов на каждом графе

    enum GraphType
    {
        eNone,
        eHilbert,
        eErdosRenyi
    };

    List<double> m_densities;
    double m_curDensity;

    GraphType m_graphType;
    List<Node> m_graph;

    std::unique_ptr<Logger> m_pLogger;
};

#endif // MONTE_CARLO_H