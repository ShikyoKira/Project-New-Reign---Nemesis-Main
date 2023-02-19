#pragma once

#include <iostream>
#include <string>

#include "Global.h"

#include "utilities/algorithm.h"

extern bool SSE;

class NemesisInfo
{
    static NemesisInfo* nemesisinfo;
    static std::atomic_flag lock;

    bool forceDirectory = false;
    bool exception = false;
    bool first = true;

    std::filesystem::path dataPath;
    std::filesystem::path stageDirectory;
    
    size_t maxAnim       = 10000;
    size_t width         = 526;
    size_t height        = 750;
    size_t modNameWidth  = 380;
    size_t authorWidth   = 75;
    size_t priorityWidth = 47;
    size_t timeout_timer = 3000;

    void setup();

    NemesisInfo();
    NemesisInfo(bool& exception);

    void ReadNemesisInfoFile();
    void SetDataPath();
    void PathValidation();

public:
    void iniFileUpdate();

    std::filesystem::path GetDataPath() const;
    std::filesystem::path GetStagePath() const;

    size_t GetMaxAnim() const;
    size_t GetWidth() const;
    size_t GetHeight() const;
    size_t GetModNameWidth() const;
    size_t GetAuthorWidth() const;
    size_t GetPriorityWidth() const;
    size_t GetTimeout() const;

    bool IsFirst() const;
    bool HasException() const;

    void SetFirst(bool _first);
    void SetWidth(size_t _width);
    void SetHeight(size_t _height);
    void SetModNameWidth(size_t _width);
    void SetAuthorWidth(size_t _width);
    void SetPriorityWidth(size_t _width);

    static NemesisInfo* GetInstance();
};
