#pragma once

#include "utilities/line.h"

namespace nemesis
{
    const char* to_lower_copy(const char* data);
    const wchar_t* to_lower_copy(const wchar_t* data);

    std::string to_lower_copy(const std::string& data);
    std::wstring to_lower_copy(const std::wstring& data);

    nemesis::Line to_lower_copy(const nemesis::Line& data);
    nemesis::Wline to_lower_copy(const nemesis::Wline& data);

    void to_lower(std::string& data);
    void to_lower(std::wstring& data);

    void to_lower(nemesis::Line& data);
    void to_lower(nemesis::Wline& data);

    const char* to_upper_copy(const char* data);
    const wchar_t* to_upper_copy(const wchar_t* data);

    std::string to_upper_copy(const std::string& data);
    std::wstring to_upper_copy(const std::wstring& data);

    nemesis::Line to_upper_copy(const nemesis::Line& data);
    nemesis::Wline to_upper_copy(const nemesis::Wline& data);

    void to_upper(nemesis::Line& data);
    void to_upper(nemesis::Wline& data);

    bool iequals(const char* l, const char* r);
    bool iequals(const wchar_t* l, const wchar_t* r);
    bool iequals(const char* l, const std::string& r);
    bool iequals(const wchar_t* l, const std::wstring& r);
    bool iequals(const std::string& l, const char* r);
    bool iequals(const std::wstring& l, const wchar_t* r);
    bool iequals(const std::string& l, const std::string& r);
    bool iequals(const std::wstring& l, const std::wstring& r);

    std::string transform_to(const std::wstring& str) noexcept;
    std::wstring transform_to(const std::string& str) noexcept;

    std::string transform_to(const wchar_t* str) noexcept;
    std::wstring transform_to(const char* str) noexcept;

    nemesis::Line transform_to(const nemesis::Wline& str) noexcept;
    nemesis::Wline transform_to(const nemesis::Line& str) noexcept;

    template <typename To, typename From>
    inline To transform_to(const From& str) noexcept
    {
        if (str.empty()) return {};

        return {std::begin(str), std::end(str)};
    };

} // namespace nemesis
