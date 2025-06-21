#include "NemesisInfo.h"
#include "Logger.h"

#include <functional>
#include <regex>

#include "Utilities/File.h"
#include "Utilities/FileWriter.h"

namespace sf = std::filesystem;

const std::wstring version         = L"1.0.0";
NemesisInfo* NemesisInfo::instance = new NemesisInfo();

const std::filesystem::path& NemesisInfo::CanonizePath(const std::filesystem::path& path)
{
    static Map<std::wstring, std::filesystem::path> PathCache;
    static std::mutex PathCacheMutex;

    std::scoped_lock<std::mutex> lock(PathCacheMutex);
    auto path_itr = PathCache.find(path);

    if (path_itr != PathCache.end()) return path_itr->second;

    return PathCache.insert({path, sf::absolute(path)}).first->second;
}

int NemesisInfo::Setup(int argc, wchar_t* argv[], VecStr& mods)
{
    std::wstring help_msg
        = L"Nemesis Unlimited Behavior Engine made by Shikyo Kira v" + version
          + L"\n\n"
            L"Nemesis Unlimited Behavior Engine processes Nemesis Extended XML (.nemx) and animation queries "
            L"to generate standard XML (.xml) or Havok (.hkx) binary formats, AnimationDataSingleFile and "
            L"AnimationSetDataSingleFile\n\n"
            L"Nemx files provide extended XML syntax with embedded functions and macros for dynamic content "
            L"generation and modular data building\n\n"
            L"Usage:\n"
            L"     "
          + sf::path(argv[0]).filename().wstring()
          + " -p win32 -d \"C:\\Program Files "
            L"(x86)\\steam\\steamapps\\common\\Skyrim Special Edition\\Data\" -o "
            L"\"My\\Staging\\Directory\\Path\" -m tkuc nemesis bcbi\n\n"
            L"     -h      Help info\n"
            L"     -p      Output platform [ps3, ps4, 360, win32 (default), amd64]\n"
            L"     -v      Havok version. ie: hk_2010.2.0-r1 (default)\n"
            L"     -d      Skyrim data directory\n"
            L"     -o      Output directory. Skyrim data directory is the default value (optional)\n"
            L"     -pi     Output progress indicator (optional)\n"
            L"     -db     Toggle debug mode (optional)\n"
            L"     -m      List of active mod codes. Must be the last. Priority order starts from lowest "
            L"(left) "
            L"to highest (right)\n"
            L"     -s      Enable synchronous processing for debugging purposes (default: asynchronous)\n";

    if (argc == 1)
    {
        std::wcout << help_msg << std::endl;
        return 0;
    }

    NemesisInfo::WorkingDirectory(sf::current_path());
    std::wstring platform = L"win32";
    std::wstring hk_ver   = L"hk_2010.2.0-r1";
    sf::path data_path;

    for (size_t i = 1; i < argc; i++)
    {
        auto& arg = argv[i];

        if (nemesis::iequals(arg, L"-s"))
        {
            instance->is_async = false;
            continue;
        }

        if (nemesis::iequals(arg, L"-p"))
        {
            if (i + 1 == argc) continue;

            ++i;
            platform = argv[i];
            continue;
        }

        if (nemesis::iequals(arg, L"-v"))
        {
            if (i + 1 == argc) continue;

            ++i;
            hk_ver = argv[i];
            continue;
        }

        if (nemesis::iequals(arg, L"-d"))
        {
            if (i + 1 == argc) continue;

            data_path = DataPath(argv[++i]);
            continue;
        }

        if (nemesis::iequals(arg, L"-o"))
        {
            if (i + 1 == argc) continue;

            StagePath(argv[++i]);
            continue;
        }

        if (nemesis::iequals(arg, L"-pi"))
        {
            instance->progress_indicator_active = true;
            continue;
        }

        if (nemesis::iequals(arg, L"-db"))
        {
            instance->debug = true;
            continue;
        }

        if (nemesis::iequals(arg, L"-h"))
        {
            std::wcout << help_msg << std::endl;
            return 0;
        }

        if (!nemesis::iequals(arg, L"-m")) continue;

        while (++i < argc)
        {
            mods.emplace_back(nemesis::transform_to(std::wstring(argv[i])));
        }
    }

    if (data_path.empty())
    {
        auto dir = WorkingDirectory();
        sf::path datapath;
        sf::path::iterator data_itr = datapath.end();

        for (auto itr = dir.begin(); itr != dir.end(); ++itr)
        {
            if (!nemesis::iequals(itr->filename().wstring(), L"data")) continue;

            data_itr = itr;
        }

        if (data_itr == datapath.end()) throw std::runtime_error("Data directory cannot be found");

        std::filesystem::path new_path;

        for (auto itr = dir.begin(); itr != dir.end(); ++itr)
        {
            new_path /= *itr;

            if (itr == data_itr) break;
        }

        data_path = DataPath(new_path);
    }

    NemesisInfo::ExePath(argv[0]);
    Logger::SetPath(PatchOutputPath(NemesisInfo::ExeDirectory() / Logger::LoggerPath.filename()));
    Logger::ClearLog();

    Logger::Log(L"Nemesis Version: " + version, true);
    Logger::Log(L"Log Path: " + Logger::LoggerPath.wstring());
    Logger::Log(L"Executable Path: " + NemesisInfo::ExePath().wstring());
    Logger::Log(L"Working Directory Path: " + NemesisInfo::WorkingDirectory().wstring(), true);
    Logger::Log(L"Data Directory: " + data_path.wstring(), true);
    Logger::Log(L"Staging Path: " + StagePath().wstring(), true);

    if (nemesis::iequals(platform, L"ps3"))
    {
        OutputPlatform(nemesis::PlatformType::PS3);
    }
    else if (nemesis::iequals(platform, L"ps4"))
    {
        OutputPlatform(nemesis::PlatformType::PS4);
    }
    else if (nemesis::iequals(platform, L"360"))
    {
        OutputPlatform(nemesis::PlatformType::XB360);
    }
    else if (nemesis::iequals(platform, L"amd64"))
    {
        OutputPlatform(nemesis::PlatformType::AMD64);
    }
    else if (nemesis::iequals(platform, L"win32"))
    {
        OutputPlatform(nemesis::PlatformType::WIN32);
    }
    else
    {
        throw std::runtime_error("ERROR: Unsupported platform argument (" + nemesis::transform_to(platform)
                                 + ")");
    }

    Logger::Log(std::wstring(L"Havok Version: ") + hk_ver, true);
    Logger::Log(std::wstring(L"Platform: ") + platform, true);

    try
    {
        OutputVersion(nemesis::GetEnumVersion(nemesis::transform_to(hk_ver)));
    }
    catch (const std::exception&)
    {
    }

    std::cout << "\n" << std::endl;
    return 1;
}

std::wstring NemesisInfo::GetVersion()
{
    return version;
}

std::string NemesisInfo::GetVersionA()
{
    return nemesis::transform_to(version);
}

nemesis::PlatformType NemesisInfo::OutputPlatform()
{
    return instance->out_platform;
}

nemesis::PlatformType NemesisInfo::OutputPlatform(nemesis::PlatformType out_platform)
{
    return instance->out_platform = out_platform;
}

nemesis::HavokVersion NemesisInfo::OutputVersion()
{
    return instance->out_version;
}

nemesis::HavokVersion NemesisInfo::OutputVersion(nemesis::HavokVersion out_version)
{
    return instance->out_version = out_version;
}

const sf::path& NemesisInfo::ExePath()
{
    return instance->exe_path;
}

const sf::path& NemesisInfo::ExePath(const sf::path& exe_path)
{
    instance->exe_dir_path    = exe_path.parent_path();
    return instance->exe_path = exe_path;
}

const sf::path& NemesisInfo::ExeDirectory()
{
    return instance->exe_dir_path;
}

const sf::path& NemesisInfo::WorkingDirectory()
{
    return instance->working_directory;
}

const std::filesystem::path& NemesisInfo::WorkingDirectory(const std::filesystem::path& working_directory)
{
    return instance->working_directory = CanonizePath(working_directory);
}

const sf::path& NemesisInfo::DataPath()
{
    return instance->data_path;
}

const sf::path& NemesisInfo::DataPath(const sf::path& data_path)
{
    return instance->data_path = CanonizePath(data_path);
}

const sf::path& NemesisInfo::StagePath()
{
    return instance->stage_path;
}

const sf::path& NemesisInfo::StagePath(const sf::path& stage_path)
{
    return instance->stage_path = CanonizePath(stage_path);
}

sf::path NemesisInfo::PatchOutputPath(const sf::path& filepath)
{
    auto& spath = instance->stage_path;
    auto& dpath = instance->data_path;

    if (spath == dpath || spath.empty()) return filepath;

    auto filepath_str = CanonizePath(filepath).wstring();
    auto datapath_str = dpath.wstring();

    if (!nemesis::istarts_with(filepath_str, datapath_str)) return filepath;

    return spath / filepath_str.substr(datapath_str.length() + 1);
}

bool NemesisInfo::IsAsync()
{
    return instance->is_async;
}

bool NemesisInfo::IsDebug()
{
    return instance->debug;
}

bool NemesisInfo::IsProgressIndicatorActive()
{
    return instance->progress_indicator_active;
}
