#ifndef DATAUNIFICATION_H_
#define DATAUNIFICATION_H_

#include <iostream>
#include <Windows.h>
#include <thread>
#include <boost\regex.hpp>
#include <boost\filesystem.hpp>
#include "functions\functionupdate.h"
#include "functions\functionwriter.h"

void SeparateMod(std::string dr, std::string f1, vecstr f2, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);

extern void JoiningEdits(std::string directory, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);
extern void CombiningFiles(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);
extern bool newAnimUpdate(std::string sourcefolder, std::string targetfolder, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);

#endif