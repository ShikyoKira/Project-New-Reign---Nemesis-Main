#ifndef DEBUGLOG_H_
#define DEBUGLOG_H_

#include <fstream>
#include <string>
#include <vector>

typedef std::vector<std::string> vecstr;

void DebugOutput();
void DebugLogging(std::string line, bool noEndLine = true);
void UpdateReset();
void PatchReset();

#endif
