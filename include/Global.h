#ifndef GLOBAL_H_
#define GLOBAL_H_

#define WIN32_LEAN_AND_MEAN

#include <ctime>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>

using VecStr  = std::vector<std::string>;
using VecChar = std::vector<char>;
using ID      = std::unordered_map<std::string, int>;
using uint    = unsigned int;

#include "debugmsg.h"
#include "nemesisinfo.h"
#include "utilities/wstrconvert.h"

#pragma warning(disable : 4503)

#define NOT_FOUND std::string::npos

// utility
extern bool debug;                     // if debug is on
extern int memory;                     // not used; for setting memory allocation from 100 - 1000
extern int fixedkey[257];              // AA installation key
extern boost::posix_time::ptime time1; // for getting elapsed time
extern NemesisInfo* nemesisInfo;       // nemesis ini info

// update patcher
extern std::unordered_map<std::string, std::string> behaviorPath; // hkx file name, file path

// behavior generator
extern std::unordered_map<std::string, bool>
    activatedBehavior; // behavior file, true/fast; check if the behavior is needed to be edited to character for animationdatasinglefile
extern std::unordered_map<std::string, std::string>
    behaviorProjectPath; // project, project's path; project that has been installed
extern std::unordered_map<std::string, VecStr>
    behaviorJoints; // lower lvl behavior file, higher lvl behavior file
extern std::unordered_map<std::string, VecStr>
    behaviorProject; // character hkx file name, list of project hkx file name; link the project
extern std::unordered_map<std::string, std::set<std::string>>
    usedAnim; // behavior name, animation path; animation used in behavior file
extern std::unordered_map<std::string, std::unordered_map<std::string, bool>>
    registeredAnim; // characters hkx file name, animation name, bool; is registered in that behavior file?
extern std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::set<std::string>>>>
    animModMatch; // characters hkx file name, animation file, animation paths & mod name; match conflicting mod (duplicated anim file)

// Alternate Animation data
extern std::unordered_map<std::string, VecStr>
    alternateAnim;                                      // original animation name, list of AA animations
extern std::unordered_map<std::string, VecStr> groupAA; // animation group name, list of animations
extern std::unordered_map<std::string, VecStr>
    groupAAPrefix; // animation group name, list of AA prefix's group; for scripting
extern std::unordered_map<std::string, VecStr> AAEvent; // AA animation, list of animEvent
extern std::unordered_map<std::string, VecStr>
    AAHasEvent; // original animation name, AA with event; which original animation associated with AA has new event name?
extern std::unordered_map<std::string, std::string> AAGroup; // AA file name, animation group name
extern std::unordered_map<std::string, std::unordered_map<std::string, int>>
    AAGroupCount; // AA prefix, animation group name, count; animation group picked count
extern std::set<std::string> groupNameList; // list of animation group name; for scripting

// string utilities
bool isOnlyNumber(std::string line);
bool hasAlpha(std::string line);
size_t wordFind(std::string line, std::string word, bool isLast = false); // case insensitive "string.find"
int sameWordCount(std::string line, std::string word);

// general file utilities
size_t fileLineCount(const char* filepath);
size_t fileLineCount(std::filesystem::path filepath);

void addUsedAnim(std::string behaviorFile, std::string animPath);

void read_directory(const std::string& name, VecStr& fv);
void read_directory(const std::wstring& name, std::vector<std::wstring>& fv);
void read_directory(const char* name, VecStr& fv);
void read_directory(const wchar_t* name, std::vector<std::wstring>& fv);

void produceBugReport(std::string directory, std::unordered_map<std::string, bool> chosenBehavior);

bool GetFunctionLines(std::filesystem::path filename, VecStr& functionlines, bool emptylast = true);
bool GetFunctionLines(std::filesystem::path filename,
                      std::vector<std::wstring>& functionlines,
                      bool emptylast = true);

inline bool isFileExist(const std::string& filename)
{
    try
    {
        return std::filesystem::exists(filename);
    }
    catch (...)
    {
        return false;
    }
}

inline bool isFileExist(std::string_view filename)
{
    try
    {
        return std::filesystem::exists(filename);
    }
    catch (...)
    {
        return false;
    }
}

inline bool isFileExist(const char* filename)
{
    try
    {
        return std::filesystem::exists(filename);
    }
    catch (...)
    {
        return false;
    }
}

inline bool isFileExist(const std::wstring& filename)
{
    try
    {
        return std::filesystem::exists(filename);
    }
    catch (...)
    {
        return false;
    }
}

inline bool isFileExist(std::wstring_view filename)
{
    try
    {
        return std::filesystem::exists(filename);
    }
    catch (...)
    {
        return false;
    }
}

inline bool isFileExist(const wchar_t* filename)
{
    try
    {
        return std::filesystem::exists(filename);
    }
    catch (...)
    {
        return false;
    }
}

inline bool CreateFolder(const std::string& folderpath)
{
    if (isFileExist(folderpath)) return true;

    return std::filesystem::create_directories(folderpath);
}

#endif
