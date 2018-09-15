#pragma once
#include <string>
#include <iostream>
#include <boost\filesystem.hpp>

extern bool SSE;

class DataPath
{
	std::string dataPath;
public:
	DataPath();
	std::string GetDataPath();
};
