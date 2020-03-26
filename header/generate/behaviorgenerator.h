#ifndef BEHAVIORGENERATOR_H_
#define BEHAVIORGENERATOR_H_

bool hkxcmdProcess(std::string xmlfile, std::string hkxfile, bool last = false);
bool hkxcmdXmlInput(std::string hkxfile, vecstr& fileline);

#endif