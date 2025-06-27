#pragma once

#include <filesystem>

namespace nemesis
{
    inline std::string to_utf8_string(const std::filesystem::path& path)
    {
#if _WIN32
        auto path_u8 = path.u8string();
        return std::string(path_u8.begin(), path_u8.end());
#else
        return path.string();
#endif
    }

    inline std::wstring to_utf8_wstring(const std::filesystem::path& path)
    {
#if _WIN32
        auto path_u8 = path.u8string();
        return std::wstring(path_u8.begin(), path_u8.end());
#else
        return path.wstring();
#endif
    }
}

#if _WIN32
#define PATH_TO_STRING(PATH) nemesis::to_utf8_wstring(PATH)
#define LITERAL_PATH(PATH) (L##PATH)

using path_char   = wchar_t;
using path_string = std::wstring;
#else
#define PATH_TO_STRING(PATH) (PATH).string()
#define LITERAL_PATH(PATH) (PATH)

using path_char   = char;
using path_string = std::string;
#endif
