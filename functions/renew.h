#ifndef RENEW_H_
#define RENEW_H_

#include <string>
#include <iostream>
#include <Windows.h>
#include <boost\filesystem.hpp>
#include <boost\regex.hpp>
#include "functionwriter.h"

extern bool Renew(boost::filesystem::path const & source, boost::filesystem::path const & destination);
extern bool copyDir(boost::filesystem::path const & source, boost::filesystem::path const & destination);
extern void Clearing(std::string directory);
extern void VanillaUpdate();

void GetPathLoop(std::string newPath);
void VanillaDeassemble(std::string path, std::string filename);

#endif