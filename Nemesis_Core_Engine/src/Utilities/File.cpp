#include <fstream>

#include "Utilities/File.h"
#include "Utilities/Line.h"
#include "Utilities/SharableWrapper.h"
#include "Utilities/Algorithm.h"

namespace sf = std::filesystem;

template <typename LineType>
bool ValidateEndLine(bool emptylast, Vec<LineType>& storeline, const Vec<LineType>& exclusions)
{
    if (storeline.size() == 0) return false;

    if (emptylast)
    {
        if (storeline.back().empty()) return true;

        for (auto& each : exclusions)
        {
            if (storeline.back().find(each) != NOT_FOUND) return true;
        }

        storeline.emplace_back(storeline.back()).clear();
    }
    else if (storeline.back().empty())
    {
        storeline.pop_back();
    }

    return true;
}

template <typename LineType>
bool ValidateEndLine(bool emptylast,
                     Vec<LineType>& storeline,
                     const Vec<LineType>& exclusions,
                     size_t linenum)
{
    if (storeline.size() == 0) return false;

    if (emptylast)
    {
        if (storeline.back().empty()) return true;

        for (auto& each : exclusions)
        {
            if (storeline.back().find(each) != NOT_FOUND) return true;
        }

        LineType line(linenum);
        storeline.emplace_back(line);
    }
    else if (storeline.back().empty())
    {
        storeline.pop_back();
    }

    return true;
}

//std::mutex mtxreader;
static const VecStr condend      = {"<!-- CONDITION END -->", "<!-- CLOSE -->"};
static const VecWstr wcondend    = {L"<!-- CONDITION END -->", L"<!-- CLOSE -->"};
static const VecNstr n_condend   = {"<!-- CONDITION END -->", "<!-- CLOSE -->"};
static const VecNwstr wn_condend = {L"<!-- CONDITION END -->", L"<!-- CLOSE -->"};

template <typename StreamType, typename RawType, typename LineType>
void GetFileNLines(const sf::path& filepath,
                   Vec<LineType>& lines,
                   std::function<RawType(const RawType&)> selector,
                   bool emptylast,
                   const Vec<LineType>& end_exclusion)
{
    lines.clear();
    StreamType stream(filepath);

    if (!stream.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                                 + "\"\nMessage: " + ec.message());
    }

    size_t linenum = 0;
    RawType line;
    lines.reserve(100000);

    if (std::getline(stream, line))
    {
        auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<sf::path>>(filepath);
        lines.emplace_back(std::move(selector(line)), ++linenum, filepath_ptr);

        while (std::getline(stream, line))
        {
            lines.emplace_back(std::move(selector(line)), ++linenum, filepath_ptr.get());
        }

        ValidateEndLine(emptylast, lines, end_exclusion);
    }

    lines.shrink_to_fit();
}

void GetFileLines(const sf::path& filepath,
                  VecNstr& lines,
                  std::function<std::string(const std::string&)> selector,
                  bool emptylast)
{
    GetFileNLines<std::fstream>(filepath, lines, selector, emptylast, n_condend);
}

void GetFileLines(const sf::path& filepath,
                  VecNwstr& lines,
                  std::function<std::wstring(const std::wstring&)> selector,
                  bool emptylast)
{
    GetFileNLines<std::wfstream>(filepath, lines, selector, emptylast, wn_condend);
}

template <typename StreamType, typename LineType>
void GetFileSLines(const sf::path& filepath,
                   Vec<LineType>& lines,
                   std::function<LineType(const LineType&)> selector,
                   bool emptylast,
                   const Vec<LineType>& end_exclusion)
{
    lines.clear();
    StreamType stream(filepath);

    if (!stream.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                                 + "\"\nMessage: " + ec.message());
    }

    LineType line;
    lines.reserve(100000);

    while (std::getline(stream, line))
    {
        lines.emplace_back(selector(line));
    }

    ValidateEndLine(emptylast, lines, end_exclusion);
    lines.shrink_to_fit();
}

void GetFileLines(const sf::path& filepath,
                  VecStr& lines,
                  std::function<std::string(const std::string&)> selector,
                  bool emptylast)
{
    GetFileSLines<std::fstream>(filepath, lines, selector, emptylast, condend);
}

void GetFileLines(const sf::path& filepath,
                  VecWstr& lines,
                  std::function<std::wstring(const std::wstring&)> selector,
                  bool emptylast)
{
    GetFileSLines<std::wfstream>(filepath, lines, selector, emptylast, wcondend);
}

template <typename StreamType, typename RawType, typename LineType>
void GetFileNLines(const sf::path& filepath,
                   Vec<LineType>& lines,
                   std::function<bool(RawType&)> predicament,
                   bool emptylast,
                   const Vec<LineType>& end_exclusion)
{
    lines.clear();
    StreamType stream(filepath);

    if (!stream.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                                 + "\"\nMessage: " + ec.message());
    }

    size_t linenum = 0;
    RawType line;
    lines.reserve(100000);

    while (std::getline(stream, line))
    {
        if (!predicament(line)) continue;

        goto continue_read;
    }

    lines.shrink_to_fit();
    return;

continue_read:
    auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<sf::path>>(filepath);
    lines.emplace_back(std::move(line), ++linenum, filepath_ptr);

    while (std::getline(stream, line))
    {
        if (!predicament(line)) continue;

        lines.emplace_back(std::move(line), ++linenum, filepath_ptr.get());
    }

    ValidateEndLine(emptylast, lines, end_exclusion);
    lines.shrink_to_fit();
}

void GetFileLines(const sf::path& filepath,
                  VecNstr& lines,
                  std::function<bool(std::string&)> predicament,
                  bool emptylast)
{
    GetFileNLines<std::fstream>(filepath, lines, predicament, emptylast, n_condend);
}

void GetFileLines(const sf::path& filepath,
                  VecNwstr& lines,
                  std::function<bool(std::wstring&)> predicament,
                  bool emptylast)
{
    GetFileNLines<std::wfstream>(filepath, lines, predicament, emptylast, wn_condend);
}

template <typename StreamType, typename LineType>
void GetFileSLines(const sf::path& filepath,
                   Vec<LineType>& lines,
                   std::function<bool(LineType&)> predicament,
                   bool emptylast,
                   const Vec<LineType>& end_exclusion)
{
    lines.clear();
    StreamType stream(filepath);

    if (!stream.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                                 + "\"\nMessage: " + ec.message());
    }

    LineType line;
    lines.reserve(100000);

    while (std::getline(stream, line))
    {
        if (!predicament(line)) continue;

        lines.emplace_back(std::move(line));
    }

    ValidateEndLine(emptylast, lines, end_exclusion);
    lines.shrink_to_fit();
}

void GetFileLines(const sf::path& filepath,
                  VecStr& lines,
                  std::function<bool(std::string&)> predicament,
                  bool emptylast)
{
    GetFileSLines<std::fstream>(filepath, lines, predicament, emptylast, condend);
}

void GetFileLines(const sf::path& filepath,
                  VecWstr& lines,
                  std::function<bool(std::wstring&)> predicament,
                  bool emptylast)
{
    GetFileSLines<std::wfstream>(filepath, lines, predicament, emptylast, wcondend);
}

template <typename StreamType, typename RawType, typename LineType>
void GetFileNLines(const sf::path& filepath,
                   Vec<LineType>& lines,
                   bool emptylast,
                   const Vec<LineType>& end_exclusion)
{
    lines.clear();
    StreamType stream(filepath);

    if (!stream.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                                 + "\"\nMessage: " + ec.message());
    }

    size_t linenum = 0;
    RawType line;
    lines.reserve(100000);

    if (std::getline(stream, line))
    {
        auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<sf::path>>(filepath);
        lines.emplace_back(std::move(line), ++linenum, filepath_ptr);

        while (std::getline(stream, line))
        {
            lines.emplace_back(std::move(line), ++linenum, filepath_ptr.get());
        }

        ValidateEndLine(emptylast, lines, end_exclusion);
    }

    lines.shrink_to_fit();
}

void GetFileLines(const sf::path& filepath, VecNstr& lines, bool emptylast)
{
    GetFileNLines<std::fstream, std::string>(filepath, lines, emptylast, n_condend);
}

void GetFileLines(const sf::path& filepath, VecNwstr& lines, bool emptylast)
{
    GetFileNLines<std::wfstream, std::wstring>(filepath, lines, emptylast, wn_condend);
}

template <typename StreamType, typename LineType>
void GetFileSLines(const sf::path& filepath,
                   Vec<LineType>& lines,
                   bool emptylast,
                   const Vec<LineType>& end_exclusion)
{
    lines.clear();
    StreamType stream(filepath);

    if (!stream.is_open())
    {
        std::error_code ec(errno, std::system_category());
        throw std::runtime_error("Failed to open file: \"" + nemesis::to_utf8_string(filepath)
                                 + "\"\nMessage: " + ec.message());
    }

    LineType line;
    lines.reserve(100000);

    while (std::getline(stream, line))
    {
        lines.emplace_back(std::move(line));
    }

    ValidateEndLine(emptylast, lines, end_exclusion);
    lines.shrink_to_fit();
}

void GetFileLines(const sf::path& filepath, VecStr& lines, bool emptylast)
{
    GetFileSLines<std::fstream>(filepath, lines, emptylast, condend);
}

void GetFileLines(const sf::path& filepath, VecWstr& lines, bool emptylast)
{
    GetFileSLines<std::wfstream>(filepath, lines, emptylast, wcondend);
}
