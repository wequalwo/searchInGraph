#include <numeric>

#include "logger/coloring_logger.h"


void ColoringLogger::errColoring(const std::string& errTxt, SizeType graphSize, double density, const std::string& coloring)
{
    m_err << "Error while " << coloring << " coloring graph on " << graphSize
          << " vertices with density " << density << ": " << errTxt << std::endl;
}

void ColoringLogger::log(SizeType graphSize, double density, SizeType naive, SizeType greedy)
{
    // пока лог упоротый, но зато отдельные части независимы
    m_log << graphSize << ' ' << density << ' ' << naive << ' ' << greedy << std::endl;

}