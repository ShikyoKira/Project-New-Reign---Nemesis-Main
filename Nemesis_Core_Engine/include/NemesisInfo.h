#pragma once

#include <filesystem>
#include <mutex>

#include "Utilities/Algorithm.h"

#include "Enums/HavokVersion.h"
#include "Enums/PlatformType.h"

struct NemesisInfo
{
private:
    static NemesisInfo* instance;

    std::filesystem::path data_path;
    std::filesystem::path stage_path;
    std::filesystem::path exe_path;
    std::filesystem::path exe_dir_path;
    std::filesystem::path working_directory;

    bool debug = false;
    bool progress_indicator_active = false;

    nemesis::PlatformType out_platform = nemesis::PlatformType::WIN32;
    nemesis::HavokVersion out_version = nemesis::HavokVersion::HK_2010_2_0;

    bool is_async = true;

    static const std::filesystem::path& CanonizePath(const std::filesystem::path& path);

public:
    static int Setup(int argc, wchar_t* argv[], VecStr& mods);
    static std::wstring GetVersion();
    static std::string GetVersionA();
    static nemesis::PlatformType OutputPlatform();
    static nemesis::PlatformType OutputPlatform(nemesis::PlatformType out_platform);
    static nemesis::HavokVersion OutputVersion();
    static nemesis::HavokVersion OutputVersion(nemesis::HavokVersion out_version);
    static const std::filesystem::path& ExePath();
    static const std::filesystem::path& ExePath(const std::filesystem::path& exe_path);
    static const std::filesystem::path& ExeDirectory();
    static const std::filesystem::path& WorkingDirectory();
    static const std::filesystem::path& WorkingDirectory(const std::filesystem::path& working_directory);
    static const std::filesystem::path& DataPath();
    static const std::filesystem::path& DataPath(const std::filesystem::path& stage_path);
    static const std::filesystem::path& StagePath();
    static const std::filesystem::path& StagePath(const std::filesystem::path& stage_path);
    static std::filesystem::path PatchOutputPath(const std::filesystem::path& filepath);
    static bool IsAsync();
    static bool IsDebug();
    static bool IsProgressIndicatorActive();
};
