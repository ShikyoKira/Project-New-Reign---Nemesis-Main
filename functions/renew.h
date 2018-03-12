#ifndef RENEW_H_
#define RENEW_H_

#include <string>
#include <iostream>
#include <Windows.h>
#include <boost\filesystem.hpp>
#include <boost\regex.hpp>
#include "functionwriter.h"

extern bool VanillaUpdate(std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);

bool GetPathLoop(std::string newPath, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);
bool VanillaDeassemble(std::string path, std::string filename, std::unordered_map<std::string, std::map<std::string, vecstr>>& newFile);

#endif