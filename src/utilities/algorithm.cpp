#include "Global.h"

#include <algorithm>
#include <cctype>
#include <string>

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
        return strcmp(CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                      CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const wchar_t* l, const wchar_t* r)
    {
        return wcscmp(CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                      CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const char* l, const string& r)
    {
        return strcmp(CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                      CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
    }

    bool iequals(const wchar_t* l, const wstring& r)
    {
        return wcscmp(CharPtrWrapper(to_lower_copy(l)).GetCharPtr(),
                      CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
    }

    bool iequals(const string& l, const char* r)
    {
        return strcmp(CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                      CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const wstring& l, const wchar_t* r)
    {
        return wcscmp(CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                      CharPtrWrapper(to_lower_copy(r)).GetCharPtr())
               == 0;
    }

    bool iequals(const string& l, const string& r)
    {
        return strcmp(CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                      CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
    }

    bool iequals(const wstring& l, const wstring& r)
    {
        return wcscmp(CharPtrWrapper(to_lower_copy(l.c_str())).GetCharPtr(),
                      CharPtrWrapper(to_lower_copy(r.c_str())).GetCharPtr())
               == 0;
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
        return std::filesystem::path(str).string();
    }

    std::wstring transform_to(const std::string& str) noexcept
    {
        // Note[1]
        return std::filesystem::path(str).wstring();
    }

    std::string transform_to(const wchar_t* str) noexcept
    {
        // Note[1]
        return std::filesystem::path(str).string();
    }

    std::wstring transform_to(const char* str) noexcept
    {
        // Note[1]
        return std::filesystem::path(str).wstring();
    }

    nemesis::Line transform_to(const nemesis::Wline& str) noexcept
    {
        return nemesis::Line(transform_to(str.ToWstring()), str.GetLineNumber());
    }

    nemesis::Wline transform_to(const nemesis::Line& str) noexcept
    {
        return nemesis::Wline(transform_to(str.ToString()), str.GetLineNumber());
    }
} // namespace nemesis

// Note [1]
// locale convert and transform_to<wstring, string> does not give consistent result
// so file system path is used as a temporary workaround
// until solid workaround is found