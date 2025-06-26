#pragma once

#include <filesystem>

namespace nemesis
{
    struct ModInfo
    {
    private:
        std::string name, author, site, automatic, modcode;

    public:
        void ReadFile(const std::filesystem::path& infopath);

        void SetModCode(const std::string& modcode);

        std::string GetDisplayName() const noexcept;
        const std::string& GetName() const noexcept;
        const std::string& GetAuthor() const noexcept;
        const std::string& GetSite() const noexcept;
        const std::string& GetAutoRefFile() const noexcept;
        const std::string& GetModCode() const noexcept;
    };
}
