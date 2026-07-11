#pragma once
#include "LogEntry.h"
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

class Analyzer {
public:
    void processEntry(const LogEntry& entry);
    void processInvalidLine();
    
    void printReport() const;
    void printTimeDistribution() const;
    void printSuspiciousActivity() const;

private:
    long long totalRequests = 0;
    long long invalidLines = 0;
    long long errorRequests = 0; // 4xx and 5xx
    
    unordered_set<string> uniqueIPs;
    unordered_map<string, long long> endpointCounts;
    
    // Time distribution: requests per hour
    long long hourlyRequests[24] = {0};
    
    // Suspicious activity: count 401s per IP
    unordered_map<string, long long> suspiciousIPs;
};