#ifndef NODEJOINT_H_
#define NODEJOINT_H_

#include <string>
#include <vector>
#include <memory>
#include <map>

typedef std::vector<std::string> vecstr;

struct nodeJoint;

struct lineCheck
{
	std::string line;
	std::shared_ptr<nodeJoint> nested;

	lineCheck(std::string _line);
	lineCheck(std::shared_ptr<nodeJoint> _nested);
};

struct nodeJoint
{
	enum funcType
	{
		NONE,
		NEW,
		FOREACH,
		CONDITION_START,
		CONDITION_ELSE,
		CONDITION
	};

	funcType type = NONE;
	std::string condt = "";
	std::vector<lineCheck> storeTemplate;
	std::vector<vecstr> output;

	nodeJoint(vecstr& node, std::string format, std::string filename, unsigned int startline, unsigned int size);
	std::vector<std::string> unpack();
};

#endif
