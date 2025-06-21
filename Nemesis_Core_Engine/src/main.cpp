#include "Logger.h"
#include "NemesisInfo.h"

#include <chrono>
#include <iostream>

#include "Core/CoreObject.h"

#include "ExAnimation/ExAnimationRepository.h"

#include "Utilities/Crc32.h"
#include "Utilities/MD5.h"
#include "Utilities/ThreadPool.h"
#include "Utilities/ProgressMeter.h"

namespace sf = std::filesystem;

void log_compilation_result(const std::chrono::steady_clock::time_point& start, std::wstring check_sum)
{
    auto end     = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    Logger::Log(L"Time taken: " + std::to_wstring(static_cast<double>(elapsed.count()) / 1000) + L"s", true);
    Logger::Log(L"CheckSum: " + check_sum, true);
}

std::wstring get_memorable_hash(const std::wstring& data)
{
    constexpr std::array<const wchar_t*, 21> first_adjectives = {
        L"BIG",  L"SMALL", L"NEW",    L"OLD",  L"FAST", L"SLOW", L"GOOD", L"BAD",   L"DEEP", L"HIGH", L"LOW",
        L"LONG", L"SHORT", L"BRIGHT", L"DARK", L"HOT",  L"COLD", L"WARM", L"FRESH", L"HARD", L"SOFT"};
    constexpr std::array<const wchar_t*, 21> second_adjectives
        = {L"RED",    L"BLUE",   L"GREEN", L"YELLOW", L"ORANGE", L"BROWN", L"PURPLE",
           L"COPPER", L"GOLDEN", L"THIN",  L"THICK",  L"LOOSE",  L"TIGHT", L"STURDY",
           L"BOUNCY", L"DUSTY",  L"HEAVY", L"LIGHT",  L"CLEAN",  L"DIRTY", L"FRAGILE"};
    constexpr std::array<const wchar_t*, 21> objects
        = {L"HEARTS", L"HANDS", L"BOXES",  L"LEAVES",  L"CARS",   L"PEBBLES", L"TILES",
           L"KNIVES", L"COINS", L"SWORDS", L"SHIELDS", L"SPOONS", L"PENS",    L"BOOKS",
           L"CHAIRS", L"BALLS", L"STONES", L"BOATS",   L"LAMPS",  L"TABLES",  L"CLOCKS"};

    constexpr const wchar_t* salt = L"d(3mN*3+H(9cE&>";
    std::wstring salted_md5       = nemesis::md5::hash_to_string(data + NemesisInfo::GetVersion() + salt);
    std::wstring checksum;

    checksum.append(std::to_wstring(std::stoi(salted_md5.substr(0, 4), nullptr, 16) % 98 + 2));
    checksum.push_back(L' ');
    checksum.append(first_adjectives[std::stoi(salted_md5.substr(4, 2), nullptr, 16) % 21]);
    checksum.push_back(L' ');
    checksum.append(second_adjectives[std::stoi(salted_md5.substr(6, 2), nullptr, 16) % 21]);
    checksum.push_back(L' ');
    checksum.append(objects[std::stoi(salted_md5.substr(31, 2), nullptr, 16) % 21]);
    checksum.append(L" & ");
    checksum.append(std::to_wstring(std::stoi(salted_md5.substr(12, 4), nullptr, 16) % 98 + 2));
    checksum.push_back(L' ');
    checksum.append(first_adjectives[std::stoi(salted_md5.substr(16, 2), nullptr, 16) % 21]);
    checksum.push_back(L' ');
    checksum.append(second_adjectives[std::stoi(salted_md5.substr(18, 2), nullptr, 16) % 21]);
    checksum.push_back(L' ');
    checksum.append(objects[std::stoi(salted_md5.substr(25, 2), nullptr, 16) % 21]);
    return checksum;
}

int wmain(int argc, wchar_t* argv[])
{
    try
    {
        auto start = std::chrono::high_resolution_clock::now();

        VecStr mods;
        int rst = NemesisInfo::Setup(argc, argv, mods);

        if (rst < 1) return rst;

        std::filesystem::path exe_dir  = NemesisInfo::ExeDirectory();
        std::filesystem::path data_dir = NemesisInfo::DataPath();
        size_t check_sum;

        nemesis::ProgressMeter progress_meter(100,
                                              [](unsigned int step, unsigned int max)
                                              {
                                                  if (!NemesisInfo::IsProgressIndicatorActive()) return;

                                                  std::cout << "\x1b[999P" << step << " / " << max
                                                            << "\x1b[999E" << std::endl;
                                              });

        // Intentional memory leak
        nemesis::ExAnimationRepository* ex_anim_repo;
        nemesis::NObjectRepository* repo;
        nemesis::AlterAnimRepository* aa_repo;
        nemesis::ModRepository* mod_repo;
        nemesis::TemplateRepository* templt_repo;
        nemesis::AnimationRequestRepository* anim_repo;

        if (NemesisInfo::IsAsync())
        {
            auto ex_anim_repo_future
                = std::async([&] { return new nemesis::ExAnimationRepository(data_dir / L"meshes"); });
            auto repo_future    = std::async([&] { return new nemesis::NObjectRepository(data_dir); });
            auto aa_repo_future = std::async(
                [&]
                {
                    return new nemesis::AlterAnimRepository(exe_dir / L"alternate_animations"
                                                            / L"AlternateAnimations.json");
                });
            auto mod_repo_future = std::async([&] { return new nemesis::ModRepository(exe_dir / L"mods"); });

            repo = repo_future.get();
            progress_meter.ProgressUp(10);

            auto templt_repo_future = std::async(
                [&] { return new nemesis::TemplateRepository(exe_dir / L"behavior_templates", *repo); });

            templt_repo = templt_repo_future.get();
            progress_meter.ProgressUp(10);

            aa_repo = aa_repo_future.get();
            progress_meter.ProgressUp(10);

            auto anim_repo_future = std::async(
                [&] { return new nemesis::AnimationRequestRepository(data_dir, *templt_repo, *aa_repo); });

            mod_repo = mod_repo_future.get();
            progress_meter.ProgressUp(10);

            anim_repo = anim_repo_future.get();
            progress_meter.ProgressUp(10);

            repo->Patch(*mod_repo);
            progress_meter.ProgressUp(10);

            std::cout << "\n" << std::endl;

            mods = mod_repo->PatchSelectedMods(mods);
            progress_meter.ProgressUp(10);

            ex_anim_repo = ex_anim_repo_future.get();
            progress_meter.ProgressUp(10);

            ex_anim_repo->OutputExAnimationInfo();
        }
        else
        {
            ex_anim_repo = new nemesis::ExAnimationRepository(data_dir / L"meshes");
            progress_meter.ProgressUp(10);

            repo = new nemesis::NObjectRepository(data_dir);
            progress_meter.ProgressUp(10);

            aa_repo      = new nemesis::AlterAnimRepository(exe_dir / L"alternate_animations"
                                                       / L"AlternateAnimations.json");
            progress_meter.ProgressUp(10);

            mod_repo = new nemesis::ModRepository(exe_dir / L"mods");
            progress_meter.ProgressUp(10);

            templt_repo = new nemesis::TemplateRepository(exe_dir / L"behavior_templates", *repo);
            progress_meter.ProgressUp(10);

            anim_repo = new nemesis::AnimationRequestRepository(data_dir, *templt_repo, *aa_repo);
            progress_meter.ProgressUp(10);

            repo->Patch(*mod_repo);
            progress_meter.ProgressUp(10);

            std::cout << "\n" << std::endl;

            mods = mod_repo->PatchSelectedMods(mods);
            progress_meter.ProgressUp(10);

            ex_anim_repo->OutputExAnimationInfo();
        }

        nemesis::CompilationManager* manager
            = new nemesis::CompilationManager(mods, *aa_repo, *anim_repo, *templt_repo, *ex_anim_repo);
        repo->Compile(*manager);
        progress_meter.ProgressUp(10);

        check_sum   = manager->GetFullCheckSum();
        auto rehash = get_memorable_hash(std::to_wstring(check_sum));

        progress_meter.Complete();
        log_compilation_result(start, rehash);
    }
    catch (const nemesis::ThreadPool::ThreadException&)
    {
        return 1;
    }
    catch (const std::exception& ex)
    {
        std::wcout << std::endl;
        Logger::Log(std::string("ERROR: ") + ex.what(), true);
        return 1;
    }
    catch (...)
    {
        std::wcout << std::endl;
        Logger::Log("ERROR: Unknown exception captured", true);
        return 1;
    }
}
