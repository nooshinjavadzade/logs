#pragma once
#include "LogEntry.h"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <cstdint> 
#include <chrono>

class Analyzer {
public:
    Analyzer();

    void processEntry(const LogEntry& entry);
    void processInvalidLine();
    void startClock();
    void stopClock();


    friend class Reporter;

private:
    uint64_t totalRequests = 0;
    uint64_t invalidLines = 0;
    uint64_t errorRequests = 0; 
    double executionTimeMs = 0.0;
    std::unordered_set<uint32_t> uniqueIPs;
    std::unordered_map<uint32_t, uint64_t> suspiciousIPs;
    std::unordered_map<std::string, uint64_t> endpointCounts;
    static uint32_t parseIpToUint32(const std::string& ipStr);
    uint64_t hourlyRequests[24] = {0};
    uint64_t hourly5xxRequests[24] = {0};
    std::chrono::high_resolution_clock::time_point startTimePoint;
    
};