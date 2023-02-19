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

    Vec<nemesis::LinkedVar<std::string>> eventname;

    AnimDataPack_Condt() {}
    AnimDataPack_Condt(const VecNstr& storeline);

    void getlines(VecStr& storeline);
};

void getLinkedLines(const nemesis::LinkedVar<AnimDataPack_Condt>& linkedanimdata, VecStr& storeline);
