#ifndef EXTERNALSCRIPT_H_
#define EXTERNALSCRIPT_H_

#include <filesystem>

extern bool dummyScript;

void RunScript(const std::filesystem::path& directory, bool& hasScript = dummyScript);

#endif
