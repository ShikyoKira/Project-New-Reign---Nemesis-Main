#pragma once

#include <string>

namespace nemesis
{
    const char* to_lower_copy(const char* data);
    const wchar_t* to_lower_copy(const wchar_t* data);

    std::string to_lower_copy(const std::string& data);
    std::wstring to_lower_copy(const std::wstring& data);

    void to_lower(std::string& data);
    void to_lower(std::wstring& data);

    const char* to_upper_copy(const char* data);
    const wchar_t* to_upper_copy(const wchar_t* data);

    std::string to_upper_copy(const std::string& data);
    std::wstring to_upper_copy(const std::wstring& data);

    void to_upper(std::string& data);
    void to_upper(std::wstring& data);

    bool iequals(const char* l, const char* r);
    bool iequals(const wchar_t* l, const wchar_t* r);
    bool iequals(const char* l, const std::string& r);
    bool iequals(const wchar_t* l, const std::wstring& r);
    bool iequals(const std::string& l, const char* r);
    bool iequals(const std::wstring& l, const wchar_t* r);
    bool iequals(const std::string& l, const std::string& r);
    bool iequals(const std::wstring& l, const std::wstring& r);

    template <typename T, typename F>
    inline T transform_to(const F& str) noexcept
    {
        if (str.empty()) return {};

        return {std::begin(str), std::end(str)};
    };

} // namespace nemesis
