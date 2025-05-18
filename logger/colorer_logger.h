#pragma once
#ifndef COLORER_LOGGER_H
#define COLORER_LOGGER_H

#include "logger/logger.h"

class ColorerLogger : public Logger
{
public:
    ColorerLogger(const std::string& log, const std::string& err, SizeType threadIdx)
        : Logger(log, err, threadIdx)
    {};

    ColorerLogger(const ColorerLogger& other) = delete;
    ColorerLogger(ColorerLogger&& other) = default;

    void errSearch(const std::string& errTxt, SizeType graphSize, double density, SizeType from, SizeType to,
                   const std::string& searchType);
    void log(SizeType graphSize, double density, SizeType dist, SizeType bfs, SizeType dfs);
};

#endif // COLORER_LOGGER_H