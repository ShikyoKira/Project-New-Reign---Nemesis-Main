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

extern bool FunctionUpdate(std::string modcode, std::string f2, std::string f3, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);
extern bool AnimDataUpdate(std::string modcode, std::string animdatafile, std::string characterfile, std::string filepath, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader, bool isNewCharacter);
extern bool AnimDataSetUpdate(std::string modcode, std::string animsetdatafile, std::string projectsource, std::string projectfile, std::string filepath, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less<std::string>>>& newAnimDataSet, vecstr& projectList, bool isNewProject);

#endif