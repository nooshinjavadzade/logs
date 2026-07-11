#include "Analyzer.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

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

void Analyzer::printReport() const {
    cout << "=== Basic Report ===\n";
    cout << "Total Requests (Valid): " << totalRequests << "\n";
    cout << "Broken/Invalid Lines Skipped: " << invalidLines << "\n";
    cout << "Unique IPs: " << uniqueIPs.size() << "\n";
    
    if (totalRequests > 0) {
        double errorRate = (double)errorRequests / totalRequests * 100.0;
        cout << "Error Rate (4xx & 5xx): " << fixed << setprecision(2) << errorRate << "%\n";
    }
    
    cout << "\n=== Top 10 Endpoints ===\n";
    vector<pair<string, uint64_t>> endpoints(endpointCounts.begin(), endpointCounts.end());
    
    sort(endpoints.begin(), endpoints.end(), [](const pair<string, uint64_t>& a, const pair<string, uint64_t>& b) {
        return a.second > b.second;
    });
    
    int limit = min(10, (int)endpoints.size());
    for (int i = 0; i < limit; i++) {
        cout << i + 1 << ". " << endpoints[i].first << " (" << endpoints[i].second << " requests)\n";
    }
}

void Analyzer::printTimeDistribution() const {
    cout << "\n=== Time Distribution (Hourly) ===\n";
    
    uint64_t maxReq = 0;
    for (int i = 0; i < 24; i++) {
        if (hourlyRequests[i] > maxReq) maxReq = hourlyRequests[i];
    }
    
    for (int i = 0; i < 24; i++) {
        cout << setfill('0') << setw(2) << i << ":00 - " 
             << setw(2) << i << ":59 | ";
        
        cout << setfill(' ') << setw(8) << hourlyRequests[i] << " | ";
        
        int barLength = 0;
        if (maxReq > 0) {
            barLength = (hourlyRequests[i] * 50) / maxReq; 
        }
        cout << string(barLength, '*') << "\n";
    }
}

void Analyzer::printSuspiciousActivity() const {
    cout << "\n=== Suspicious Activity ===\n";
    bool found = false;
    
    for (const auto& pair : suspiciousIPs) {
        if (pair.second > 10) { 
            uint32_t ip = pair.first;
            string ipStr = to_string((ip >> 24) & 0xFF) + "." +
                           to_string((ip >> 16) & 0xFF) + "." +
                           to_string((ip >> 8) & 0xFF) + "." +
                           to_string(ip & 0xFF);
                           
            cout << "Suspicious IP: " << ipStr << " had " << pair.second << " failed login attempts (401s).\n";
            found = true;
        }
    }
    if (!found) {
        cout << "No suspicious brute-force login attempts detected.\n";
    }
}