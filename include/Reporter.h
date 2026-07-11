#pragma once
#include "Analyzer.h"

class Reporter {
public:
    static void printReport(const Analyzer& analyzer);
    static void printTimeDistribution(const Analyzer& analyzer);
    static void printSuspiciousActivity(const Analyzer& analyzer);
    static void print5xxSpikeDetection(const Analyzer& analyzer);
};