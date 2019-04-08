#ifndef FUNCTIONUPDATE_H_
#define FUNCTIONUPDATE_H_

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <boost\regex.hpp>
#include "writetextfile.h"
#include "generator_utility.h"
#include "add animation\animationdata.h"
#include "add animation\animationsetdata.h"

class NodeU
{
	std::atomic_flag nodelock = ATOMIC_FLAG_INIT;

	bool NodeUpdate(std::string modcode, std::string f2, std::string f3, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, SSMap& stateID, SSMap& parent,
		std::unordered_map<std::string, vecstr>& statelist, std::unordered_map<std::string, std::string>& lastUpdate);

public:
	bool FunctionUpdate(std::string modcode, std::string f2, std::string f3, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, SSMap& stateID, SSMap& parent,
	std::unordered_map<std::string, vecstr>& statelist, std::unordered_map<std::string, std::string>& lastUpdate);
};

bool AnimDataUpdate(std::string modcode, std::string animdatafile, std::string characterfile, std::string filepath, MasterAnimData& animData,
	bool isNewCharacter, std::unordered_map<std::string, std::string>& lastUpdate, bool& openAnim, bool& openInfo);

bool AnimSetDataUpdate(std::string modcode, std::string animsetdatafile, std::string projectsource, std::string projectfile, std::string filepath,
	MasterAnimSetData& animSetData, bool isNewProject, std::unordered_map<std::string, std::string>& lastUpdate);

#endif