#include "Core/ModRepository.h"

#include <iostream>

#include "Utilities/Algorithm.h"
#include "Utilities/ThreadPool.h"

#include "Logger.h"


nemesis::ModRepository::ModRepository(const std::filesystem::path& mods_dir_path)
{
    Logger::Log(L"Processing Mod Objects...", true);

    if (!std::filesystem::exists(mods_dir_path)) return;

    nemesis::ThreadPool thread_pool;

    for (auto& entry : std::filesystem::directory_iterator(mods_dir_path))
    {
        if (!entry.is_directory()) continue;

        auto ini_path = entry.path() / "info.ini";

        if (!std::filesystem::exists(ini_path)) continue;

        ModClassList.emplace_back(std::make_unique<nemesis::ModClass>(ini_path, thread_pool));
    }

    thread_pool.join_all();
}

Vec<const nemesis::ModClass*> nemesis::ModRepository::GetModClassList() const
{
    if (ModClassListCache) return *ModClassListCache;

    ModClassListCache = std::make_unique<Vec<const nemesis::ModClass*>>();

    for (auto& mod_class : ModClassList)
    {
        ModClassListCache->emplace_back(mod_class.get());
    }

    return *ModClassListCache;
}

VecStr nemesis::ModRepository::PatchSelectedMods(const VecStr& selected_mods) const
{
    constexpr auto has_value = [](const VecStr& selected_mods, const std::string& mod_code)
    {
        for (auto& mod : selected_mods)
        {
            if (!nemesis::iequals(mod, mod_code)) continue;

            return true;
        }

        return false;
    };

    VecStr mods;
    USetStr valid_mods;

    for (auto& mod_class : ModClassList)
    {
        auto& info = mod_class->GetModInfo();
        std::string mod_code = info.GetModCodeA();
        valid_mods.insert(mod_code);

        if (!info.IsHidden()) continue;


        if (has_value(selected_mods, mod_code)) continue;

        Logger::Log("(Auto) Active Mod " + std::to_string(mods.size() + 1) + ": " + mod_code, true);
        mods.emplace_back(mod_code);
    }

    for (auto& mod : selected_mods)
    {
        if (valid_mods.find(mod) == valid_mods.end())
        {
            throw std::runtime_error("Invalid mod code (" + mod + ") selected. Please check if you have installed the mod patch correctly");
        }

        Logger::Log("Active Mod " + std::to_string(mods.size() + 1) + ": " + mod, true);
        mods.emplace_back(mod);
    }

    std::cout << "\n" << std::endl;
    return mods;
}
