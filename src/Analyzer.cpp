#include "Analyzer.h"

using namespace std;

Analyzer::Analyzer() {
    uniqueIPs.reserve(200000);
    suspiciousIPs.reserve(5000);
    endpointCounts.reserve(1000);
}

uint32_t Analyzer::parseIpToUint32(const string& ipStr) {
    uint32_t res = 0;
    uint32_t byte = 0;
    
    for (char c : ipStr) {
        if (c == '.') {
            res = (res << 8) | byte;
            byte = 0;
        } else if (c >= '0' && c <= '9') {
            byte = byte * 10 + (c - '0');
        }
    }
    res = (res << 8) | byte;
    return res;
}

void Analyzer::processEntry(const LogEntry& entry) {
    totalRequests++;
    
    uint32_t ipNum = parseIpToUint32(entry.ip);
    uniqueIPs.insert(ipNum);
    
    endpointCounts[entry.endpoint]++;
    
    if (entry.status >= 400 && entry.status < 600) {
        errorRequests++;
        if (entry.hour >= 0 && entry.hour < 24) {
        hourly5xxRequests[entry.hour]++; 
    }
    }
    
    if (entry.hour >= 0 && entry.hour < 24) {
        hourlyRequests[entry.hour]++;
    }
    
    if (entry.status == 401 && entry.endpoint.find("/login") != string::npos) {
        suspiciousIPs[ipNum]++;
    }
}

void Analyzer::processInvalidLine() {
    invalidLines++;
}