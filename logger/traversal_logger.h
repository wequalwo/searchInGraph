#pragma once
#ifndef TRAVERSAL_LOGGER_H
#define TRAVERSAL_LOGGER_H

#include "logger/logger.h"

class TraversalLogger : public Logger
{
public:
    TraversalLogger(const std::string& log, const std::string& err, SizeType threadIdx)
        : Logger(log, err, threadIdx)
    {};

    TraversalLogger(const TraversalLogger& other) = delete;
    TraversalLogger(TraversalLogger&& other) = default;

    void errSearch(const std::string& errTxt, SizeType graphSize, double density, SizeType from, SizeType to,
                   const std::string& searchType);
    void log(SizeType graphSize, double density, SizeType dist, SizeType bfs, SizeType dfs);
};

#endif // TRAVERSAL_LOGGER_H
