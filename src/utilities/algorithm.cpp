#include "Global.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <random>
#include <sstream>
#include <iomanip>

#include "utilities/algorithm.h"

using namespace std;

namespace nemesis
{
    template<typename CharType>
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
        char* temp = nullptr;

        while (!temp)
        {
            temp = _strdup(data);
        }

        int size   = strlen(data);
        int i      = 0;

        while (i < size)
        {
            temp[i] = tolower(temp[i]);
            i++;
        }

        return temp;
    }

    const wchar_t* to_lower_copy(const wchar_t* data)
    {
        wchar_t* temp = nullptr;

        while (!temp)
        {
            temp = _wcsdup(data);
        }

        int size      = wcslen(data);
        int i         = 0;

        while (i < size)
        {
            temp[i] = tolower(temp[i]);
            i++;
        }

        return temp;
    }

    string to_lower_copy(const string& data)
    {
        return CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr();
    }

    wstring to_lower_copy(const wstring& data)
    {
        return CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr();
    }

    nemesis::Line to_lower_copy(const nemesis::Line& data)
    {
        return nemesis::Line(CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    nemesis::Wline to_lower_copy(const nemesis::Wline& data)
    {
        return nemesis::Wline(CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    void to_lower(string& data)
    {
        data = CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr();
    }

    void to_lower(wstring& data)
    {
        data = CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr();
    }

    void to_lower(nemesis::Line& data)
    {
        data = nemesis::Line(CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    void to_lower(nemesis::Wline& data)
    {
        data = nemesis::Wline(CharPtrWrapper(to_lower_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    const char* to_upper_copy(const char* data)
    {
        char* temp = _strdup(data);
        int size   = strlen(data);
        int i      = 0;

        while (i < size)
        {
            temp[i] = toupper(temp[i]);
            i++;
        }

        return temp;
    }

    const wchar_t* to_upper_copy(const wchar_t* data)
    {
        wchar_t* temp = _wcsdup(data);
        int size      = wcslen(data);
        int i         = 0;

        while (i < size)
        {
            temp[i] = toupper(temp[i]);
            i++;
        }

        return temp;
    }

    string to_upper_copy(const string& data)
    {
        return CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr();
    }

    wstring to_upper_copy(const wstring& data)
    {
        return CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr();
    }

    nemesis::Line to_upper_copy(const nemesis::Line& data)
    {
        return nemesis::Line(CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    nemesis::Wline to_upper_copy(const nemesis::Wline& data)
    {
        return nemesis::Wline(CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    void to_upper(string& data)
    {
        data = CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr();
    }

    void to_upper(wstring& data)
    {
        data = CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr();
    }

    void to_upper(nemesis::Line& data)
    {
        data = nemesis::Line(CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    void to_upper(nemesis::Wline& data)
    {
        data = nemesis::Wline(CharPtrWrapper(to_upper_copy(data.c_str())).GetCharPtr(), data.GetLineNumber());
    }

    bool iequals(const char* l, const char* r)
    {
        return std::strcmp(CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                           CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const wchar_t* l, const wchar_t* r)
    {
        return std::wcscmp(CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                           CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const char* l, const string& r)
    {
        return std::strcmp(CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                           CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
    }

    bool iequals(const wchar_t* l, const wstring& r)
    {
        return std::wcscmp(CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                           CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
    }

    bool iequals(const string& l, const char* r)
    {
        return std::strcmp(CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                           CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const wstring& l, const wchar_t* r)
    {
        return std::wcscmp(CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                           CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const string& l, const string& r)
    {
        return std::strcmp(CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                           CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
    }

    bool iequals(const wstring& l, const wstring& r)
    {
        return std::wcscmp(CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                           CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
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
        return data._Starts_with(start);
    }

    bool starts_with(const std::wstring& data, const wchar_t* start)
    {
        return data._Starts_with(start);
    }

    bool starts_with(const std::string& data, const std::string& start)
    {
        return data._Starts_with(start);
    }

    bool starts_with(const std::wstring& data, const std::wstring& start)
    {
        return data._Starts_with(start);
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
        auto pos                = sv.find(prefix);

        if (pos == NOT_FOUND) throw std::runtime_error("prefix not matched");

        pos += prefix.length();
        sv.remove_prefix(pos);
        pos = sv.find(suffix);

        if (pos == NOT_FOUND) throw std::runtime_error("suffix not matched");

        return sv.substr(0, pos);
    }

    std::string transform_to(const std::wstring& str) noexcept
    {
        // Note[1]
        return std::filesystem::_Convert_wide_to_narrow<char_traits<char>>(
            __std_fs_code_page(), str, allocator<char>());
    }

    std::wstring transform_to(const std::string& str) noexcept
    {
        // Note[1]
        return std::filesystem::_Convert_narrow_to_wide(__std_fs_code_page(), str);
    }

    std::string transform_to(const wchar_t* str) noexcept
    {
        // Note[1]
        return std::filesystem::_Convert_wide_to_narrow<char_traits<char>>(
            __std_fs_code_page(), str, allocator<char>());
    }

    std::wstring transform_to(const char* str) noexcept
    {
        // Note[1]
        return std::filesystem::_Convert_narrow_to_wide(__std_fs_code_page(), str);
    }

    nemesis::Line transform_to(const nemesis::Wline& str) noexcept
    {
        return nemesis::Line(transform_to(str.ToWstring()), str.GetLineNumber());
    }

    nemesis::Wline transform_to(const nemesis::Line& str) noexcept
    {
        return nemesis::Wline(transform_to(str.ToString()), str.GetLineNumber());
    }
    std::string generate_guid()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);

        std::stringstream ss;
        ss << std::hex;

        for (int i = 0; i < 32; ++i)
        {
            int rand_num = dis(gen);
            ss << (i == 8 || i == 12 || i == 16 || i == 20 ? "-" : "") << rand_num;
        }

        return ss.str();
    }

    std::wstring generate_guid_w()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);

        std::wstringstream ss;
        ss << std::hex;

        for (int i = 0; i < 32; ++i)
        {
            int rand_num = dis(gen);
            ss << (i == 8 || i == 12 || i == 16 || i == 20 ? L"-" : L"") << rand_num;
        }

        return ss.str();
    }
} // namespace nemesis

// Note [1]
// locale convert and transform_to<wstring, string> does not give consistent result
// so file system path is used as a temporary workaround
// until solid workaround is found