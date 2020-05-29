#ifndef BEHAVIORGENERATOR_H_
#define BEHAVIORGENERATOR_H_

bool hkxcmdProcess(std::string xmlfile, std::string hkxfile, bool last = false);
bool hkxcmdProcess(std::wstring xmlfile, std::wstring hkxfile, bool last = false);
bool hkxcmdXmlInput(std::string hkxfile, VecStr& fileline);
bool hkxcmdXmlInput(std::wstring hkxfile, VecStr& fileline);

#endif