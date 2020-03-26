#ifndef DEBUGLOG_H_
#define DEBUGLOG_H_

#include <string>
#include <vector>
#include <fstream>

typedef std::vector<std::string> vecstr;

void DebugOutput();
void DebugLogging(std::string line, bool noEndLine = true);
void UpdateReset();
void PatchReset();

#endif
