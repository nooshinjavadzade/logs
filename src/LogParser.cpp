#include "LogParser.h"
#include <string>

bool LogParser::parse(const std::string& line, LogEntry& entry)
{
    size_t ipEnd = line.find(' ');
    if (ipEnd == std::string::npos) return false;
    entry.ip = line.substr(0, ipEnd);

    size_t timeStart = line.find('[');
    size_t timeEnd = line.find(']', timeStart);
    if (timeStart == std::string::npos || timeEnd == std::string::npos) return false;
    std::string timeStr = line.substr(timeStart + 1, timeEnd - timeStart - 1);
    
    size_t firstColon = timeStr.find(':');
    if (firstColon == std::string::npos || firstColon + 3 > timeStr.size()) return false;
    entry.hour = std::stoi(timeStr.substr(firstColon + 1, 2));

    size_t reqStart = line.find('"');
    size_t reqEnd = line.find('"', reqStart + 1);
    if (reqStart == std::string::npos || reqEnd == std::string::npos) return false;
    
    std::string reqStr = line.substr(reqStart + 1, reqEnd - reqStart - 1);
    
    size_t firstSpace = reqStr.find(' ');
    size_t lastSpace = reqStr.rfind(' ');
    if (firstSpace == std::string::npos || lastSpace == std::string::npos || firstSpace == lastSpace) return false;
    
    entry.endpoint = reqStr.substr(firstSpace + 1, lastSpace - firstSpace - 1);

    if (reqEnd + 2 >= line.size()) return false;
    size_t statusEnd = line.find(' ', reqEnd + 2);
    if (statusEnd == std::string::npos) return false;
    
    std::string statusStr = line.substr(reqEnd + 2, statusEnd - (reqEnd + 2));
    entry.status = std::stoi(statusStr);

    return true;
}