#ifndef DEBUGLOG_H_
#define DEBUGLOG_H_

#include <fstream>
#include <string>
#include <vector>

void DebugOutput();
void DebugLogging(std::string line, bool noEndLine = true);
void DebugLogging(std::wstring line, bool noEndLine = true);
void UpdateReset();
void PatchReset();

#endif
