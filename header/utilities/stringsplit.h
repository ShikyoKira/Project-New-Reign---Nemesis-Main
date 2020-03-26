#ifndef STRINGSPLIT_H_
#define STRINGSPLIT_H_

typedef std::vector<std::string> vecstr;

void StringSplit(std::string line, vecstr& container);
void StringSplit(std::string line, vecstr& container, std::string anyChar);

#endif
