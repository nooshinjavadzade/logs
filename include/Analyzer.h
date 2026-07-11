#pragma once
#include "LogEntry.h"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <cstdint> 

using namespace std;

class Analyzer {
public:
    Analyzer();

    void processEntry(const LogEntry& entry);
    void processInvalidLine();
    
    void printReport() const;
    void printTimeDistribution() const;
    void printSuspiciousActivity() const;

private:
    uint64_t totalRequests = 0;
    uint64_t invalidLines = 0;
    uint64_t errorRequests = 0; 
    unordered_set<uint32_t> uniqueIPs;
    unordered_map<uint32_t, uint64_t> suspiciousIPs;
    unordered_map<std::string, uint64_t> endpointCounts;
    uint64_t hourlyRequests[24] = {0};
    static uint32_t parseIpToUint32(const std::string& ipStr);
};