#include "core/memoryfile.h"

#include "utilities/writetextfile.h"

nemesis::MemoryFile::MemoryFile(const std::filesystem::path& filepath)
    : filepath(filepath)
{
    filepath_ptr = std::make_shared<nemesis::SharableWrapper<std::filesystem::path>>(filepath);
}

void nemesis::MemoryFile::Read()
{
    contents = nemesis::File(filepath).GetLines();
}

VecNstr nemesis::MemoryFile::GetLines() const noexcept
{
    return contents;
}

void nemesis::MemoryFile::Clear()
{
    contents.clear();
}

void nemesis::MemoryFile::Write(const VecStr& lines)
{
    contents.clear();
    
    if (lines.empty()) return;
    
    contents          = {nemesis::Line(lines.front(), 1, filepath_ptr)};
    auto* path_rawptr = filepath_ptr.get();

    for (size_t i = 1; i < lines.size(); ++i)
    {
        contents.emplace_back(lines[i], i + 1, path_rawptr);
    }
}

void nemesis::MemoryFile::Write(const VecNstr& lines)
{
    contents.clear();

    if (lines.empty()) return;

    contents = {nemesis::Line(lines.front().ToString(), 1, filepath_ptr)};
    auto* path_rawptr = filepath_ptr.get();

    for (size_t i = 1; i < lines.size(); ++i)
    {
        contents.emplace_back(lines[i].ToString(), i + 1, path_rawptr);
    }
}

void nemesis::MemoryFile::Write(const std::string& line)
{
    contents = {nemesis::Line(line, 1, filepath_ptr)};
}

void nemesis::MemoryFile::Write(const std::string_view& vline)
{
    Write(std::string(vline));
}

void nemesis::MemoryFile::Write(const nemesis::Line& line)
{
    contents = {nemesis::Line(line.ToString(), 1, filepath_ptr)};
}

void nemesis::MemoryFile::Append(const VecStr& lines)
{
    if (contents.empty())
    {
        Write(lines);
        return;
    }

    auto* path_rawptr = filepath_ptr.get();
    size_t linenum    = contents.size();

    for (auto& line : lines)
    {
        contents.emplace_back(line, ++linenum, path_rawptr);
    }
}

void nemesis::MemoryFile::Append(const VecNstr& lines)
{
    if (contents.empty())
    {
        Write(lines);
        return;
    }

    auto* path_rawptr = filepath_ptr.get();
    size_t linenum    = contents.size();

    for (auto& line : lines)
    {
        contents.emplace_back(line.ToString(), ++linenum, path_rawptr);
    }
}

void nemesis::MemoryFile::Append(const std::string& line)
{
    if (contents.empty())
    {
        Write(line);
        return;
    }

    contents.emplace_back(line, contents.size() + 1, filepath_ptr.get());
}

void nemesis::MemoryFile::Append(const std::string_view& vline)
{
    Write(std::string(vline));
}

void nemesis::MemoryFile::Append(const nemesis::Line& line)
{
    if (contents.empty())
    {
        Write(line);
        return;
    }

    contents.emplace_back(line.ToString(), contents.size() + 1, filepath_ptr.get());
}

void nemesis::MemoryFile::Save() const
{
    FileWriter writer(filepath);
    
    for (auto& line : contents)
    {
        writer.LockFreeWriteLine(line);
    }
}
