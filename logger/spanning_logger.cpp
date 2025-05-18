#include "logger/spanning_logger.h"

void SpanningLogger::errSpanning(const std::string& errTxt, SizeType graphSize, double density, const std::string& spanType)
{
    m_err << "Error while " << spanType << " spanning graph on " << graphSize
          << " vertices with density " << density << ": " << errTxt << std::endl;
}

void SpanningLogger::log(SizeType graphSize, double density, uint32_t kruskal, uint32_t prim)
{
    // пока лог упоротый, но зато отдельные части независимы
    m_log << graphSize << ' ' << density << ' ' << kruskal << ' ' << prim << std::endl;
}