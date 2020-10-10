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

public:
    void getlines(VecStr& storeline);

    friend struct AnimSetData;
};

void getLinkedLines(const nemesis::LinkedVar<AnimSetCRC32Pack>& linkedcrc32, VecStr& storeline);
