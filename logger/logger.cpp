#include <iostream>
#include <filesystem>

#include "logger.h"


Logger::Logger(const std::string& log, const std::string& err)
{
    std::filesystem::path logPath(log);
    if (std::filesystem::exists(log))
        std::cerr << "log file exists" << std::endl;
    m_log.open(logPath);
    if (!m_log.is_open()) {
        std::cerr << "Error opening log file." << std::endl;
    }
    std::filesystem::path errPath(err);
    if (std::filesystem::exists(errPath))
        std::cerr << "log file exists" << std::endl;
    m_err.open(errPath);
    if (!m_err.is_open()) {
        std::cerr << "Error opening log file." << std::endl;
    }
}

Logger::~Logger()
{
    m_log.close();
    m_err.close();
}

void Logger::errSearch(const std::string& errTxt, SizeType graphSize, double density, SizeType from, SizeType to,
                       const std::string& searchType)
{
    m_err << "Error while " << searchType << " searching graph on " << graphSize
          << " vertices from " << from << " to " << to << " with density " << density << ": " << errTxt << std::endl;
}

void Logger::logErrGraph(const List<Node>& graph)
{
    m_err << "graph representaion: <n = num of incident verts> <v1> <v2> ... <vn>" << std::endl;
    for (const Node& node : graph)
    {
        m_err << node.incident.size() << ' ';
        for (SizeType inc : node.incident)
        m_err << inc << ' ';
        m_err << std::endl;
    }
}

void Logger::errBuild(const std::string& errTxt, SizeType graphSize, double density)
{
    m_err << "Error while building graph on " << graphSize
          << " vertices with density " << density << ": " << errTxt << std::endl;
}

void Logger::log(SizeType graphSize, double density, SizeType dist, SizeType bfs, SizeType dfs)
{
    // пока лог упоротый, но зато отдельные части независимы
    m_log << graphSize << ' ' << density << ' ' << dist << ' ' << bfs << ' ' << dfs << std::endl;
}