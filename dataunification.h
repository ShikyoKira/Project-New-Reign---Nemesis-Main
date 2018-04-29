#ifndef DATAUNIFICATION_H_
#define DATAUNIFICATION_H_

#include <iostream>
#include <Windows.h>
#include <thread>
#include <boost\regex.hpp>
#include <boost\filesystem.hpp>
#include <boost\algorithm\string.hpp>
#include "functions\functionupdate.h"
#include "functions\functionwriter.h"

void SeparateMod(std::string directory, std::string modecode, vecstr behaviorfilelist, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);

extern void JoiningEdits(std::string directory, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);
extern void CombiningFiles(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);
extern bool newAnimUpdate(std::string sourcefolder, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);

bool newAnimUpdateExt(std::string folderpath, std::string behaviorfile, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);
bool newAnimDataUpdateExt(std::string folderpath, std::string modcode, std::string characterfile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);

#endif