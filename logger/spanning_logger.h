#pragma once
#ifndef SPANNING_LOGGER_H
#define SPANNING_LOGGER_H

#include "logger/logger.h"

class SpanningLogger : public Logger
{
public:
    SpanningLogger(const std::string& log, const std::string& err, SizeType threadIdx)
        : Logger(log, err, threadIdx)
    {};

    SpanningLogger(const SpanningLogger& other) = delete;
    SpanningLogger(SpanningLogger&& other) = default;

    void errSpanning(const std::string& errTxt, SizeType graphSize, double density, const std::string& searchType);
    void log(SizeType graphSize, double density, uint32_t kruskal, uint32_t prim);
};

#endif // SPANNING_LOGGER_H
