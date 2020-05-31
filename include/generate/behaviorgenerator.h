#ifndef BEHAVIORGENERATOR_H_
#define BEHAVIORGENERATOR_H_

#include <string>
#include <filesystem>

bool hkxcmdProcess(std::filesystem::path xmlfile, std::filesystem::path hkxfile, bool last = false);
bool hkxcmdXmlInput(std::filesystem::path hkxfile, VecStr& fileline);
bool hkxcmdXmlInput(std::filesystem::path hkxfile, VecWstr& fileline);

std::string hkxTempCompile();

#endif