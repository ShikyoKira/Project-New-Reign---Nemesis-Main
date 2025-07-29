#include <chrono>
#include <iostream>
#include <sstream>

#include <Python.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Logger.h"
#include "NemesisInfo.h"

#include "Core/CoreObject.h"

#include "ExAnimation/ExAnimationRepository.h"

#include "Utilities/Crc32.h"
#include "Utilities/MD5.h"
#include "Utilities/OnScopeEnds.h"
#include "Utilities/ProgressMeter.h"
#include "Utilities/ThreadPool.h"

std::atomic_bool StopProcessFlag(false);

namespace sf = std::filesystem;

void log_compilation_result(const std::chrono::high_resolution_clock::time_point& start,
                            std::string check_sum)
{
    auto end     = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    Logger::Log("Time taken: " + std::to_string(static_cast<double>(elapsed.count()) / 1000) + "s", true);
    Logger::Log("CheckSum: " + check_sum, true);
}

std::string get_memorable_hash(const std::string& data)
{
    constexpr std::array<const char*, 21> first_adjectives
        = {"BIG",  "SMALL", "NEW",    "OLD",  "FAST", "SLOW", "GOOD", "BAD",   "DEEP", "HIGH", "LOW",
           "LONG", "SHORT", "BRIGHT", "DARK", "HOT",  "COLD", "WARM", "FRESH", "HARD", "SOFT"};
    constexpr std::array<const char*, 21> second_adjectives
        = {"RED",    "BLUE",   "GREEN", "YELLOW", "ORANGE", "BROWN", "PURPLE",
           "COPPER", "GOLDEN", "THIN",  "THICK",  "LOOSE",  "TIGHT", "STURDY",
           "BOUNCY", "DUSTY",  "HEAVY", "LIGHT",  "CLEAN",  "DIRTY", "FRAGILE"};
    constexpr std::array<const char*, 21> objects
        = {"PHONES",  "BUDS",  "COINS", "KEYS",    "CARS",    "SHOES",   "BUTTONS",
           "BOTTLES", "COINS", "HATS",  "EYES",    "BAGS",    "HANDS",   "RINGS",
           "BADGES",  "BALLS", "ROCKS", "WATCHES", "GLASSES", "WALLETS", "CUPS"};

    constexpr const char* salt = "d(3mN*3+H(9cE&>";
    std::string salted_md5     = nemesis::md5::hash_to_string(data + NemesisInfo::GetVersion() + salt);
    std::string checksum;

    checksum.append(std::to_string(std::stoi(salted_md5.substr(0, 4), nullptr, 16) % 98 + 2));
    checksum.push_back(' ');
    checksum.append(first_adjectives[std::stoi(salted_md5.substr(4, 2), nullptr, 16) % 21]);
    checksum.push_back(' ');
    checksum.append(second_adjectives[std::stoi(salted_md5.substr(6, 2), nullptr, 16) % 21]);
    checksum.push_back(' ');
    checksum.append(objects[std::stoi(salted_md5.substr(31, 2), nullptr, 16) % 21]);
    checksum.append(" & ");
    checksum.append(std::to_string(std::stoi(salted_md5.substr(12, 4), nullptr, 16) % 98 + 2));
    checksum.push_back(' ');
    checksum.append(first_adjectives[std::stoi(salted_md5.substr(16, 2), nullptr, 16) % 21]);
    checksum.push_back(' ');
    checksum.append(second_adjectives[std::stoi(salted_md5.substr(18, 2), nullptr, 16) % 21]);
    checksum.push_back(' ');
    checksum.append(objects[std::stoi(salted_md5.substr(25, 2), nullptr, 16) % 21]);
    return checksum;
}

void run_python_scripts(const std::filesystem::path& dir_path)
{
    Logger::Log(LITERAL_PATH("Scanning for Python Scripts: ") + PATH_TO_STRING(dir_path));

    if (!std::filesystem::exists(dir_path)) return;

    for (auto entry : std::filesystem::directory_iterator(dir_path))
    {
        auto path = entry.path();

        if (entry.is_directory()) continue;

        if (!nemesis::iequals(PATH_TO_STRING(path.extension()), LITERAL_PATH(".py"))) continue;

        auto path_s = path.string();

        try
        {
            FILE* py_file;
#if _WIN32
            _wfopen_s(&py_file, path.wstring().c_str(), L"r");
#else
            py_file = fopen(path.string().c_str(), "r");
#endif

            if (!py_file) return;

            auto filename_s = nemesis::to_utf8_string(path.filename());
            Logger::Log("Executing Python Script: " + filename_s, true);

            Py_Initialize();
            PyRun_SimpleFile(py_file, filename_s.c_str());
            Py_Finalize();

            fclose(py_file);

            Logger::Log(LITERAL_PATH("Executed Python Script: ") + PATH_TO_STRING(path.filename()));
        }
        catch (const std::exception& ex)
        {
            throw std::runtime_error("Exception occured when executing a python script (File: " + path_s
                                     + ", Message: " + ex.what() + ")");
        }
    }
}

void change_python_config_settings(std::function<PyStatus()> change_settings)
{
    PyStatus status = change_settings();

    if (!PyStatus_Exception(status)) return;

    Py_ExitStatusException(status);
}

void setup_python_config(const std::filesystem::path& libs_dir)
{
    Logger::Log(LITERAL_PATH("Setting up python environment: ") + PATH_TO_STRING(libs_dir));

    std::wstring dir_path  = libs_dir.wstring();
    std::wstring site_path = (libs_dir / LITERAL_PATH("lib") / LITERAL_PATH("site-packages")).wstring();
    PyConfig config;
    nemesis::OnScopeEnds on_ends([&config]() { PyConfig_Clear(&config); });

    config.write_bytecode = 0;
    PyConfig_InitPythonConfig(&config);

#ifdef _WIN32
    change_python_config_settings([&config, &dir_path]()
                                  { return PyConfig_SetString(&config, &config.home, dir_path.c_str()); });
#else
    change_python_config_settings(
        [&config, &libs_dir]()
        { return PyConfig_SetBytesString(&config, &config.home, libs_dir.string().c_str()); });
#endif

    change_python_config_settings(
        [&config, &site_path]()
        { return PyWideStringList_Append(&config.module_search_paths, site_path.c_str()); });

    change_python_config_settings([&config]() { return Py_InitializeFromConfig(&config); });

    Py_Finalize();
}

#if _WIN32
int wmain(int argc, wchar_t* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
#else
int main(int argc, char* argv[])
{
#endif

    try
    {
        VecStr mods;
        int rst = NemesisInfo::Setup(argc, argv, mods);

        if (rst < 1) return rst;

        auto start = std::chrono::high_resolution_clock::now();
        std::future<void> preload_task;

        if (NemesisInfo::IsPreload())
        {
            preload_task = std::async(std::launch::deferred, [&mods, &start]() {
                    std::string mod_code;
                    std::string mod_code_cmd;

                    std::cout << "Mod Codes: ";

                    auto wait_for = std::chrono::high_resolution_clock::now();
                    std::getline(std::cin, mod_code_cmd);
                    auto wait_until = std::chrono::high_resolution_clock::now();

                    for (auto& ch : mod_code_cmd)
                    {
                        switch (ch)
                        {
                            case '\n':
                            case '\r':
                                break;
                            default:
                            {
                                mod_code.push_back(ch);
                                break;
                            }
                        }
                    }

                    std::istringstream ss(mod_code);
                    mods.clear();

                    while (std::getline(ss, mod_code, ' '))
                    {
                        mods.emplace_back(mod_code);
                    }

                    if (!mods.empty() && nemesis::is_only_number(mods.back()))
                    {
                        std::chrono::milliseconds duration_from_ticks(std::stoll(mods.back()));
                        start = std::chrono::high_resolution_clock::time_point() + duration_from_ticks;
                        mods.pop_back();
                    }
                    else
                    {
                        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(wait_until - wait_for);
                        start += elapsed;
                    }
                });
        }

        std::filesystem::path exe_dir  = NemesisInfo::ExeDirectory();
        std::filesystem::path data_dir = NemesisInfo::DataPath();
        size_t check_sum;

        setup_python_config(exe_dir / LITERAL_PATH("scripts"));

        nemesis::ProgressMeter progress_meter(120,
            NemesisInfo::IsProgressIndicatorActive() ? [](unsigned int step, unsigned int max)
            {
                if (StopProcessFlag) return;

                std::cout << "\x1b[999P" << step << " / " << max << "\x1b[999E" << std::endl;
            } : [](unsigned int step, unsigned int max) {});

        // Intentional memory leak
        nemesis::ExAnimationRepository* ex_anim_repo;
        nemesis::NObjectRepository* repo;
        nemesis::AlterAnimRepository* aa_repo;
        nemesis::ModRepository* mod_repo;
        nemesis::TemplateRepository* templt_repo;
        nemesis::AnimationRequestRepository* anim_repo;

        run_python_scripts(exe_dir / LITERAL_PATH("scripts") / LITERAL_PATH("start"));

        progress_meter.ProgressUp(10);

        std::cout << "\n" << std::endl;

        if (NemesisInfo::IsAsync())
        {
            auto ex_anim_repo_future = std::async(
                [&]
                {
                    auto ptr = new nemesis::ExAnimationRepository(data_dir / LITERAL_PATH("meshes"));
                    progress_meter.ProgressUp(10);
                    return ptr;
                });
            auto repo_future = std::async(
                [&]
                {
                    auto ptr = new nemesis::NObjectRepository(data_dir);
                    progress_meter.ProgressUp(10);
                    return ptr;
                });
            auto aa_repo_future = std::async(
                [&]
                {
                    auto ptr = new nemesis::AlterAnimRepository(exe_dir / LITERAL_PATH("alternate_animations")
                                                                / LITERAL_PATH("AlternateAnimations.json"));
                    progress_meter.ProgressUp(10);
                    return ptr;
                });
            auto mod_repo_future = std::async(
                [&]
                {
                    auto ptr = new nemesis::ModRepository(exe_dir / LITERAL_PATH("mods"));
                    progress_meter.ProgressUp(10);
                    return ptr;
                });

            repo = repo_future.get();

            auto templt_repo_future = std::async(
                [&]
                {
                    auto ptr = new nemesis::TemplateRepository(exe_dir / LITERAL_PATH("behavior_templates"),
                                                               *repo);
                    progress_meter.ProgressUp(10);
                    return ptr;
                });

            templt_repo = templt_repo_future.get();

            aa_repo = aa_repo_future.get();

            auto anim_repo_future = std::async(
                [&]
                {
                    auto ptr = new nemesis::AnimationRequestRepository(data_dir, *templt_repo, *aa_repo);
                    progress_meter.ProgressUp(10);
                    return ptr;
                });

            mod_repo  = mod_repo_future.get();
            anim_repo = anim_repo_future.get();

            std::cout << "\n" << std::endl;

            repo->Patch(*mod_repo);
            progress_meter.ProgressUp(10);

            std::cout << "\n" << std::endl;

            if (preload_task.valid()) preload_task.get();

            mods = mod_repo->PatchSelectedMods(mods);
            // Required to make sure the console output is flushed and captured correctly
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            progress_meter.ProgressUp(10);

            ex_anim_repo = ex_anim_repo_future.get();

            std::cout << "\n" << std::endl;

            ex_anim_repo->OutputExAnimationInfo();
        }
        else
        {
            ex_anim_repo = new nemesis::ExAnimationRepository(data_dir / LITERAL_PATH("meshes"));
            progress_meter.ProgressUp(10);

            repo = new nemesis::NObjectRepository(data_dir);
            progress_meter.ProgressUp(10);

            aa_repo = new nemesis::AlterAnimRepository(exe_dir / LITERAL_PATH("alternate_animations")
                                                       / LITERAL_PATH("AlternateAnimations.json"));
            progress_meter.ProgressUp(10);

            mod_repo = new nemesis::ModRepository(exe_dir / "mods");
            progress_meter.ProgressUp(10);

            templt_repo
                = new nemesis::TemplateRepository(exe_dir / LITERAL_PATH("behavior_templates"), *repo);
            progress_meter.ProgressUp(10);

            anim_repo = new nemesis::AnimationRequestRepository(data_dir, *templt_repo, *aa_repo);
            progress_meter.ProgressUp(10);

            std::cout << "\n" << std::endl;

            repo->Patch(*mod_repo);
            progress_meter.ProgressUp(10);

            std::cout << "\n" << std::endl;

            if (preload_task.valid()) preload_task.get();

            mods = mod_repo->PatchSelectedMods(mods);
            // Required to make sure the console output is flushed and captured correctly
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            progress_meter.ProgressUp(10);

            std::cout << "\n" << std::endl;

            ex_anim_repo->OutputExAnimationInfo();
        }

        int cur_step            = 0;
        constexpr int step_size = 20;
        std::mutex step_mtx;
        nemesis::CompilationManager* manager
            = new nemesis::CompilationManager(mods, *aa_repo, *anim_repo, *templt_repo, *ex_anim_repo);
        repo->Compile(*manager,
                      [&progress_meter, &cur_step, &step_mtx](int step, int max)
                      {
                          std::scoped_lock<std::mutex> lock(step_mtx);
                          int new_step = static_cast<double>(step) / max * step_size;

                          if (new_step > cur_step)
                          {
                              progress_meter.ProgressUp(new_step - cur_step);
                              cur_step = new_step;
                          }
                      });

        std::cout << "\n" << std::endl;

        run_python_scripts(exe_dir / LITERAL_PATH("scripts") / LITERAL_PATH("end"));

        std::cout << "\n" << std::endl;
        // Required to make sure the console output is flushed and captured correctly
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        check_sum   = manager->GetFullCheckSum();
        auto rehash = get_memorable_hash(std::to_string(check_sum));

        progress_meter.Complete();
        log_compilation_result(start, rehash);
    }
    catch (const nemesis::ThreadPool::ThreadException&)
    {
        return 1;
    }
    catch (const std::exception& ex)
    {
        StopProcessFlag = true;
        std::cout << std::endl;
        Logger::Log(std::string("[ERROR] ") + ex.what(), true);
        std::cout << std::endl;
        return 1;
    }
    catch (...)
    {
        StopProcessFlag = true;
        std::cout << std::endl;
        Logger::Log("[ERROR] Unknown exception captured", true);
        std::cout << std::endl;
        return 1;
    }
}
