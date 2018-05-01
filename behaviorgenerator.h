#ifndef BEHAVIORGENERATOR_H_
#define BEHAVIORGENERATOR_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <Windows.h>
#include <algorithm>
#include <sys/stat.h>
#include <boost\regex.hpp>
#include <boost\filesystem.hpp>
#include <boost\algorithm\string.hpp>
#include "functions\functionwriter.h"
#include "add animation\furniture.h"
#include "add animation\grouptemplate.h"
#include "add animation\optionlist.h"
#include "add animation\registeranimation.h"
#include "add animation\gettemplate.h"
#include "add animation\import.h"
#include "add animation\animationdata.h"
#include "generator_utility.h"
#include "filechecker.h"

typedef std::unordered_map<std::string, std::set<std::string>> mapSetString;

void hkxcmdOutput(std::string filename, std::string hkxfile);
void BehaviorCompilation(std::string directory, vecstr filelist, int curList, vecstr behaviorPriority, std::unordered_map<std::string, bool> chosenBehavior, getTemplate BehaviorTemplate, std::vector<std::unique_ptr<registerAnimation>>& animationList, std::unordered_map<std::string, std::vector<std::shared_ptr<Furniture>>>& newAnimation, mapSetString newAnimEvent, mapSetString newAnimVariable, std::unordered_map<std::string, var> AnimVar, std::unordered_map<std::string, std::unordered_map<int, bool>> ignoreFunction, std::unordered_map<std::string, std::vector<std::string>> modAnimBehavior, bool isCharacter);
extern void GenerateBehavior(std::string directory, vecstr behaviorPriority, std::unordered_map<std::string, bool> chosenBehavior);

#endif