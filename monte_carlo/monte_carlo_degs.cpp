
#include "monte_carlo/monte_carlo_degs.h"


void MonteCarloDegs::runMonteCarlo() {
    if (m_graphType == GraphType::eNone)
        throw std::logic_error("monte carlo coloring experiment not inited");

    for (double curDensity : m_densities)
        for (int graphIndex = 0; graphIndex < m_numGraphs; ++graphIndex)
        {
            buildGraph(curDensity);
            try
            {              
                m_degsCounter = countDegrees(m_graph);
            }
            catch(const std::exception& e)
            {
                static_cast<DegreeLogger*>(m_pLogger.get())->errCounting(e.what(), m_graph.size(), m_curDensity);
            }

            logResults();
            clear();
        }
}

void MonteCarloDegs::clear() {
    m_degsCounter.clear();
}

// Логирование результатов
void MonteCarloDegs::logResults() {
    static_cast<DegreeLogger*>(m_pLogger.get())->log(m_graph.size(), m_curDensity, m_degsCounter);
}


Map<SizeType, SizeType> MonteCarloDegs::countDegrees(const List<Node>& graph) 
{
    Map<SizeType, SizeType> degs;
    for (const auto& node : graph)
        ++degs[node.incident.size()];
    return degs;
}