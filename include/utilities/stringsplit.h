#ifndef STRINGSPLIT_H_
#define STRINGSPLIT_H_

typedef std::vector<std::string> vecstr;

void StringSplit(const std::string& line, vecstr& container);
void StringSplit(const std::string& line, vecstr& container, const std::string& anyChar);

#endif
