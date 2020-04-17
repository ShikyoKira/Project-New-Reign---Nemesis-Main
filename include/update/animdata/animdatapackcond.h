#pragma once

#include "utilities/linkedvar.h"

struct AnimDataPack_Condt
{
    using ModCode = std::string;

    // anim data
    std::string name;
    nemesis::LinkedVar<std::string> uniquecode;
    nemesis::LinkedVar<std::string> unknown1;
    nemesis::LinkedVar<std::string> unknown2;
    nemesis::LinkedVar<std::string> unknown3;

    std::vector<nemesis::LinkedVar<std::string>> eventname;

    AnimDataPack_Condt() {}
    AnimDataPack_Condt(const VecStr& storeline, size_t linenum = 0);

    void getlines(VecStr& storeline);
};
