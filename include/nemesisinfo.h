#pragma once
#include <iostream>
#include <string>

#include "utilities/algorithm.h"

extern bool SSE;

class NemesisInfo
{
    bool first = true;
    std::string dataPath;
    unsigned int maxAnim       = 10000;
    unsigned int width         = 526;
    unsigned int height        = 750;
    unsigned int modNameWidth  = 380;
    unsigned int authorWidth   = 75;
    unsigned int priorityWidth = 47;

    void setup();

public:
    NemesisInfo();
    NemesisInfo(bool& exception);

    std::string GetDataPath();
    unsigned int GetMaxAnim();
    bool IsFirst();
    unsigned int GetWidth();
    unsigned int GetHeight();
    unsigned int GetModNameWidth();
    unsigned int GetAuthorWidth();
    unsigned int GetPriorityWidth();

    void setFirst(bool _first);
    void setWidth(unsigned int _width);
    void setHeight(unsigned int _height);
    void setModNameWidth(unsigned int _width);
    void setAuthorWidth(unsigned int _width);
    void setPriorityWidth(unsigned int _width);
    void iniFileUpdate();
};
