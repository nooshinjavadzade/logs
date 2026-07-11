#include "LogParser.h"
bool LogParser::parse(const string& line, LogEntry& entry)
{
    size_t pos = line.find(' ');

    if (pos == string::npos)
        return false;

    entry.ip = line.substr(0, pos);

    return true;
}