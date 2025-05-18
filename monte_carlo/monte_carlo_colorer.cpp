#include <chrono>

#include "monte_carlo/monte_carlo_colorer.h"
#include "graph/coloring/colorer.h"


void MonteCarloColorer::runMonteCarlo() {
    if (m_graphType == GraphType::eNone)
        throw std::logic_error("monte carlo coloring experiment not inited");

    using Clock = std::chrono::steady_clock;
    std::cerr << "starting RANDOM COLORING process on " + std::to_string(m_densities.size()) + " densities with "
                 + std::to_string(m_numGraphs) + (m_graphType == GraphType::eErdosRenyi ? " Erdos-Renyi" : " Hilbert")
                 + " graphs each\n";
    Clock::time_point begin = Clock::now();
    Clock::time_point iter = begin;
    Clock::time_point persearch = begin;
    float avg = 0;
    for (double curDensity : m_densities)
    {
        std::cerr << std::string("density: ") + std::to_string(curDensity) + '\n';
        for (int graphIndex = 0; graphIndex < m_numGraphs; ++graphIndex)
        {
            buildGraph(curDensity);

            Colorer colorer;
            m_naiveColors = colorer.naiveColoring(m_graph);
            m_greedyColors = colorer.greedyColoring(m_graph);

            SizeType maxColorsToCheck = std::max(m_naiveColors, m_greedyColors) + 3;
            SizeType minColorsToCheck = std::min(m_naiveColors, m_greedyColors);
            if (minColorsToCheck > 4)
                minColorsToCheck -= 3;
            else
                minColorsToCheck = 2;
            
            for (SizeType curColors = maxColorsToCheck; curColors >= minColorsToCheck; --curColors)
            {
                bool foundColoring = false;
                for (int searchIndex = 0; searchIndex < m_numRuns; ++searchIndex)
                    if (colorer.randColoring(m_graph, curColors))
                    {
                        m_tries.push_back(++searchIndex);
                        foundColoring = true;
                        break;
                    }
                if (!foundColoring)
                    m_tries.push_back(0);
            }
            logResults();
            avg += std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - persearch).count();
            if ((graphIndex + 1) % 100 == 0) {
                std::cerr << graphIndex + 1 << " graphs processed\n";
                std::cerr << "Avg coloring time per " << m_numRuns << "coloring = " << avg / (100) << "[mcs] = " << avg / (100) / 1'000'000.0 << " sec" << '\n';
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

void MonteCarloColorer::clear() {
    m_graph.clear();
    m_tries.clear();
    m_greedyColors = 0;
    m_naiveColors = 0;
}

// Логирование результатов
void MonteCarloColorer::logResults() {
    static_cast<ColoringLogger*>(m_pLogger.get())->log(m_graph.size(), m_curDensity, m_naiveColors, m_greedyColors,
                                                       m_tries);
}