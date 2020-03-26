#ifndef ADDEVENTS_H_
#define ADDEVENTS_H_

bool AddEvents(std::string curNum, vecstr& storeline, std::string input, std::unordered_map<std::string, bool> orievent, std::unordered_map<std::string, bool>& isExist, int& counter
	, std::string& ZeroEvent, std::unordered_map<std::string, int>& eventid, std::unordered_map<int, std::string>& eventName);

#endif
