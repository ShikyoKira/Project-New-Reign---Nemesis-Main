#include <chrono>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Logger.h"
#include "NemesisInfo.h"

#include "Utilities/Algorithm.h"
#include "Utilities/Exception.h"

std::mutex Logger::LoggerMutex;
std::filesystem::path Logger::LoggerPath = std::filesystem::current_path() / "log.txt";

void Logger::SetPath(const std::filesystem::path& path)
{
    std::filesystem::create_directories(path.parent_path());
    LoggerPath = path;
}

void Logger::Log(const std::string& msg, bool console_out)
{
    std::stringstream ss;
    ss << "[" << std::format("{:%Y-%m-%d %T}", std::chrono::system_clock::now()).substr(0, 23) << "] " << msg;

    std::scoped_lock<std::mutex> lock(LoggerMutex);

    if (console_out)
    {
        std::cout << msg << std::endl;
    }

    std::ofstream writer(LoggerPath, std::ios::app);

    if (writer.is_open())
    {
        writer << ss.str() + "\n";
        return;
    }

    throw nemesis::Exception("Failed to open log file (File: " + nemesis::to_utf8_string(LoggerPath) + ")");
}

void Logger::Log(const std::wstring& msg, bool console_out)
{
    std::wstringstream ss;
    ss << L"[" << std::format(L"{:%Y-%m-%d %T}", std::chrono::system_clock::now()).substr(0, 23) << L"] "
       << msg;

    std::scoped_lock<std::mutex> lock(LoggerMutex);

    if (console_out)
    {
        std::wcout << msg << std::endl;
    }

    std::wofstream writer(LoggerPath, std::ios::app);

    if (writer.is_open())
    {
        writer << ss.str() + L"\n";
        return;
    }

    throw nemesis::Exception("Failed to open log file (File: " + nemesis::to_utf8_string(LoggerPath) + ")");
}

void Logger::ClearLog()
{
    std::scoped_lock<std::mutex> lock(LoggerMutex);
    std::filesystem::remove(std::filesystem::weakly_canonical(LoggerPath));
    std::filesystem::remove(LoggerPath);
}
