#ifndef DEBUGLOG_H_
#define DEBUGLOG_H_

#include <fstream>
#include <string>
#include <vector>

typedef std::vector<std::string> VecStr;
typedef std::vector<std::wstring> VecWstr;

void DebugOutput();
void DebugLogging(std::string line, bool noEndLine = true);
void DebugLogging(std::wstring line, bool noEndLine = true);
void UpdateReset();
void PatchReset();

#endif
