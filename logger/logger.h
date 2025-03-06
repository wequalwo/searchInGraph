#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

#include "common/common.h"
#include "graph/node.h"

class Logger
{
public:
    Logger(const std::string& log, const std::string& err);

    ~Logger();

    void errSearch(const std::string& errTxt, SizeType graphSize, double density, SizeType from, SizeType to,
                   const std::string& searchType)
    {
        m_err << "Error while " << searchType << " searching graph on " << graphSize
              << " vertices from " << from << " to " << to << " with density " << density << ": " << errTxt << std::endl;
    }

    void logErrGraph(const List<Node>& graph)
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

    void errBuild(const std::string& errTxt, SizeType graphSize, double density)
    {
        m_err << "Error while building graph on " << graphSize
              << " vertices with density " << density << ": " << errTxt << std::endl;
    }

    void log(SizeType graphSize, double density, SizeType dist, SizeType bfs, SizeType dfs)
    {
        // пока лог упоротый, но зато отдельные части независимы
        m_log << graphSize << ' ' << density << ' ' << dist << ' ' << bfs << ' ' << dfs << std::endl;
    }
private:
    std::ofstream m_log;
    std::ofstream m_err;
};

Logger::Logger(const std::string& log, const std::string& err)
{
    m_log.open(log);
    if (!m_log.is_open()) {
        std::cerr << "Error opening log file." << std::endl;
    }
    m_err.open(log);
    if (!m_err.is_open()) {
        std::cerr << "Error opening log file." << std::endl;
    }
}

Logger::~Logger()
{
    m_log.close();
    m_err.close();
}
#endif // LOGGER_H