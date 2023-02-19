#include "base/file.h"

#include "utilities/stringextension.h"

namespace sf = std::filesystem;

const VecStr nemesis::File::condend      = {"<!-- CONDITION END -->", "<!-- CLOSE -->"};
const VecWstr nemesis::File::wcondend    = {L"<!-- CONDITION END -->", L"<!-- CLOSE -->"};
const VecNstr nemesis::File::n_condend   = {"<!-- CONDITION END -->", "<!-- CLOSE -->"};
const VecNwstr nemesis::File::wn_condend = {L"<!-- CONDITION END -->", L"<!-- CLOSE -->"};

nemesis::File::File(const std::filesystem::path& filepath)
    : filepath(filepath)
{
}

nemesis::File::FileType nemesis::File::GetType() const noexcept
{
    return type;
}

std::wstring nemesis::File::GetLastModified() const
{
    struct _stat64 buf;
    _wstat64(filepath.c_str(), &buf);
    wchar_t buffer[26];
    _wctime64_s(buffer, 26, &buf.st_mtime);
    buffer[24] = '\0';
    return buffer;
}

void nemesis::File::SetFilePath(const std::filesystem::path& path)
{
    filepath = path;
    type     = GetFileType(filepath);
}

std::string_view nemesis::File::GetFileClassName() const
{
    return classname;
}

std::filesystem::path nemesis::File::GetFilePath() const
{
    return filepath;
}

VecNstr nemesis::File::GetLines(bool emptylast) const
{
    VecNstr lines;
    TryGetLines(lines, emptylast);
    return lines;
}

VecNstr nemesis::File::GetLines(std::function<const std::string(const std::string&)> selector,
                                bool emptylast) const
{
    VecNstr lines;
    TryGetLines(lines, selector, emptylast);
    return lines;
}

VecNstr nemesis::File::GetLines(std::function<bool(std::string&)> predicament, bool emptylast) const
{
    VecNstr lines;
    TryGetLines(lines, predicament, emptylast);
    return lines;
}

bool nemesis::File::TryGetLines(VecNstr& lines, bool emptylast) const
{
    return TryGetLines(
        lines, [&](std::string& line) -> bool { return true; }, emptylast);
}

bool nemesis::File::TryGetLines(VecNstr& lines,
                                std::function<const std::string(const std::string&)> selector,
                                bool emptylast) const
{
    return TryGetLines(
        lines,
        [&](std::string& line) -> bool {
            line = selector(line);
            return true;
        },
        emptylast);
}

bool nemesis::File::TryGetLines(VecNstr& lines,
                                std::function<bool(std::string&)> predicament,
                                bool emptylast) const
{
    size_t num        = 0;
    auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<std::filesystem::path>>(filepath);
    return TryGetLines<nemesis::Line, std::string>(
        lines,
        predicament,
        [&](const std::string& line) -> nemesis::Line {
            return num != 0 ? nemesis::Line(line, ++num, filepath_ptr.get())
                            : nemesis::Line(line, ++num, filepath_ptr);
        },
        emptylast,
        n_condend);
}

bool nemesis::File::TryGetLines(VecNwstr& lines, bool emptylast) const
{
    return TryGetLines(
        lines, [&](std::wstring& line) -> bool { return true; }, emptylast);
}

bool nemesis::File::TryGetLines(VecNwstr& lines,
                                std::function<const std::wstring(const std::wstring&)> selector,
                                bool emptylast) const
{
    return TryGetLines(
        lines,
        [&](std::wstring& line) -> bool {
            line = selector(line);
            return true;
        },
        emptylast);
}

bool nemesis::File::TryGetLines(VecNwstr& lines,
                                std::function<bool(std::wstring&)> predicament,
                                bool emptylast) const
{
    size_t num        = 0;
    auto filepath_ptr = std::make_shared<nemesis::SharableWrapper<std::filesystem::path>>(filepath);
    return TryGetLines<nemesis::Wline, std::wstring>(
        lines,
        predicament,
        [&](const std::wstring& line) -> nemesis::Wline {
            return num != 0 ? nemesis::Wline(line, ++num, filepath_ptr.get())
                            : nemesis::Wline(line, ++num, filepath_ptr);
        },
        emptylast,
        wn_condend);
}

bool nemesis::File::TryGetLines(VecStr& lines, bool emptylast) const
{
    return TryGetLines(
        lines, [&](std::string& line) { return true; }, emptylast);
}

bool nemesis::File::TryGetLines(VecStr& lines,
                                std::function<const std::string(const std::string&)> selector,
                                bool emptylast) const
{
    return TryGetLines(
        lines,
        [&](std::string& line) -> bool {
            line = selector(line);
            return true;
        },
        emptylast);
}

bool nemesis::File::TryGetLines(VecStr& lines,
                                std::function<bool(std::string&)> predicament,
                                bool emptylast) const
{
    return TryGetLines(lines, predicament, emptylast, condend);
}

bool nemesis::File::TryGetLines(VecWstr& lines, bool emptylast) const
{
    return TryGetLines(
        lines, [&](std::wstring& line) -> bool { return true; }, emptylast);
}

bool nemesis::File::TryGetLines(VecWstr& lines,
                                std::function<const std::wstring(const std::wstring&)> selector,
                                bool emptylast) const
{
    return TryGetLines(
        lines,
        [&](std::wstring& line) -> bool {
            line = selector(line);
            return true;
        },
        emptylast);
}

bool nemesis::File::TryGetLines(VecWstr& lines,
                                std::function<bool(std::wstring&)> predicament,
                                bool emptylast) const
{
    return TryGetLines(lines, predicament, emptylast, wcondend);
}

nemesis::File::FileType nemesis::File::GetFileType(const std::filesystem::path& path)
{
    if (sf::is_directory(path) || path.extension().wstring() != L".txt") return FileType::NONE;

    std::string filename = path.stem().string();

    if (StringStartWith_NC(filename, "FNIS_"))
    {
        return StringEndWith_NC(filename, "_List") ? FileType::FNIS_LIST : FileType::NONE;
    }

    if (StringEndWith_NC(filename, "_Template")) return FileType::NEMESIS_TEMPLATE;

    if (!StringStartWith_NC(filename, "Nemesis_")) return FileType::NONE;

    if (StringEndWith_NC(filename, "_List")) return FileType::NEMESIS_LIST;

    return FileType::NONE;
}
