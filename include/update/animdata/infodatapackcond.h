#pragma once

#include "utilities/linkedvar.h"

struct InfoDataPack_Condt
{
    using ModCode = std::string;

    std::string uniquecode;
    std::string duration;

    VecStr motiondata;
    VecStr rotationdata;

    InfoDataPack_Condt() {}
    InfoDataPack_Condt(const VecStr& storeline, size_t linenum = 0);

    void getlines(VecStr& storeline);
};

void getlinkedline(const nemesis::LinkedVar<InfoDataPack_Condt>& linkedanimdata, VecStr& storeline);
