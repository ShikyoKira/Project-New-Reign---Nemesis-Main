#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <set>
#include <ctime>
#include <codecvt>
#include <stdlib.h>
#include <mutex>
#include <boost\filesystem.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>
#include "logging\debugmsg.h"

#include "skyrimdirectory.h"
#ifndef DEBUG
#include "skyrimdirectory.h"
#endif

#pragma warning(disable:4503)

#define NOT_FOUND std::string::npos

typedef std::vector<std::string> vecstr;
typedef std::vector<char> vecchar;

// utility
extern bool debug;								// if debug is on
extern int memory;								// not used; for setting memory allocation from 100 - 1000
extern int fixedkey[257];						// AA installation key
extern boost::posix_time::ptime time1;			// for getting elapsed time
#ifndef DEBUG
extern DataPath* skyrimDataPath;					// skyrim data path
#endif

// update patcher
extern std::unordered_map<std::string, std::string> behaviorPath;															// hkx file name, file path

// behavior generator
extern std::unordered_map<std::string, bool> activatedBehavior;																// behavior file, true/fast; check if the behavior is needed to be edited to character for animationdatasinglefile
extern std::unordered_map<std::string, std::string> behaviorProjectPath;													// project, project's path; project that has been installed
extern std::unordered_map<std::string, vecstr> behaviorJoints;																// lower lvl behavior file, higher lvl behavior file
extern std::unordered_map<std::string, vecstr> behaviorProject;																// character hkx file name, list of project hkx file name; link the project
extern std::unordered_map<std::string, std::set<std::string>> usedAnim;														// behavior name, animation path; animation used in behavior file
extern std::unordered_map<std::string, std::unordered_map<std::string, bool>> registeredAnim;								// characters hkx file name, animation name, bool; is registered in that behavior file?
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::set<std::string>>>> animModMatch;	// characters hkx file name, animation file, animation paths & mod name; match conflicting mod (duplicated anim file)

// Alternate Animation data
extern std::unordered_map<std::string, vecstr> alternateAnim;																// original animation name, list of AA animations
extern std::unordered_map<std::string, vecstr> groupAA;																		// animation group name, list of animations
extern std::unordered_map<std::string, vecstr> groupAAPrefix;																// animation group name, list of AA prefix's group; for scripting
extern std::unordered_map<std::string, vecstr> AAEvent;																		// AA animation, list of animEvent
extern std::unordered_map<std::string, vecstr> AAHasEvent;																	// original animation name, AA with event; which original animation associated with AA has new event name?
extern std::unordered_map<std::string, std::string> AAGroup;																// AA file name, animation group name
extern std::unordered_map<std::string, std::unordered_map<std::string, int>> AAGroupCount;									// AA prefix, animation group name, count; animation group picked count
extern std::set<std::string> groupNameList;																					// list of animation group name; for scripting

// string utilities
bool isOnlyNumber(std::string line);
bool hasAlpha(std::string line);
size_t wordFind(std::string line, std::string word, bool isLast = false);								// case insensitive "string.find"
inline int sameWordCount(std::string line, std::string word);

// general file utilities
size_t fileLineCount(std::string filepath);
void addUsedAnim(std::string behaviorFile, std::string animPath);
void read_directory(const std::string& name, vecstr& fv);
void produceBugReport(std::string directory, std::unordered_map<std::string, bool> chosenBehavior);
bool GetFunctionLines(std::string filename, vecstr& functionlines, bool emptylast = true);

inline bool isFileExist(const std::string& filename)
{
	return boost::filesystem::exists(boost::filesystem::path(filename));
}

inline bool CreateFolder(const std::string& folderpath)
{
	if (isFileExist(folderpath))
	{
		return true;
	}

	if (boost::filesystem::create_directory(boost::filesystem::path(folderpath)))
	{
		return true;
	}

	return false;
}


#endif