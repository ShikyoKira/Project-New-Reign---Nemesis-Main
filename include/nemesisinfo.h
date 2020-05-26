#pragma once

#include <iostream>
#include <string>

#include "Global.h"

#include "utilities/algorithm.h"

extern bool SSE;

class NemesisInfo
{
    bool first = true;
    std::string dataPath;
    std::string stageDirectory;
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

    void iniFileUpdate();

    std::string GetDataPath() const;
    std::string GetStagePath() const;
    uint GetMaxAnim() const;
    bool IsFirst() const;
    uint GetWidth() const;
    uint GetHeight() const;
    uint GetModNameWidth() const;
    uint GetAuthorWidth() const;
    uint GetPriorityWidth() const;

    void setFirst(bool _first);
    void setWidth(uint _width);
    void setHeight(uint _height);
    void setModNameWidth(uint _width);
    void setAuthorWidth(uint _width);
    void setPriorityWidth(uint _width);
};
