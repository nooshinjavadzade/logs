#include <iostream>
#include <fstream>
#include <string>
#include "Analyzer.h"
#include "Reporter.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Error: No log file provided.\n";
        cerr << "Usage: " << argv[0] << " <path_to_log_file> [-n <top_n>] [--start <0-23>] [--end <0-23>]\n";
        return 1;
    }

    string filePath = argv[1];
    int topN = 10;
    bool hasTopNCommand = false;
    int startHour = 0;
    int endHour = 23;

    for (int i = 2; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-n" && i + 1 < argc) {
            topN = stoi(argv[++i]);
            hasTopNCommand = true;
        } else if (arg == "--start" && i + 1 < argc) {
            startHour = stoi(argv[++i]);
        } else if (arg == "--end" && i + 1 < argc) {
            endHour = stoi(argv[++i]);
        }
    }

    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << "\n";
        return 1;
    }

    Analyzer analyzer;
    analyzer.startClock();

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        bool isValid = true;
        LogEntry entry;
        

        if (isValid) {
            if (entry.hour >= startHour && entry.hour <= endHour) {
                analyzer.processEntry(entry);
            }
        } else {
            analyzer.processInvalidLine();
        }
    }
    file.close();

    analyzer.stopClock();

    if (hasTopNCommand) {
        Reporter::printReport(analyzer, topN);
    } else {
        Reporter::printReport(analyzer);
    }

    Reporter::printTimeDistribution(analyzer);
    Reporter::printSuspiciousActivity(analyzer);
    Reporter::print5xxSpikeDetection(analyzer);
    Reporter::printBenchmark(analyzer);

    return 0;
}