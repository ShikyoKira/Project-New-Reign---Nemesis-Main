#pragma once

#include <mutex>
#include <filesystem>


struct Logger
{
    static std::mutex LoggerMutex;
    static std::filesystem::path LoggerPath;

    static void SetPath(const std::filesystem::path& path);
    static void Log(const std::string& msg, bool console_out = false);
    static void Log(const std::wstring& msg, bool console_out = false);
    static void ClearLog();
};
