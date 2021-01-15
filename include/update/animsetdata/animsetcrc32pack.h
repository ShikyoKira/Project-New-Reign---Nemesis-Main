#pragma once

#include <string>

#include "Global.h"

#include "utilities/linkedvar.h"

struct AnimSetCRC32Pack
{
    using crc32 = std::string;

private:
    nemesis::LinkedVar<crc32> filepath;
    nemesis::LinkedVar<crc32> filename;
    nemesis::LinkedVar<std::string> extension;

    bool Validation(const nemesis::Line& line);

public:
    void getlines(VecStr& storeline);

    bool AddFilePath(const nemesis::Line& _filepath);
    bool AddFileName(const nemesis::Line& _filename);
    bool AddExtension(const nemesis::Line& _extension);

    friend struct AnimSetData;
};

void getLinkedLines(const nemesis::LinkedVar<AnimSetCRC32Pack>& linkedcrc32, VecStr& storeline);
