#ifndef RENEW_H_
#define RENEW_H_

#include <string>
#include <iostream>
#include <Windows.h>
#include <boost\filesystem.hpp>
#include <boost\regex.hpp>
#include <boost\algorithm\string.hpp>
#include "functionwriter.h"
#include "generator_utility.h"
#include "add animation\animationsetdata.h"

extern bool VanillaUpdate(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less<std::string>>>& newAnimDataSet, vecstr& projectList);
extern void ClearTempBehaviors();

bool GetPathLoop(std::string newPath, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less<std::string>>>& newAnimDataSet, vecstr& projectList);
bool VanillaDisassemble(std::string path, std::string filename, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);
bool AnimDataDisassemble(std::string path, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);
bool AnimDataSetDisassemble(std::string path, std::unordered_map<std::string, std::map<std::string, vecstr, alphanum_less<std::string>>>& newAnimDataSet, vecstr& projectList);

#endif