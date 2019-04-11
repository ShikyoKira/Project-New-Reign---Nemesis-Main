#pragma once
#include <string>
#include <iostream>
#include <boost\filesystem.hpp>

extern bool SSE;

class NemesisInfo
{
	std::string dataPath;
	unsigned int maxAnim = 10000;

public:
	NemesisInfo();
	std::string GetDataPath();
	unsigned int GetMaxAnim();
};
