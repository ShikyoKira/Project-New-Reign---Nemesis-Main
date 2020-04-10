#pragma once

#include "Global.h"
#include <iostream>
#include <string>

#include "utilities/algorithm.h"

extern bool SSE;

class NemesisInfo
{
    bool first = true;
    std::string dataPath;
    uint maxAnim       = 10000;
    uint width         = 526;
    uint height        = 750;
    uint modNameWidth  = 380;
    uint authorWidth   = 75;
    uint priorityWidth = 47;

    void setup();

public:
    NemesisInfo();
    NemesisInfo(bool& exception);

    std::string GetDataPath();
    uint GetMaxAnim();
    bool IsFirst();
    uint GetWidth();
    uint GetHeight();
    uint GetModNameWidth();
    uint GetAuthorWidth();
    uint GetPriorityWidth();

    void setFirst(bool _first);
    void setWidth(uint _width);
    void setHeight(uint _height);
    void setModNameWidth(uint _width);
    void setAuthorWidth(uint _width);
    void setPriorityWidth(uint _width);
    void iniFileUpdate();
};
