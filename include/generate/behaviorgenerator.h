#ifndef BEHAVIORGENERATOR_H_
#define BEHAVIORGENERATOR_H_

#include <string>
#include <filesystem>

#include "Global.h"

class HkxCompileCount;

class HkxCompiler
{
    using fpath = std::filesystem::path;

    int cur;
    std::string tempdir;
    std::string tempcompiler;

public:
    // Creating a copy of the compiler in safe path because hkxcmd will not work if placed in a path that has special character(s)
    HkxCompiler();

    // Removing all files in temporary folder containing the compiler and hkx files
    ~HkxCompiler();

    bool hkxcmdProcess(fpath xmlfile, fpath hkxfile, bool last = false) const;
    bool hkxcmdXmlInput(fpath hkxfile, VecStr& fileline) const;
    bool hkxcmdXmlInput(fpath hkxfile, VecWstr& fileline) const;

private:
    void ensureExtension(fpath& file1, const std::wstring& ext1, fpath& file2, const std::wstring& ext2) const;
    std::string xmlDecompile(fpath hkxfile, fpath xmlfile, const HkxCompileCount& hkxcount) const;
};

#endif