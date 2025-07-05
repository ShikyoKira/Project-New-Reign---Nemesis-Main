#include "Core/ModRepository.h"

#include <iostream>

#include "Utilities/Algorithm.h"
#include "Utilities/ThreadPool.h"

#include "Logger.h"


nemesis::ModRepository::ModRepository(const std::filesystem::path& mods_dir_path)
{
    Logger::Log("Processing Mod Objects...", true);

    if (!std::filesystem::exists(mods_dir_path)) return;

    nemesis::ThreadPool thread_pool;

    for (auto& entry : std::filesystem::directory_iterator(mods_dir_path))
    {
        if (!entry.is_directory()) continue;

        auto ini_path = entry.path() / "info.ini";

        if (!std::filesystem::exists(ini_path)) continue;

        auto& mod_cls = ModClassList.emplace_back(std::make_unique<nemesis::ModClass>(ini_path, thread_pool));
        mod_cls->FinalizeInitialization();
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
    VecStr mods;
    USetStr valid_mods;

    for (auto& mod_class : ModClassList)
    {
        auto& info     = mod_class->GetModInfo();
        auto& mod_code = info.GetModCode();
        valid_mods.insert(nemesis::to_lower_copy(mod_code));
    }

    for (auto& mod : selected_mods)
    {
        std::string mod_code = nemesis::to_lower_copy(mod);

        if (valid_mods.find(mod_code) == valid_mods.end())
        {
            throw std::runtime_error(
                "Invalid mod code (" + mod_code
                + ") selected. Please check if you have installed the mod patch correctly");
        }

        Logger::Log("Active Mod " + std::to_string(mods.size() + 1) + ": " + mod_code, true);
        mods.emplace_back(mod_code);
    }

    if (mods.empty())
    {
        Logger::Log("No active mods detected", true);
    }

    return mods;
}
