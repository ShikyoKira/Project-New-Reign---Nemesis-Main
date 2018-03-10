#pragma once
#include <string>
#include <iostream>
#include <boost\filesystem.hpp>

class DataPath
{
	std::string dataPath;
public:
	DataPath();
	std::string GetDataPath();
};