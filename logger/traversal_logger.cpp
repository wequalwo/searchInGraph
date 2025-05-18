#include "logger/traversal_logger.h"

void TraversalLogger::errSearch(const std::string& errTxt, SizeType graphSize, double density, SizeType from, SizeType to,
                       const std::string& searchType)
{
    m_err << "Error while " << searchType << " searching graph on " << graphSize
          << " vertices from " << from << " to " << to << " with density " << density << ": " << errTxt << std::endl;
}

void TraversalLogger::log(SizeType graphSize, double density, SizeType dist, SizeType bfs, SizeType dfs)
{
    // пока лог упоротый, но зато отдельные части независимы
    m_log << graphSize << ' ' << density << ' ' << dist << ' ' << bfs << ' ' << dfs << std::endl;
}