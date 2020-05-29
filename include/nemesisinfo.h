#pragma once

#include <iostream>
#include <string>

#include "Global.h"

#include "utilities/algorithm.h"

extern bool SSE;

class NemesisInfo
{
    bool first = true;
    std::wstring dataPath;
    std::wstring stageDirectory;
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

    const std::wstring& GetDataPath() const;
    const std::wstring& GetStagePath() const;
    const std::string& GetDataPathA() const;
    const std::string& GetStagePathA() const;
    const uint& GetMaxAnim() const;
    const bool& IsFirst() const;
    const uint& GetWidth() const;
    const uint& GetHeight() const;
    const uint& GetModNameWidth() const;
    const uint& GetAuthorWidth() const;
    const uint& GetPriorityWidth() const;

    void setFirst(bool _first);
    void setWidth(uint _width);
    void setHeight(uint _height);
    void setModNameWidth(uint _width);
    void setAuthorWidth(uint _width);
    void setPriorityWidth(uint _width);
};
