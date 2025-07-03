#pragma once
#ifndef DEGREE_LOGGER_H
#define DEGREE_LOGGER_H

#include "logger/logger.h"

class DegreeLogger : public Logger
{
public:
    DegreeLogger(const std::string& log, const std::string& err, SizeType threadIdx)
        : Logger(log, err, threadIdx)
    {};

    DegreeLogger(const DegreeLogger& other) = delete;
    DegreeLogger(DegreeLogger&& other) = default;

    void errCounting(const std::string& errTxt, SizeType graphSize, double density);
    void log(SizeType graphSize, double density, Map<SizeType, SizeType> degsCounter);
};

#endif // DEGREE_LOGGER_H
