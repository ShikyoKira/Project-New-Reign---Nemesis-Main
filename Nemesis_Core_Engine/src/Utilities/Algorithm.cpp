#include <cctype>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "Utilities/Algorithm.h"

#if _WIN32
#include <Windows.h>

#define DUPLICATE_STRING(CharPtr) _strdup(CharPtr)
#define DUPLICATE_WSTRING(WCharPtr) _wcsdup(WCharPtr)
#else
#include <wchar.h>

#define DUPLICATE_STRING(CharPtr) strdup(CharPtr)
#define DUPLICATE_WSTRING(WCharPtr) wcsdup(WCharPtr)
#endif

namespace nemesis
{
    template <typename CharType>
    struct CharPtrWrapper
    {
    private:
        CharType _c;

    public:
        CharPtrWrapper(CharType c)
        {
            _c = c;
        }

        ~CharPtrWrapper()
        {
            delete _c;
        }

        CharType GetCharPtr()
        {
            return _c;
        }
    };

    const char* to_lower_copy(const char* data)
    {
        char* temp  = DUPLICATE_STRING(data);

        if (temp == nullptr)
        {
            std::cerr << "Memory allocation failed. Out of memory or invalid input" << std::endl;
            exit(-1);
        }

        size_t size = std::strlen(data);
        size_t i    = 0;

        while (i < size)
        {
            temp[i] = std::tolower(temp[i]);
            i++;
        }

        return temp;
    }

    const wchar_t* to_lower_copy(const wchar_t* data)
    {
        wchar_t* temp = DUPLICATE_WSTRING(data);

        if (temp == nullptr)
        {
            std::cerr << "Memory allocation failed. Out of memory or invalid input" << std::endl;
            exit(-1);
        }

        size_t size = std::wcslen(data);
        size_t i    = 0;

        while (i < size)
        {
            temp[i] = std::tolower(temp[i]);
            i++;
        }

        return temp;
    }

    std::string to_lower_copy(const std::string& data)
    {
        return nemesis::CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr();
    }

    std::wstring to_lower_copy(const std::wstring& data)
    {
        return nemesis::CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr();
    }

    nemesis::Line to_lower_copy(const nemesis::Line& data)
    {
        return nemesis::Line(nemesis::CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    nemesis::Wline to_lower_copy(const nemesis::Wline& data)
    {
        return nemesis::Wline(nemesis::CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    void to_lower(std::string& data)
    {
        data = nemesis::CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr();
    }

    void to_lower(std::wstring& data)
    {
        data = nemesis::CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr();
    }

    void to_lower(nemesis::Line& data)
    {
        data = nemesis::Line(nemesis::CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    void to_lower(nemesis::Wline& data)
    {
        data = nemesis::Wline(nemesis::CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    const char* to_upper_copy(const char* data)
    {
        char* temp  = DUPLICATE_STRING(data);

        if (temp == nullptr)
        {
            std::cerr << "Memory allocation failed. Out of memory or invalid input" << std::endl;
            exit(-1);
        }

        size_t size = std::strlen(data);
        size_t i    = 0;

        while (i < size)
        {
            temp[i] = std::toupper(temp[i]);
            i++;
        }

        return temp;
    }

    const wchar_t* to_upper_copy(const wchar_t* data)
    {
        wchar_t* temp = DUPLICATE_WSTRING(data);

        if (temp == nullptr)
        {
            std::cerr << "Memory allocation failed. Out of memory or invalid input" << std::endl;
            exit(-1);
        }

        size_t size   = std::wcslen(data);
        size_t i      = 0;

        while (i < size)
        {
            temp[i] = std::toupper(temp[i]);
            i++;
        }

        return temp;
    }

    std::string to_upper_copy(const std::string& data)
    {
        return nemesis::CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr();
    }

    std::wstring to_upper_copy(const std::wstring& data)
    {
        return nemesis::CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr();
    }

    nemesis::Line to_upper_copy(const nemesis::Line& data)
    {
        return nemesis::Line(nemesis::CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    nemesis::Wline to_upper_copy(const nemesis::Wline& data)
    {
        return nemesis::Wline(nemesis::CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    void to_upper(std::string& data)
    {
        data = nemesis::CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr();
    }

    void to_upper(std::wstring& data)
    {
        data = nemesis::CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr();
    }

    void to_upper(nemesis::Line& data)
    {
        data = nemesis::Line(nemesis::CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    void to_upper(nemesis::Wline& data)
    {
        data = nemesis::Wline(nemesis::CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    bool iequals(const char* l, const char* r)
    {
        return std::strcmp(nemesis::CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                           nemesis::CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const wchar_t* l, const wchar_t* r)
    {
        return std::wcscmp(nemesis::CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                           nemesis::CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const char* l, const std::string& r)
    {
        return std::strcmp(nemesis::CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                           nemesis::CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
    }

    bool iequals(const wchar_t* l, const std::wstring& r)
    {
        return std::wcscmp(nemesis::CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                           nemesis::CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
    }

    bool iequals(const std::string& l, const char* r)
    {
        return std::strcmp(nemesis::CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                           nemesis::CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const std::wstring& l, const wchar_t* r)
    {
        return std::wcscmp(nemesis::CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                           nemesis::CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const std::string& l, const std::string& r)
    {
        if (l.size() != r.size()) return false;

        return std::equal(
            l.begin(), l.end(), r.begin(), [](char a, char b) { return std::tolower(a) == std::tolower(b); });
    }

    bool iequals(const std::wstring& l, const std::wstring& r)
    {
        if (l.size() != r.size()) return false;

        return std::equal(l.begin(),
                          l.end(),
                          r.begin(),
                          [](wchar_t a, wchar_t b) { return std::tolower(a) == std::tolower(b); });
    }

    bool starts_with(const char* data, const char* start)
    {
        size_t data_size  = std::strlen(data);
        size_t start_size = std::strlen(start);

        if (data_size < start_size) return false;

        if (data_size == start_size) return std::strcmp(data, start) == 0;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[i] == start[i]) continue;

            return false;
        }

        return true;
    }

    bool starts_with(const wchar_t* data, const wchar_t* start)
    {
        size_t data_size  = std::wcslen(data);
        size_t start_size = std::wcslen(start);

        if (data_size < start_size) return false;

        if (data_size == start_size) return std::wcscmp(data, start) == 0;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[i] == start[i]) continue;

            return false;
        }

        return true;
    }

    bool starts_with(const char* data, const std::string& start)
    {
        size_t data_size  = std::strlen(data);
        size_t start_size = start.size();

        if (data_size < start_size) return false;

        if (data_size == start_size) return std::strcmp(data, start.c_str()) == 0;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[i] == start[i]) continue;

            return false;
        }

        return true;
    }

    bool starts_with(const wchar_t* data, const std::wstring& start)
    {
        size_t data_size  = std::wcslen(data);
        size_t start_size = start.size();

        if (data_size < start_size) return false;

        if (data_size == start_size) return std::wcscmp(data, start.c_str()) == 0;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[i] == start[i]) continue;

            return false;
        }

        return true;
    }

    bool starts_with(const std::string& data, const char* start)
    {
        return data.starts_with(start);
    }

    bool starts_with(const std::wstring& data, const wchar_t* start)
    {
        return data.starts_with(start);
    }

    bool starts_with(const std::string& data, const std::string& start)
    {
        return data.starts_with(start);
    }

    bool starts_with(const std::wstring& data, const std::wstring& start)
    {
        return data.starts_with(start);
    }

    bool ends_with(const char* data, const char* start)
    {
        size_t data_size  = std::strlen(data);
        size_t start_size = std::strlen(start);

        if (data_size < start_size) return false;

        if (data_size == start_size) return std::strcmp(data, start) == 0;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[data_size - i - 1] == start[start_size - i - 1]) continue;

            return false;
        }

        return true;
    }

    bool ends_with(const wchar_t* data, const wchar_t* start)
    {
        size_t data_size  = std::wcslen(data);
        size_t start_size = std::wcslen(start);

        if (data_size < start_size) return false;

        if (data_size == start_size) return std::wcscmp(data, start) == 0;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[data_size - i - 1] == start[start_size - i - 1]) continue;

            return false;
        }

        return true;
    }

    bool ends_with(const char* data, const std::string& start)
    {
        size_t data_size  = std::strlen(data);
        size_t start_size = start.size();

        if (data_size < start_size) return false;

        if (data_size == start_size) return std::strcmp(data, start.c_str()) == 0;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[data_size - i - 1] == start[start_size - i - 1]) continue;

            return false;
        }

        return true;
    }

    bool ends_with(const wchar_t* data, const std::wstring& start)
    {
        size_t data_size  = std::wcslen(data);
        size_t start_size = start.size();

        if (data_size < start_size) return false;

        if (data_size == start_size) return std::wcscmp(data, start.c_str()) == 0;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[data_size - i - 1] == start[start_size - i - 1]) continue;

            return false;
        }

        return true;
    }

    bool ends_with(const std::string& data, const char* start)
    {
        return ends_with(data.c_str(), start);
    }

    bool ends_with(const std::wstring& data, const wchar_t* start)
    {
        return ends_with(data.c_str(), start);
    }

    bool ends_with(const std::string& data, const std::string& start)
    {
        size_t data_size  = data.size();
        size_t start_size = start.size();

        if (data_size < start_size) return false;

        if (data_size == start_size) return data == start;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[data_size - i - 1] == start[start_size - i - 1]) continue;

            return false;
        }

        return true;
    }

    bool ends_with(const std::wstring& data, const std::wstring& start)
    {
        size_t data_size  = data.size();
        size_t start_size = start.size();

        if (data_size < start_size) return false;

        if (data_size == start_size) return data == start;

        for (size_t i = 0; i < start_size; i++)
        {
            if (data[data_size - i - 1] == start[start_size - i - 1]) continue;

            return false;
        }

        return true;
    }

    size_t isearch(const std::string& data, const std::string& key, size_t off)
    {
        return to_lower_copy(data).find(to_lower_copy(key), off);
    }

    size_t isearch(const std::wstring& data, const std::wstring& key, size_t off)
    {
        return to_lower_copy(data).find(to_lower_copy(key), off);
    }

    size_t risearch(const std::string& data, const std::string& key, size_t off)
    {
        return to_lower_copy(data).rfind(to_lower_copy(key), off);
    }

    size_t risearch(const std::wstring& data, const std::wstring& key, size_t off)
    {
        return to_lower_copy(data).rfind(to_lower_copy(key), off);
    }

    std::string_view between(const std::string& line, std::string_view prefix, std::string_view suffix)
    {
        std::string_view sv = line;
        auto pos            = sv.find(prefix);

        if (pos == NOT_FOUND) throw std::runtime_error("prefix not matched");

        pos += prefix.length();
        sv.remove_prefix(pos);
        pos = sv.find(suffix);

        if (pos == NOT_FOUND) throw std::runtime_error("suffix not matched");

        return sv.substr(0, pos);
    }

    std::wstring_view between(const std::wstring& line, std::wstring_view prefix, std::wstring_view suffix)
    {
        std::wstring_view sv = line;
        auto pos             = sv.find(prefix);

        if (pos == NOT_FOUND) throw std::runtime_error("prefix not matched");

        pos += prefix.length();
        sv.remove_prefix(pos);
        pos = sv.find(suffix);

        if (pos == NOT_FOUND) throw std::runtime_error("suffix not matched");

        return sv.substr(0, pos);
    }

    std::string& replace(std::string& line, const char& old_char, const char& new_char)
    {
        size_t length = line.length();

        for (size_t i = 0; i < length; ++i)
        {
            if (line[i] != old_char) continue;

            line[i] = new_char;
        }

        return line;
    }

    std::string& replace(std::string& line, const char& old_char, const std::string& new_val)
    {
        size_t length = line.length();
        std::string new_line;

        for (size_t i = 0; i < length; ++i)
        {
            if (line[i] != old_char)
            {
                new_line.push_back(line[i]);
                continue;
            }

            new_line.append(new_val);
        }

        line = new_line;
        return line;
    }

    std::string& replace(std::string& line, const std::string& old_val, const char& new_char)
    {
        size_t length     = line.length();
        size_t old_length = old_val.length();

        if (length < old_length) return line;

        std::string new_line;
        size_t replace_limit = length - old_length;

        for (size_t i = 0; i < replace_limit; ++i)
        {
            if (line[i] != old_val[0])
            {
                new_line.push_back(line[i]);
                continue;
            }

            for (size_t k = 1; k < old_length; ++k)
            {
                if (line[i + k] != old_val[k]) break;

                if (k != old_length - 1) continue;

                i += k;
                new_line.push_back(new_char);
            }
        }

        for (size_t i = replace_limit; i < length; ++i)
        {
            new_line.push_back(line[i]);
        }

        line = new_line;
        return line;
    }

    std::string& replace(std::string& line, const std::string& old_val, const std::string& new_val)
    {
        if (old_val.size() == 1)
        {
            if (new_val.size() == 1) return nemesis::replace(line, old_val.front(), new_val.front());

            return nemesis::replace(line, old_val.front(), new_val);
        }
        else if (new_val.size() == 1)
        {
            return nemesis::replace(line, old_val, new_val.front());
        }

        size_t length     = line.length();
        size_t old_length = old_val.length();

        if (length < old_length) return line;

        std::string new_line;
        size_t replace_limit = length - old_length;

        for (size_t i = 0; i < replace_limit; ++i)
        {
            if (line[i] != old_val[0])
            {
                new_line.push_back(line[i]);
                continue;
            }

            for (size_t k = 1; k < old_length; ++k)
            {
                if (line[i + k] != old_val[k]) break;

                if (k != old_length - 1) continue;

                i += k;
                new_line.append(new_val);
            }
        }

        for (size_t i = replace_limit; i < length; ++i)
        {
            new_line.push_back(line[i]);
        }

        line = new_line;
        return line;
    }

    std::wstring& replace(std::wstring& line, const wchar_t& old_char, const wchar_t& new_char)
    {
        size_t length = line.length();

        for (size_t i = 0; i < length; ++i)
        {
            if (line[i] != old_char) continue;

            line[i] = new_char;
        }

        return line;
    }

    std::wstring& replace(std::wstring& line, const wchar_t& old_char, const std::wstring& new_val)
    {
        size_t length = line.length();
        std::wstring new_line;

        for (size_t i = 0; i < length; ++i)
        {
            if (line[i] != old_char)
            {
                new_line.push_back(line[i]);
                continue;
            }

            new_line.append(new_val);
        }

        line = new_line;
        return line;
    }

    std::wstring& replace(std::wstring& line, const std::wstring& old_val, const wchar_t& new_char)
    {
        size_t length     = line.length();
        size_t old_length = old_val.length();

        if (length < old_length) return line;

        std::wstring new_line;
        size_t replace_limit = length - old_length;

        for (size_t i = 0; i < replace_limit; ++i)
        {
            if (line[i] != old_val[0])
            {
                new_line.push_back(line[i]);
                continue;
            }

            for (size_t k = 1; k < old_length; ++k)
            {
                if (line[i + k] != old_val[k]) break;

                if (k != old_length - 1) continue;

                i += k;
                new_line.push_back(new_char);
            }
        }

        for (size_t i = replace_limit; i < length; ++i)
        {
            new_line.push_back(line[i]);
        }

        line = new_line;
        return line;
    }

    std::wstring& replace(std::wstring& line, const std::wstring& old_val, const std::wstring& new_val)
    {
        if (old_val.size() == 1)
        {
            if (new_val.size() == 1) return nemesis::replace(line, old_val.front(), new_val.front());

            return nemesis::replace(line, old_val.front(), new_val);
        }
        else if (new_val.size() == 1)
        {
            return nemesis::replace(line, old_val, new_val.front());
        }

        size_t length     = line.length();
        size_t old_length = old_val.length();

        if (length < old_length) return line;

        std::wstring new_line;
        size_t replace_limit = length - old_length;

        for (size_t i = 0; i < replace_limit; ++i)
        {
            if (line[i] != old_val[0])
            {
                new_line.push_back(line[i]);
                continue;
            }

            for (size_t k = 1; k < old_length; ++k)
            {
                if (line[i + k] != old_val[k]) break;

                if (k != old_length - 1) continue;

                i += k;
                new_line.append(new_val);
            }
        }

        for (size_t i = replace_limit; i < length; ++i)
        {
            new_line.push_back(line[i]);
        }

        line = new_line;
        return line;
    }

    std::string transform_to(const std::wstring& str) noexcept
    {
        if (str.empty()) return std::string();

#if _WIN32
        size_t length = WideCharToMultiByte(CP_UTF8, 0, &str[0], (int) str.size(), NULL, 0, NULL, NULL);
        std::string narrow(length, 0);
        WideCharToMultiByte(CP_UTF8, 0, &str[0], (int) str.size(), &narrow[0], length, NULL, NULL);
        return narrow;
#else
        return std::string(str.begin(), str.end());
#endif
    }

    std::wstring transform_to(const std::string& str) noexcept
    {
        if (str.empty()) return std::wstring();

#if _WIN32
        int length = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), NULL, 0);
        std::wstring wide(length, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &wide[0], length);
        return wide;
#else
        return std::wstring(str.begin(), str.end());
#endif
    }

    nemesis::Line transform_to(const nemesis::Wline& str) noexcept
    {
        return nemesis::Line(nemesis::transform_to(str.ToWstring()), str.GetLineNumber());
    }

    nemesis::Wline transform_to(const nemesis::Line& str) noexcept
    {
        return nemesis::Wline(nemesis::transform_to(str.ToString()), str.GetLineNumber());
    }

    std::string generate_guid()
    {
        static thread_local std::mt19937 gen(std::random_device{}());
        static thread_local std::uniform_int_distribution<> dis(0, 255);

        const size_t size = 16;
        uint8_t bytes[size];

        for (auto& byte : bytes)
        {
            byte = dis(gen);
        }

        bytes[8] = (bytes[8] & 0x3F) | 0x80; // Set variant bits
        bytes[6] = (bytes[6] & 0x0F) | 0x40; // Set version bits

        static const char hex_chars[] = "0123456789abcdef";
        std::string result;
        result.reserve(36);

        for (size_t i = 0; i < size; ++i)
        {
            result.push_back(hex_chars[(bytes[i] >> 4) & 0x0F]);
            result.push_back(hex_chars[bytes[i] & 0x0F]);

            switch (i)
            {
                case 3:
                case 5:
                case 7:
                case 9:
                {
                    result.push_back('-');
                    break;
                }
            }
        }

        return result;
    }

    std::wstring generate_guid_w()
    {
        static thread_local std::mt19937 gen(std::random_device{}());
        static thread_local std::uniform_int_distribution<> dis(0, 255);

        const size_t size = 16;
        uint8_t bytes[size];

        for (auto& byte : bytes)
        {
            byte = dis(gen);
        }

        bytes[8] = (bytes[8] & 0x3F) | 0x80; // Set variant bits
        bytes[6] = (bytes[6] & 0x0F) | 0x40; // Set version bits

        static const wchar_t hex_chars[] = L"0123456789abcdef";
        std::wstring result;
        result.reserve(36);

        for (size_t i = 0; i < size; ++i)
        {
            result.push_back(hex_chars[(bytes[i] >> 4) & 0x0F]);
            result.push_back(hex_chars[bytes[i] & 0x0F]);

            switch (i)
            {
                case 3:
                case 5:
                case 7:
                case 9:
                {
                    result.push_back(L'-');
                    break;
                }
            }
        }

        return result;
    }

    bool is_only_number(const std::string& line)
    {
        char* end  = nullptr;
        double val = std::strtod(line.c_str(), &end);
        return end != line.c_str() && *end == '\0' && val != HUGE_VAL;
    }

    bool is_only_number(const std::wstring& line)
    {
        wchar_t* end = nullptr;
        double val   = std::wcstod(line.c_str(), &end);
        return end != line.c_str() && *end == L'\0' && val != HUGE_VAL;
    }
}
