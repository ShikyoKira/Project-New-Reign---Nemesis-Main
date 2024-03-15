#include "utilities/algorithm.h"
#include "utilities/writetextfile.h"

FileWriter::FileWriter(const std::filesystem::path& filepath,
                       Encoding encoding,
                       VecWstr args)
    : filepath(filepath)
{
    std::wstring mode;

    switch (encoding)
    {
        case FileWriter::ASCII:
            mode = L"ccs=ASCII";
            break;
        case FileWriter::UTF8:
            mode = L"ccs=UTF-8";
            break;
        case FileWriter::UTF16:
            mode = L"ccs=UTF-16";
            break;
        case FileWriter::UTF32:
            mode = L"ccs=UTF-32";
            break;
        default:
            throw std::runtime_error("Unsupported encoding format. File path (" + filepath.string() + ")");
    }

    for (auto each : args)
    {
        mode.append(L"," + each);
    }

    _wfopen_s(&file, filepath.wstring().c_str(), mode.c_str());
}

FileWriter::~FileWriter()
{
    if (file)
    {
        fflush(file);
        fclose(file);
    }
}

const std::filesystem::path& FileWriter::GetFilePath() const
{
    return filepath;
}

bool FileWriter::is_open() const
{
    return file;
}

void FileWriter::Close()
{
    Lockless lock(filelock);

    if (file == nullptr) return;

    fflush(file);
    fclose(file);
    file = nullptr;
}

void FileWriter::LockFreeWrite(const char* line)
{
    fprintf_s(file, "%s", line);
}

void FileWriter::LockFreeWrite(const wchar_t* line)
{
    fwprintf_s(file, L"%s", line);
}

void FileWriter::LockFreeWrite(const std::string& line)
{
    fprintf_s(file, "%s", line.c_str());
}

void FileWriter::LockFreeWrite(const std::wstring& line)
{
    fwprintf_s(file, L"%s", line.c_str());
}

void FileWriter::LockFreeWriteLine(const char* line)
{
    fprintf_s(file, "%s\n", line);
}

void FileWriter::LockFreeWriteLine(const wchar_t* line)
{
    fwprintf_s(file, L"%s\n", line);
}

void FileWriter::LockFreeWriteLine(const std::string& line)
{
    fprintf_s(file, "%s\n", line.c_str());
}

void FileWriter::LockFreeWriteLine(const std::wstring& line)
{
    fwprintf_s(file, L"%s\n", line.c_str());
}

void FileWriter::WriteLines(const VecStr& lines)
{
    for (auto& line : lines)
    {
        LockFreeWrite(line.c_str());
    }
}

void FileWriter::WriteLines(const VecWstr& lines)
{
    for (auto& line : lines)
    {
        LockFreeWrite(line.c_str());
    }
}

void FileWriter::WriteLines(const VecNstr& lines)
{
    for (auto& line : lines)
    {
        LockFreeWrite(line.c_str());
    }
}

FileWriter& FileWriter::operator<<(const char* input)
{
    Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}

FileWriter& FileWriter::operator<<(const wchar_t* input)
{
    Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}

FileWriter& FileWriter::operator<<(const std::string& input)
{
    Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}

FileWriter& FileWriter::operator<<(const std::wstring& input)
{
    Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}

FileWriter& FileWriter::operator<<(const nemesis::Line& input)
{
    Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}
