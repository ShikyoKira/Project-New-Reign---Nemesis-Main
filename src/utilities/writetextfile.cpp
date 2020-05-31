#include "utilities/algorithm.h"
#include "utilities/writetextfile.h"

using namespace std;

FileWriter::FileWriter(filesystem::path filename, VecWstr args)
    : filepath(filename)
{
    wstring mode = L"w";
    
    for (auto each : args)
    {
        mode.append(L"," + each);
    }

    _wfopen_s(&file, filename.wstring().c_str(), mode.c_str());
}

FileWriter::~FileWriter()
{
    if (file)
    {
        fflush(file);
        fclose(file);
    }
}

const filesystem::path& FileWriter::GetFilePath() const
{
    return filepath;
}

bool FileWriter::is_open() const
{
    return file;
}

void FileWriter::LockFreeWrite(const char* line)
{
    fwprintf(file, L"%s", nemesis::transform_to<wstring>(string(line + '\n')).c_str());
}

void FileWriter::LockFreeWrite(const wchar_t* line)
{
    fwprintf(file, L"%s", line + L'\n');
}

void FileWriter::LockFreeWrite(const string& line)
{
    fwprintf(file, L"%s", nemesis::transform_to<wstring>(line + "\n").c_str());
}

void FileWriter::LockFreeWrite(const std::wstring& line)
{
    fwprintf(file, L"%s", (line + L"\n").c_str());
}

FileWriter& FileWriter::operator<<(const char* input)
{
    Lockless lock(filelock);
    fwprintf(file, L"%s", nemesis::transform_to<wstring>(string(input)).c_str());
    return *this;
}

FileWriter& FileWriter::operator<<(const wchar_t* input)
{
    Lockless lock(filelock);
    fwprintf(file, L"%s", input);
    return *this;
}

FileWriter& FileWriter::operator<<(const string& input)
{
    Lockless lock(filelock);
    fwprintf(file, L"%s", nemesis::transform_to<wstring>(input).c_str());
    return *this;
}

FileWriter& FileWriter::operator<<(const wstring& input)
{
    Lockless lock(filelock);
    fwprintf(file, L"%s", input.c_str());
    return *this;
}
