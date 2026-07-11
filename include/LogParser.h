#pragma once
#include "LogEntry.h"

class LogParser {
public:
    static bool parse(const string& line, LogEntry& entry);
};
