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

extern bool VanillaUpdate(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);

bool GetPathLoop(std::string newPath, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);
bool VanillaDisassemble(std::string path, std::string filename, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);
bool AnimDataDisassemble(std::string path, std::unordered_map<std::string, std::unordered_map<std::string, vecstr>>& newAnimData, vecstr& animDataChar, std::unordered_map<std::string, vecstr>& animDataHeader);

#endif