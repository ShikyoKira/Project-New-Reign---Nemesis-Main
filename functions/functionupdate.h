#ifndef FUNCTIONUPDATE_H_
#define FUNCTIONUPDATE_H_

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include <boost\regex.hpp>
#include "functionwriter.h"
#include "generator_utility.h"
#include "add animation\animationdata.h"
#include "add animation\animationsetdata.h"

bool FunctionUpdate(std::string modcode, std::string f2, std::string f3, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile,
	std::unordered_map<std::string, std::string>& lastUpdate);

bool AnimDataUpdate(std::string modcode, std::string animdatafile, std::string characterfile, std::string filepath, MasterAnimData& animData,
	bool isNewCharacter, std::unordered_map<std::string, std::string>& lastUpdate);

bool AnimSetDataUpdate(std::string modcode, std::string animsetdatafile, std::string projectsource, std::string projectfile, std::string filepath,
	MasterAnimSetData& animSetData, bool isNewProject, std::unordered_map<std::string, std::string>& lastUpdate);

#endif