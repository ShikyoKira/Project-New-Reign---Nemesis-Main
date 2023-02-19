#pragma once

#include <string>
#include <string_view>

namespace nemesis
{
    struct ModInfo
    {
    private:
        std::wstring name, author, site, automatic, modcode;
        bool hidden = false;

    public:
        void ReadFile(const std::filesystem::path& infopath);

        void SetModCode(const std::wstring& modcode);

        std::wstring GetDisplayName() const noexcept;
        std::wstring_view GetName() const noexcept;
        std::wstring_view GetAuthor() const noexcept;
        std::wstring_view GetSite() const noexcept;
        std::wstring_view GetAutoRefFile() const noexcept;
        std::wstring_view GetModCode() const noexcept;
        std::string GetDisplayNameA() const noexcept;
        std::string GetNameA() const noexcept;
        std::string GetAuthorA() const noexcept;
        std::string GetSiteA() const noexcept;
        std::string GetAutoRefFileA() const noexcept;
        std::string GetModCodeA() const noexcept;
        bool IsHidden() const noexcept;
    };
}
