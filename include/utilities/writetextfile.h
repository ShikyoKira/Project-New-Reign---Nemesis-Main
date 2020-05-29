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
    FileWriter(const char* filename)
        : filepath(filename)
    {
        fopen_s(&file, filename, "w");
    }

    FileWriter(std::string filename)
        : filepath(filename)
    {
        fopen_s(&file, filename.c_str(), "w");
    }

    FileWriter(std::filesystem::path filename)
        : filepath(filename)
    {
        fopen_s(&file, filename.string().c_str(), "w");
    }

    ~FileWriter()
    {
        if (file)
        {
            fflush(file);
            fclose(file);
        }
    }

    std::filesystem::path GetFilePath() const
    {
        return filepath;
    }

    bool is_open()
    {
        return file;
    }

    FileWriter& operator<<(const std::wstring& input)
    {
        Lockless lock(filelock);
        fwprintf(file, L"%s", input.c_str());
        return *this;
    }

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

    void LockFreeWrite(std::string line)
    {
        fprintf(file, "%s", (line + "\n").c_str());
    }

    void LockFreeWrite(char* line)
    {
        fprintf(file, "%s", line + '\n');
    }
};

#endif
