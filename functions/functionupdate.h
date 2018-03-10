#ifndef FUNCTIONUPDATE_H_
#define FUNCTIONUPDATE_H_

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <mutex>
#include <boost\regex.hpp>
#include "functionwriter.h"

extern bool FunctionUpdate(std::string modcode, std::string f2, std::string f3, int memoryStore);

#endif