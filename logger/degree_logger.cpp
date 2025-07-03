#include "logger/degree_logger.h"

void DegreeLogger::errCounting(const std::string& errTxt, SizeType graphSize, double density)
{
    m_err << "Error while counting degrees on " << graphSize
          << " vertices with density " << density << ": " << errTxt << std::endl;
}

void DegreeLogger::log(SizeType graphSize, double density, Map<SizeType, SizeType> degsCounter)
{
    m_log << graphSize << ' ' << density;
    for (auto& pair : degsCounter)
        m_log << ' ' << pair.first << ' ' << pair.second;
    m_log << std::endl;
}