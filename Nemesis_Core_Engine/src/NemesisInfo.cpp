#include <functional>
#include <regex>

#include "Logger.h"
#include "NemesisInfo.h"

#include "Utilities/File.h"
#include "Utilities/FileWriter.h"

namespace sf = std::filesystem;

const std::string version          = "1.0.0";
NemesisInfo* NemesisInfo::instance = new NemesisInfo();

const std::filesystem::path& NemesisInfo::CanonizePath(const std::filesystem::path& path)
{
    static Map<std::filesystem::path, std::filesystem::path> PathCache;
    static std::mutex PathCacheMutex;

    std::scoped_lock<std::mutex> lock(PathCacheMutex);
    auto path_itr = PathCache.find(path);

    if (path_itr != PathCache.end()) return path_itr->second;

    return PathCache.insert({path, sf::absolute(path)}).first->second;
}

void NemesisInfo::PrintHelp(const std::filesystem::path& exe_path)
{
    std::cout
        << "Nemesis Unlimited Behavior Engine made by Shikyo Kira v" << version << "\n\n"
        << "Nemesis Unlimited Behavior Engine processes Nemesis Extended XML (.nemx) and animation queries "
           "to generate standard XML (.xml) or Havok (.hkx) binary formats, AnimationDataSingleFile and "
           "AnimationSetDataSingleFile\n\n"
           "Nemx files provide extended XML syntax with embedded functions and macros for dynamic content "
           "generation and modular data building\n\n"
           "Usage:\n"
        << "     " << nemesis::to_utf8_string(exe_path.filename())
        << " -p win32 -d \"C:\\Program Files "
           "(x86)\\steam\\steamapps\\common\\Skyrim Special Edition\\Data\" -o "
           "\"My\\Staging\\Directory\\Path\" -m tkuc nemesis bcbi\n\n"
           "     -h      Help info\n"
           "     -p      Output platform [ps3, ps4, 360, win32 (default), amd64]\n"
           "     -v      Havok version. ie: hk_2010.2.0-r1 (default)\n"
           "     -d      Skyrim data directory\n"
           "     -o      Output directory. Skyrim data directory is the default value (optional)\n"
           "     -pi     Output progress indicator (optional)\n"
           "     -db     Toggle debug mode (optional)\n"
           "     -m      List of active mod codes. Must be the last. Priority order starts from lowest "
           "(left) "
           "to highest (right)\n"
           "     -s      Enable synchronous processing for debugging purposes (default: asynchronous)\n"
        << std::endl;
}

int NemesisInfo::Setup(int argc, path_char* argv[], VecStr& mods)
{
    if (argc == 1)
    {
        PrintHelp(argv[0]);
        return 0;
    }

    NemesisInfo::WorkingDirectory(sf::current_path());

    path_string platform = LITERAL_PATH("win32");
    path_string hk_ver   = LITERAL_PATH("hk_2010.2.0-r1");
    sf::path data_path;

    for (size_t i = 1; i < argc; i++)
    {
        auto& arg = argv[i];

        if (nemesis::iequals(arg, LITERAL_PATH("-s")))
        {
            instance->is_async = false;
            continue;
        }

        if (nemesis::iequals(arg, LITERAL_PATH("-p")))
        {
            if (i + 1 == argc) continue;

            ++i;
            platform = argv[i];
            continue;
        }

        if (nemesis::iequals(arg, LITERAL_PATH("-v")))
        {
            if (i + 1 == argc) continue;

            ++i;
            hk_ver = argv[i];
            continue;
        }

        if (nemesis::iequals(arg, LITERAL_PATH("-d")))
        {
            if (i + 1 == argc) continue;

            data_path = DataPath(argv[++i]);
            continue;
        }

        if (nemesis::iequals(arg, LITERAL_PATH("-o")))
        {
            if (i + 1 == argc) continue;

            StagePath(argv[++i]);
            continue;
        }

        if (nemesis::iequals(arg, LITERAL_PATH("-pi")))
        {
            instance->progress_indicator_active = true;
            continue;
        }

        if (nemesis::iequals(arg, LITERAL_PATH("-db")))
        {
            instance->debug = true;
            continue;
        }

        if (nemesis::iequals(arg, LITERAL_PATH("-h")))
        {
            PrintHelp(argv[0]);
            return 0;
        }

        if (!nemesis::iequals(arg, LITERAL_PATH("-m"))) continue;

        while (++i < argc)
        {
#if _WIN32
            mods.emplace_back(nemesis::transform_to(std::wstring(argv[i])));
#else
            mods.emplace_back(argv[i]);
#endif
        }
    }

    if (data_path.empty())
    {
        auto dir = WorkingDirectory();
        sf::path datapath;
        sf::path::iterator data_itr = datapath.end();

        for (auto itr = dir.begin(); itr != dir.end(); ++itr)
        {
            if (!nemesis::iequals(PATH_TO_STRING(itr->filename()), LITERAL_PATH("data"))) continue;

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

    Logger::Log("Nemesis Version: " + version, true);
    Logger::Log(LITERAL_PATH("Log Path: ") + PATH_TO_STRING(Logger::LoggerPath));
    Logger::Log(LITERAL_PATH("Executable Path: ") + PATH_TO_STRING(NemesisInfo::ExePath()));
    Logger::Log(LITERAL_PATH("Executable Directory: ") + PATH_TO_STRING(NemesisInfo::ExeDirectory()), true);
    Logger::Log(LITERAL_PATH("Working Directory: ") + PATH_TO_STRING(NemesisInfo::WorkingDirectory()), true);
    Logger::Log(LITERAL_PATH("Data Directory: ") + PATH_TO_STRING(data_path), true);
    Logger::Log(LITERAL_PATH("Staging Path: ") + PATH_TO_STRING(StagePath()), true);

    if (nemesis::iequals(platform, LITERAL_PATH("ps3")))
    {
        OutputPlatform(nemesis::PlatformType::PS3);
    }
    else if (nemesis::iequals(platform, LITERAL_PATH("ps4")))
    {
        OutputPlatform(nemesis::PlatformType::PS4);
    }
    else if (nemesis::iequals(platform, LITERAL_PATH("360")))
    {
        OutputPlatform(nemesis::PlatformType::XB360);
    }
    else if (nemesis::iequals(platform, LITERAL_PATH("amd64")))
    {
        OutputPlatform(nemesis::PlatformType::AMD64);
    }
    else if (nemesis::iequals(platform, LITERAL_PATH("win32")))
    {
        OutputPlatform(nemesis::PlatformType::WIN32);
    }
    else
    {
#ifdef _WIN32
        throw std::runtime_error("Unsupported platform argument (" + nemesis::transform_to(platform)
                                 + ")");
#else
        throw std::runtime_error("Unsupported platform argument (" + platform + ")");
#endif
    }

    Logger::Log(LITERAL_PATH("Havok Version: ") + hk_ver, true);
    Logger::Log(LITERAL_PATH("Platform: ") + platform, true);

    try
    {
#ifdef _WIN32
        OutputVersion(nemesis::GetEnumVersion(nemesis::transform_to(hk_ver)));
#else
        OutputVersion(nemesis::GetEnumVersion(hk_ver));
#endif
    }
    catch (const std::exception&)
    {
    }

    std::cout << "\n" << std::endl;
    return 1;
}

std::string NemesisInfo::GetVersion()
{
    return version;
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

    auto filepath_str = PATH_TO_STRING(CanonizePath(filepath));
    auto datapath_str = PATH_TO_STRING(dpath);

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
