#pragma once

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

    // Creating a copy of the compiler in safe path because hkxcmd will not work if placed in a path that has special character(s)
    HkxCompiler();

    // Removing all files in temporary folder containing the compiler and hkx files
    ~HkxCompiler();

    static HkxCompiler compiler;

public:
    static bool ConvertToHkx(fpath xmlfile, fpath hkxfile);
    static bool hkxcmdProcess(fpath xmlfile, fpath hkxfile, bool last = false);
    static bool hkxcmdXmlInput(fpath hkxfile, VecStr& fileline);
    static bool hkxcmdXmlInput(fpath hkxfile, VecWstr& fileline);

private:
    static void ensureExtension(fpath& file1, const std::wstring& ext1, fpath& file2, const std::wstring& ext2);
    static std::string xmlDecompile(fpath hkxfile, fpath xmlfile, const HkxCompileCount& hkxcount);
};
