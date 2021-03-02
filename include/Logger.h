#pragma once
#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;
using std::find;

class Logger
{
    private:
        static int level;
        static vector<string> levels;

    public:
        static void setLevel(string message);
        static void printCurTime();
        static void debug(string message);
        static void info(string message);
        static void warning(string message);
        static void error(string message);
        static void critical(string message);
};

