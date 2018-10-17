#ifndef DEBUGLOG_H_
#define DEBUGLOG_H_

#include <string>
#include <vector>
#include <fstream>

typedef std::vector<std::string> vecstr;

void DebugOutput();
void DebugLogging(std::string line);
void UpdateReset();
void PatchReset();

#endif
