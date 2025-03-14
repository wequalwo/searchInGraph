#pragma once
#ifndef LOGGER_H
#define LOGGER_H

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
                   const std::string& searchType);
    void logErrGraph(const List<Node>& graph);
    void errBuild(const std::string& errTxt, SizeType graphSize, double density);
    void log(SizeType graphSize, double density, SizeType dist, SizeType bfs, SizeType dfs);
private:
    std::ofstream m_log;
    std::ofstream m_err;
};


#endif // LOGGER_H