#pragma once
#include "LogEntry.h"

class LogParser
{
public:
    bool parse(const string& line, LogEntry& entry);
};