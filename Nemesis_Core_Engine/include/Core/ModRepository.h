#pragma once

#include "Core/ModClass.h"

namespace nemesis
{
    struct ModRepository
    {
    private:
        Vec<UPtr<nemesis::ModClass>> ModClassList;

        mutable UPtr<Vec<const nemesis::ModClass*>> ModClassListCache;

    public:
        ModRepository(const std::filesystem::path& mods_dir_path);

        Vec<const nemesis::ModClass*> GetModClassList() const;
        VecStr PatchSelectedMods(const VecStr& selected_mods) const;
    };
}
