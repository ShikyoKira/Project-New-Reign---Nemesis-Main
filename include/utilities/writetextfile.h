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
private:
    FILE* file;
    std::filesystem::path filepath;
    std::atomic_flag filelock{};

public:
    FileWriter(std::filesystem::path filename, VecWstr args = { L"ccs=UTF-8" });

    ~FileWriter();

    const std::filesystem::path& GetFilePath() const;

    bool is_open() const;

    void LockFreeWrite(const char* line);
    void LockFreeWrite(const wchar_t* line);
    void LockFreeWrite(const std::string& line);
    void LockFreeWrite(const std::wstring& line);

    FileWriter& operator<<(const char* input);
    FileWriter& operator<<(const wchar_t* input);
    FileWriter& operator<<(const std::string& input);
    FileWriter& operator<<(const std::wstring& input);

    template <typename T>
    FileWriter& operator<<(const T& input)
    {
        std::stringstream sstream;
        sstream << input;
        Lockless lock(filelock);
        fprintf(file, "%s", sstream.str().c_str());
        return *this;
    }

    template <typename T>
    FileWriter& operator<<(const T* input)
    {
        std::stringstream sstream;
        sstream << input;
        Lockless lock(filelock);
        fprintf(file, "%s", sstream.str().c_str());
        return *this;
    }
};

#endif
