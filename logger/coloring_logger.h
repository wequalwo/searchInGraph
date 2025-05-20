#pragma once
#ifndef COLORING_LOGGER_H
#define COLORING_LOGGER_H

#include "logger/logger.h"

class ColoringLogger : public Logger
{
public:
    ColoringLogger(const std::string& log, const std::string& err, SizeType threadIdx)
        : Logger(log, err, threadIdx)
    {};

    ColoringLogger(const ColoringLogger& other) = delete;
    ColoringLogger(ColoringLogger&& other) = default;

    void errColoring(const std::string& errTxt, SizeType graphSize, double density, const std::string& coloring);
    void log(SizeType graphSize, double density, SizeType naive, SizeType greedy);
};

#endif // COLORING_LOGGER_H
