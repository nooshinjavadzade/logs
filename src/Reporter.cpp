#include "Reporter.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

void Reporter::printReport(const Analyzer& analyzer) {
    cout << "=== Basic Report ===\n";
    cout << "Total Requests (Valid): " << analyzer.totalRequests << "\n";
    cout << "Broken/Invalid Lines Skipped: " << analyzer.invalidLines << "\n";
    cout << "Unique IPs: " << analyzer.uniqueIPs.size() << "\n";
    
    if (analyzer.totalRequests > 0) {
        double errorRate = (double)analyzer.errorRequests / analyzer.totalRequests * 100.0;
        cout << "Error Rate (4xx & 5xx): " << fixed << setprecision(2) << errorRate << "%\n";
    }
    
    cout << "\n=== Top 10 Endpoints ===\n";
    vector<pair<string, uint64_t>> endpoints(analyzer.endpointCounts.begin(), analyzer.endpointCounts.end());
    
    sort(endpoints.begin(), endpoints.end(), [](const pair<string, uint64_t>& a, const pair<string, uint64_t>& b) {
        return a.second > b.second;
    });
    
    int limit = min(10, (int)endpoints.size());
    for (int i = 0; i < limit; i++) {
        cout << i + 1 << ". " << endpoints[i].first << " (" << endpoints[i].second << " requests)\n";
    }
}

void Reporter::printTimeDistribution(const Analyzer& analyzer) {
    cout << "\n=== Time Distribution (Hourly) ===\n";
    
    uint64_t maxReq = 0;
    for (int i = 0; i < 24; i++) {
        if (analyzer.hourlyRequests[i] > maxReq) maxReq = analyzer.hourlyRequests[i];
    }
    
    for (int i = 0; i < 24; i++) {
        cout << setfill('0') << setw(2) << i << ":00 - " 
             << setw(2) << i << ":59 | ";
        
        cout << setfill(' ') << setw(8) << analyzer.hourlyRequests[i] << " | ";
        
        int barLength = 0;
        if (maxReq > 0) {
            barLength = (analyzer.hourlyRequests[i] * 50) / maxReq; 
        }
        cout << string(barLength, '*') << "\n";
    }
}

void Reporter::printSuspiciousActivity(const Analyzer& analyzer) {
    cout << "\n=== Suspicious Activity ===\n";
    bool found = false;
    
    for (const auto& pair : analyzer.suspiciousIPs) {
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


void Reporter::print5xxSpikeDetection(const Analyzer& analyzer) {
    cout << "\n=== Automated 5xx Error Spike Detection ===\n";
    
    uint64_t total5xx = 0;
    for (int i = 0; i < 24; i++) {
        total5xx += analyzer.hourly5xxRequests[i];
    }
    
    if (total5xx == 0) {
        cout << "No 5xx errors detected. Server stability is nominal.\n";
        return;
    }
    
    double average5xx = (double)total5xx / 24.0;
    bool spikeFound = false;
    
    for (int i = 0; i < 24; i++) {
        if (analyzer.hourly5xxRequests[i] > (average5xx * 3.0) && analyzer.hourly5xxRequests[i] > 5) {
            double hourErrorRate = 0.0;
            if (analyzer.hourlyRequests[i] > 0) {
                hourErrorRate = (double)analyzer.hourly5xxRequests[i] / analyzer.hourlyRequests[i] * 100.0;
            }
            
            cout << "[CRITICAL SPIKE] Server 5xx anomaly detected at interval: " 
                 << setfill('0') << setw(2) << i << ":00 - " << setw(2) << i << ":59\n"
                 << "  -> 5xx Count: " << analyzer.hourly5xxRequests[i] << " (Global Hourly Avg: " << fixed << setprecision(1) << average5xx << ")\n"
                 << "  -> Hourly 5xx Error Rate: " << fixed << setprecision(2) << hourErrorRate << "%\n\n";
                 
            spikeFound = true;
        }
    }
    
    if (!spikeFound) {
        cout << "5xx errors are distributed evenly. No sudden spikes or bursts detected.\n";
    }
}