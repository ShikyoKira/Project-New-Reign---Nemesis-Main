#include "core/modinfomanager.h"

void nemesis::ModInfoManager::ReadAllInfo()
{
    std::wstring folder = L"mod\\";
    VecWstr modlist;
    read_directory(folder, modlist);

    for (auto& modcode : modlist)
    {
        std::wstring path = folder + modcode;

        if (!std::filesystem::is_directory(path)) continue;

        if (!isFileExist(path + L"\\info.ini")) continue;

        std::wstring filename = path + L"\\info.ini";
        nemesis::ModInfo modinfo;
        modinfo.ReadFile(filename);
        modinfo.SetModCode(modcode);

        if (modinfo.GetName().empty() || modinfo.GetAuthor().empty() || modinfo.GetSite().empty())
        {
            throw std::runtime_error(
                nemesis::transform_to<std::string>(TextBoxMessage(1014) + L" " + modcode));
        }

        if (!modinfo.IsHidden())
        {
            modinfo_list.emplace_back(modinfo);
            continue;
        }

        hiddenMods.emplace_back(nemesis::transform_to<std::string>(modcode));
    }
}

const nemesis::ModInfo* nemesis::ModInfoManager::GetModInfo(const std::wstring& modcode) const noexcept
{
    for (auto& modinfo : modinfo_list)
    {
        if (modinfo.GetModCode() == modcode) return &modinfo;
    }

    return nullptr;
}

const nemesis::ModInfo*
nemesis::ModInfoManager::GetModInfoByDisplay(const std::wstring& displayname) const noexcept
{
    for (auto& modinfo : modinfo_list)
    {
        if (modinfo.GetDisplayName() == displayname) return &modinfo;
    }

    return nullptr;
}

const Vec<nemesis::ModInfo> nemesis::ModInfoManager::GetModInfoList() const noexcept
{
    return modinfo_list;
}

const VecStr nemesis::ModInfoManager::GetHiddenMods() const noexcept
{
    return hiddenMods;
}
