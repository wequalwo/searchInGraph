#include "monte_carlo/monte_carlo.h"
#include "graph/rand_graph.h"
#include "prufer_graph/random_graph.h"
#include "prufer_graph/prufer.h"
#include "graph/traversal/traversal.h"

void MonteCarlo::initHilbert()
{
    m_graphType = GraphType::eHilbert;
}

void MonteCarlo::initErdosRenyi()
{
    m_graphType = GraphType::eHilbert;
}

void MonteCarlo::buildGraph(double density) {

    if (m_graphType == GraphType::eErdosRenyi)
    {
        try
        {
            m_graph = transform(prufer_unpack(prufer_gen(m_numVertices), m_numVertices), m_numVertices);
            setGraphDensity(m_graph, density);
            m_curDensity = density;
        }
        catch (std::exception& exc)
        {
            m_pLogger->errBuild(exc.what(), m_numVertices, density);
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
                m_graph = hilbert_graph(m_numVertices, density, edgesNum);
                m_curDensity = double(2*edgesNum)/(m_numVertices*(m_numVertices-1));
            }
            catch (std::exception& exc)
            {
                m_pLogger->errBuild(exc.what(), m_numVertices, m_curDensity);
            }
            try
            {
                isConnected = Traverser::checkConnected(m_graph);
            }
            catch (std::exception& exc)
            {
                m_pLogger->errConnectionCheck(m_numVertices, m_curDensity);
            }
        }
    }
}
