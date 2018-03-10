#ifndef DATAUNIFICATION_H_
#define DATAUNIFICATION_H_

#include <iostream>
#include <Windows.h>
#include <thread>
#include <boost\regex.hpp>
#include <boost\filesystem.hpp>
#include "functions\functionupdate.h"
#include "functions\functionwriter.h"

extern void JoiningEdits(std::string directory);
extern void CombiningFiles(std::string directory);
extern bool newAnimUpdate(std::string sourcefolder, std::string targetfolder);

#endif