#ifndef BEHAVIORGENERATOR_H_
#define BEHAVIORGENERATOR_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <Windows.h>
#include <algorithm>
#include <sys/stat.h>
#include <boost\regex.hpp>
#include <boost\filesystem.hpp>
#include <boost\algorithm\string.hpp>
#include "functions\functionwriter.h"
#include "add animation\furniture.h"
#include "add animation\grouptemplate.h"
#include "add animation\optionlist.h"
#include "add animation\registeranimation.h"
#include "add animation\gettemplate.h"
#include "add animation\import.h"
#include "generator_utility.h"

typedef std::unordered_map<std::string, SSMap> ImportContainer;

struct IDCatcher
{
private:
	int ID;
	int line;

public:
	IDCatcher(int id, int curline);
	int getID();
	int getLine();
};

#endif