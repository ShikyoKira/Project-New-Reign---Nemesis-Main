#pragma once
#include <string>
#include <iostream>
#include <boost\filesystem.hpp>

extern bool SSE;

class NemesisInfo
{
	bool first = true;
	std::string dataPath;
	unsigned int maxAnim = 10000;

public:
	NemesisInfo();

	std::string GetDataPath();
	unsigned int GetMaxAnim();
	bool IsFirst();

	void setFirst(bool _first);
	void iniFileUpdate();
};
