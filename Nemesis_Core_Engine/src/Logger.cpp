#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Logger.h"
#include "NemesisInfo.h"

#include "Utilities/Algorithm.h"
#include "Utilities/FileWriter.h"

std::mutex Logger::LoggerMutex;
std::filesystem::path Logger::LoggerPath = std::filesystem::current_path() / "log.txt";

void Logger::SetPath(const std::filesystem::path& path)
{
    std::filesystem::create_directories(path.parent_path());
    LoggerPath = path;
}

void Logger::Log(const std::string& msg, bool console_out)
{
    auto now        = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto now_ms     = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::tm local_time;
    localtime_s(&local_time, &time_t_now);

    std::stringstream ss;
    ss << std::put_time(&local_time, "[%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill('0')
       << now_ms.count() << "] " << msg;

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

    throw std::runtime_error("Failed to open log file (File: " + nemesis::to_utf8_string(LoggerPath) + ")");
}

void Logger::Log(const std::wstring& msg, bool console_out)
{
    auto now        = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    auto now_ms     = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::tm local_time;
    localtime_s(&local_time, &time_t_now);

    std::wstringstream ss;
    ss << std::put_time(&local_time, L"[%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill(L'0')
       << now_ms.count() << L"] " << msg;

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

    throw std::runtime_error("Failed to open log file (File: " + nemesis::to_utf8_string(LoggerPath) + ")");
}

void Logger::ClearLog()
{
    std::scoped_lock<std::mutex> lock(LoggerMutex);
    std::filesystem::remove(std::filesystem::weakly_canonical(LoggerPath));
    std::filesystem::remove(LoggerPath);
}
