#include "Global.h"

#include <algorithm>
#include <cctype>
#include <string>

#include "utilities/algorithm.h"

using namespace std;

namespace nemesis
{
    const char* to_lower_copy(const char* data)
    {
        char* temp = _strdup(data);
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
        wchar_t* temp = _wcsdup(data);
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
        return to_lower_copy(data.c_str());
    }

    wstring to_lower_copy(const wstring& data)
    {
        return to_lower_copy(data.c_str());
    }

    nemesis::Line to_lower_copy(const nemesis::Line& data)
    {
        return nemesis::Line(to_lower_copy(data.c_str()), data.GetLineNumber());
    }

    nemesis::Wline to_lower_copy(const nemesis::Wline& data)
    {
        return nemesis::Wline(to_lower_copy(data.c_str()), data.GetLineNumber());
    }

    void to_lower(string& data)
    {
        data = to_lower_copy(data.c_str());
    }

    void to_lower(wstring& data)
    {
        data = to_lower_copy(data.c_str());
    }

    void to_lower(nemesis::Line& data)
    {
        data = nemesis::Line(to_lower_copy(data.c_str()), data.GetLineNumber());
    }

    void to_lower(nemesis::Wline& data)
    {
        data = nemesis::Wline(to_lower_copy(data.c_str()), data.GetLineNumber());
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
        return to_upper_copy(data.c_str());
    }

    wstring to_upper_copy(const wstring& data)
    {
        return to_upper_copy(data.c_str());
    }

    nemesis::Line to_upper_copy(const nemesis::Line& data)
    {
        return nemesis::Line(to_upper_copy(data.c_str()), data.GetLineNumber());
    }

    nemesis::Wline to_upper_copy(const nemesis::Wline& data)
    {
        return nemesis::Wline(to_upper_copy(data.c_str()), data.GetLineNumber());
    }

    void to_upper(string& data)
    {
        data = to_upper_copy(data.c_str());
    }

    void to_upper(wstring& data)
    {
        data = to_upper_copy(data.c_str());
    }

    void to_upper(nemesis::Line& data)
    {
        data = nemesis::Line(to_upper_copy(data.c_str()), data.GetLineNumber());
    }

    void to_upper(nemesis::Wline& data)
    {
        data = nemesis::Wline(to_upper_copy(data.c_str()), data.GetLineNumber());
    }

    bool iequals(const char* l, const char* r)
    {
        return strcmp(to_lower_copy(l), to_lower_copy(r)) == 0;
    }

    bool iequals(const wchar_t* l, const wchar_t* r)
    {
        return wcscmp(to_lower_copy(l), to_lower_copy(r)) == 0;
    }

    bool iequals(const char* l, const string& r)
    {
        return strcmp(to_lower_copy(l), to_lower_copy(r.c_str())) == 0;
    }

    bool iequals(const wchar_t* l, const wstring& r)
    {
        return wcscmp(to_lower_copy(l), to_lower_copy(r.c_str())) == 0;
    }

    bool iequals(const string& l, const char* r)
    {
        return strcmp(to_lower_copy(l.c_str()), to_lower_copy(r)) == 0;
    }

    bool iequals(const wstring& l, const wchar_t* r)
    {
        return wcscmp(to_lower_copy(l.c_str()), to_lower_copy(r)) == 0;
    }

    bool iequals(const string& l, const string& r)
    {
        return strcmp(to_lower_copy(l.c_str()), to_lower_copy(r.c_str())) == 0;
    }

    bool iequals(const wstring& l, const wstring& r)
    {
        return wcscmp(to_lower_copy(l.c_str()), to_lower_copy(r.c_str())) == 0;
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