#ifndef FILEWRITER_H_
#define FILEWRITER_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

#include "utilities/atomiclock.h"

class FileWriter
{
    enum Encoding
    {
        ASCII,
        UTF8,
        UTF16,
        UTF32
    };

private:
    FILE* file;
    std::filesystem::path filepath;
    std::atomic_flag filelock{};

public:
    FileWriter(const std::filesystem::path& filename,
               Encoding encoding = Encoding::ASCII,
               VecWstr args      = {L"w"});

    ~FileWriter();

    const std::filesystem::path& GetFilePath() const;

    bool is_open() const;

    void Close();

    void LockFreeWrite(const char* line);
    void LockFreeWrite(const wchar_t* line);
    void LockFreeWrite(const std::string& line);
    void LockFreeWrite(const std::wstring& line);
    
    void LockFreeWriteLine(const char* line);
    void LockFreeWriteLine(const wchar_t* line);
    void LockFreeWriteLine(const std::string& line);
    void LockFreeWriteLine(const std::wstring& line);

    void WriteLines(const VecStr& lines);
    void WriteLines(const VecWstr& lines);
    void WriteLines(const VecNstr& lines);

    FileWriter& operator<<(const char* input);
    FileWriter& operator<<(const wchar_t* input);
    FileWriter& operator<<(const std::string& input);
    FileWriter& operator<<(const std::wstring& input);
    FileWriter& operator<<(const nemesis::Line& input);

    template <typename T>
    FileWriter& operator<<(const T& input)
    {
        std::wstringstream sstream;
        sstream << input;
        Lockless lock(filelock);
        LockFreeWrite(sstream.str().c_str());
        return *this;
    }

    template <typename T>
    FileWriter& operator<<(const T* input)
    {
        std::wstringstream sstream;
        sstream << input;
        Lockless lock(filelock);
        LockFreeWrite(sstream.str().c_str());
        return *this;
    }
};

#endif
