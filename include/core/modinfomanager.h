#pragma once

#include "core/modinfo.h"

namespace nemesis
{
	struct ModInfoManager
    {
    private:
        Vec<nemesis::ModInfo> modinfo_list;
        VecStr hiddenMods;

    public:
        void ReadAllInfo();

        const nemesis::ModInfo* GetModInfo(const std::wstring& modcode) const noexcept;
        const nemesis::ModInfo* GetModInfoByDisplay(const std::wstring& displayname) const noexcept;
        const Vec<nemesis::ModInfo> GetModInfoList() const noexcept;
        const VecStr GetHiddenMods() const noexcept;
    };
}
