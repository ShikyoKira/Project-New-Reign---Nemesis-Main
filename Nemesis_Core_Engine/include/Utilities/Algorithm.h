#pragma once

#include "Nemesis_Havok/include/Utilities/Algorithm.h"

#include "Utilities/Line.h"

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

    bool starts_with(const char* data, const char* start);
    bool starts_with(const wchar_t* data, const wchar_t* start);
    bool starts_with(const char* data, const std::string& start);
    bool starts_with(const wchar_t* data, const std::wstring& start);
    bool starts_with(const std::string& data, const char* start);
    bool starts_with(const std::wstring& data, const wchar_t* start);
    bool starts_with(const std::string& data, const std::string& start);
    bool starts_with(const std::wstring& data, const std::wstring& start);

    bool ends_with(const char* data, const char* start);
    bool ends_with(const wchar_t* data, const wchar_t* start);
    bool ends_with(const char* data, const std::string& start);
    bool ends_with(const wchar_t* data, const std::wstring& start);
    bool ends_with(const std::string& data, const char* start);
    bool ends_with(const std::wstring& data, const wchar_t* start);
    bool ends_with(const std::string& data, const std::string& start);
    bool ends_with(const std::wstring& data, const std::wstring& start);

    template <typename DataType, typename StartType>
    bool istarts_with(DataType data, StartType start)
    {
        return starts_with(to_lower_copy(data), to_lower_copy(start));
    }

    template <typename DataType, typename EndType>
    bool iends_with(DataType data, EndType end)
    {
        return ends_with(to_lower_copy(data), to_lower_copy(end));
    }

    size_t isearch(const std::string& data, const std::string& key, size_t off = 0U);
    size_t isearch(const std::wstring& data, const std::wstring& key, size_t off = 0U);
    
    size_t risearch(const std::string& data, const std::string& key, size_t off = std::string::npos);
    size_t risearch(const std::wstring& data, const std::wstring& key, size_t off = std::wstring::npos);

    std::string_view between(const std::string& line, std::string_view prefix, std::string_view suffix);
    std::wstring_view between(const std::wstring& line, std::wstring_view prefix, std::wstring_view suffix);

    std::string& replace(std::string& line, const char& old_char, const char& new_char);
    std::string& replace(std::string& line, const char& old_char, const std::string& new_val);
    std::string& replace(std::string& line, const std::string& old_val, const char& new_char);
    std::string& replace(std::string& line, const std::string& old_val, const std::string& new_val);
    std::wstring& replace(std::wstring& line, const wchar_t& old_char, const wchar_t& new_char);
    std::wstring& replace(std::wstring& line, const wchar_t& old_char, const std::wstring& new_val);
    std::wstring& replace(std::wstring& line, const std::wstring& old_val, const wchar_t& new_char);
    std::wstring& replace(std::wstring& line, const std::wstring& old_val, const std::wstring& new_val);

    std::string transform_to(const std::wstring& str) noexcept;
    std::wstring transform_to(const std::string& str) noexcept;

    nemesis::Line transform_to(const nemesis::Wline& str) noexcept;
    nemesis::Wline transform_to(const nemesis::Line& str) noexcept;

    template <typename To, typename From>
    inline To transform_to(const From& str) noexcept
    {
        if constexpr (std::is_same_v<std::string, To> && std::is_same_v<std::wstring, From>)
        {
            return transform_to(static_cast<std::wstring>(str));
        }
        
        if constexpr (std::is_same_v<std::wstring, To> && std::is_same_v<std::string, From>)
        {
            return transform_to(static_cast<std::string>(str));
        }

        if (str.empty()) return {};

        return {std::begin(str), std::end(str)};
    };

    std::string generate_guid();
    std::wstring generate_guid_w();

    bool is_only_number(const std::string& line);
    bool is_only_number(const std::wstring& line);
}
