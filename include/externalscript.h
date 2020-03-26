#ifndef EXTERNALSCRIPT_H_
#define EXTERNALSCRIPT_H_

#include <string>

extern bool dummyScript;

void RunScript(std::string directory, bool& hasScript = dummyScript);

#endif
