#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <ctime>
#include <stdlib.h>
#include <compute.h>
#include <boost\filesystem.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>

#pragma warning(disable:4503)

typedef std::vector<std::string> vecstr;
typedef std::vector<char> vecchar;

// utility
extern bool debug;												// if debug is on
extern bool error;												// get error warning
extern int memory;												// not used; for setting memory allocation from 100 - 1000
extern int fixedkey[257];										// AA installation key
extern boost::posix_time::ptime time1;							// for getting elapsed time

// update patcher
extern std::unordered_map<std::string, std::string> behaviorPath;															// hkx file name, file path

// behavior generator
extern std::unordered_map<std::string, std::unordered_map<std::string, bool>> registeredAnim;								// characters hkx file name, animation name, bool; is registered in that behavior file?
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::set<std::string>>>> animModMatch;	// characters hkx file name, animation file, animation paths & mod name; match conflicting mod (duplicated anim file)
extern std::unordered_map<std::string, std::set<std::string>> characterHeaders;												// character, list of headers; use to check if header exist
extern std::unordered_map<std::string, std::set<std::string>> usedAnim;														// behavior name, animation path; animation used in behavior file
extern std::unordered_map<std::string, vecstr> behaviorJoints;																// lower lvl behavior file, higher lvl behavior file
extern std::unordered_map<std::string, bool> activatedBehavior;																// behavior file, true/fast; check if the behavior is needed to be edited

// Alternate Animation data
extern std::unordered_map<std::string, vecstr> alternateAnim;																// original animation name, list of AA animations
extern std::unordered_map<std::string, vecstr> groupAA;																		// animation group name, list of animations
extern std::unordered_map<std::string, vecstr> groupAAPrefix;																// animation group name, list of AA prefix's group; for scripting
extern std::unordered_map<std::string, vecstr> AAEvent;																		// AA animation, list of animEvent
extern std::unordered_map<std::string, vecstr> AAHasEvent;																	// original animation name, AA with event; which original animation associated with AA has new event name?
extern std::unordered_map<std::string, std::string> AAGroup;																// AA file name, animation group name
extern std::unordered_map<std::string, std::unordered_map<std::string, int>> AAGroupCount;									// AA prefix, animation group name, count; animation group picked count
extern vecstr groupNameList;																								// list of animation group name; for scripting

size_t fileLineCount(std::string filepath);

extern bool isOnlyNumber(std::string line);
extern bool hasAlpha(std::string line);
extern size_t wordFind(std::string line, std::string word, bool isLast = false);								// case insensitive "string.find"
extern void read_directory(const std::string& name, vecstr& fv);
extern void produceBugReport(std::string directory, std::unordered_map<std::string, bool> chosenBehavior);
inline extern int sameWordCount(std::string line, std::string word);
extern vecstr GetFunctionLines(std::string filename);
inline extern bool isFileExist(const std::string& filename);

#endif