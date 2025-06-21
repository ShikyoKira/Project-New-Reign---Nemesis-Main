#include "Utilities/Algorithm.h"
#include "Utilities/FileWriter.h"

FileWriter::FileWriter(const std::filesystem::path& filepath, Encoding encoding, const std::wstring& args)
    : filepath(filepath)
{
    std::wstring mode = nemesis::to_lower_copy(args);

    switch (encoding)
    {
        case FileWriter::ASCII:
            break;
        case FileWriter::UTF8:
        case FileWriter::UTF16:
        case FileWriter::UNICODE_:
        {
            if (mode.find(L"w") == NOT_FOUND || std::filesystem::exists(filepath)) break;

            Vec<unsigned char> bom;
            if (encoding == FileWriter::UTF8)
            {
                bom = {0xEF, 0xBB, 0xBF};
            }
            else
            {
                bom = {0xFF, 0xFE};
            }

            FILE* tempfile = nullptr;
            _wfopen_s(&tempfile, filepath.wstring().c_str(), L"wb");

            fwrite(bom.data(), sizeof(bom), 1, tempfile);
            fclose(tempfile);

            if (mode.find(L"a") == NOT_FOUND)
            {
                std::replace(mode.begin(), mode.begin(), L'w', L'a');
            }
            else
            {
                mode.erase(mode.find(L"w"), 1);
            }

            break;
        }
        default:
            throw std::runtime_error("Unsupported encoding format. (File: " + filepath.string() + ")");
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
    nemesis::Lockless lock(filelock);

    if (!file) return;

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
    nemesis::Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}

FileWriter& FileWriter::operator<<(const wchar_t* input)
{
    nemesis::Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}

FileWriter& FileWriter::operator<<(const std::string& input)
{
    nemesis::Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}

FileWriter& FileWriter::operator<<(const std::wstring& input)
{
    nemesis::Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}

FileWriter& FileWriter::operator<<(const nemesis::Line& input)
{
    nemesis::Lockless lock(filelock);
    LockFreeWrite(input);
    return *this;
}
