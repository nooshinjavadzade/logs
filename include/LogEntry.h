#pragma once
#include <string>

using namespace std;

struct LogEntry
{
    string ip;

    string method;

    string endpoint;

    int status = 0;

    long bytes = 0;

    int hour = 0;
};