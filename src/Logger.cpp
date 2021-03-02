#include <iomanip>
#include <iostream>
#include <chrono>
#include <Logger.h>

vector<string> Logger::levels = { "DEBUG", "INFO",
                                  "WARNING", "ERROR",
                                  "CRITICAL" };
int Logger::level;

void Logger::setLevel(string _level)
{
    auto iter = find(levels.begin(), levels.end(), _level);

    if (iter != levels.end())
        Logger::level = distance(levels.begin(), iter);
    else
        throw std::runtime_error("Logger::invalid level:" + _level);
}

void Logger::printCurTime()
{
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = std::chrono::system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::cout << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
    std::cout << '.' << std::setfill('0') << std::setw(3) << ms.count();
}

void Logger::debug(string message)
{
    if (level <= 0) {
        printCurTime();
        std::cout << " - DEBUG: " << message << std::endl;
    }
}

void Logger::info(string message)
{
    if (level <= 1) {
        printCurTime();
        std::cout << " - INFO: " << message << std::endl;
    }
}

void Logger::warning(string message)
{
    if (level <= 2) {
        printCurTime();
        std::cout << " - WARNING: " << message << std::endl;
    }
}

void Logger::error(string message)
{
    if (level <= 3) {
        printCurTime();
        std::cout << " - ERROR: " << message << std::endl;
    }
}

void Logger::critical(string message)
{
    if (level <= 4) {
        printCurTime();
        std::cout << " - CRITICAL: " << message << std::endl;
    }
}

