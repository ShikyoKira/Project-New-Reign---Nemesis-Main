#include "nodeJoint.h"
#include "Global.h"

using namespace std;

lineCheck::lineCheck(std::string _line)
{
	line = _line;
}

lineCheck::lineCheck(std::shared_ptr<nodeJoint> _nested)
{
	nested = _nested;
}

nodeJoint::nodeJoint(vecstr& node, string format, string filename, unsigned int startline = 0, unsigned int size = -1)
{
	string multiOption;
	vecstr tempstore;
	funcType curType = NONE;
	int scope = 0;
	int start;

	if (size == -1) size = node.size();

	for (int i = startline; i < size; ++i)
	{
		string line = node[i];

		if (line.find("<!-- CONDITION START ^", 0) != NOT_FOUND)
		{
			if (scope == 0)
			{
				curType = CONDITION_START;
				start = i + 1;
				unsigned int pos = line.find("<!-- CONDITION START ^") + 22;
				multiOption = line.substr(pos, line.find("^ -->", pos) - pos);
			}

			++scope;
		}
		else if (line.find("<!-- CONDITION ^", 0) != NOT_FOUND)
		{
			if (scope == 0)
			{
				curType = CONDITION_ELSE;
				start = i + 1;
				unsigned int pos = line.find("<!-- CONDITION ^") + 16;
				multiOption = line.substr(pos, line.find("^", pos) - pos);
			}

			++scope;
		}
		else if (line.find("<!-- CONDITION -->", 0) != NOT_FOUND)
		{
			if (scope == 0)
			{
				curType = CONDITION;
				start = i + 1;
			}

			++scope;
		}
		else if (line.find("<!-- FOREACH ^", 0) != NOT_FOUND)
		{
			if (scope == 0)
			{
				curType = FOREACH;
				start = i + 1;
				unsigned int pos = line.find("<!-- FOREACH ^") + 14;
				multiOption = boost::to_lower_copy(line.substr(pos, line.find("^", pos) - pos));
			}

			++scope;
		}
		else if (line.find("<!-- NEW ^", 0) != NOT_FOUND && line.find("^ -->", line.find("<!-- NEW ^", 0)) != NOT_FOUND)
		{
			if (scope == 0)
			{
				curType = NEW;
				start = i + 1;
				unsigned int pos = line.find("<!-- NEW ^") + 10;
				multiOption = boost::to_lower_copy(line.substr(pos, line.find("^", pos) - pos));
			}

			++scope;
		}
		else if (line.find("<!-- CLOSE -->", 0) != NOT_FOUND)
		{
			if (scope == 0) ErrorMessage(1171, format, filename, i + 1);

			curType = NONE;
			--scope;
			storeTemplate.push_back(make_shared<nodeJoint>(node, format, filename, start, i));
			storeTemplate.back().nested->condt = multiOption;
			storeTemplate.back().nested->type = curType;
		}
		else if (scope == 0)
		{
			storeTemplate.push_back(line);
		}
	}
}

vecstr nodeJoint::unpack()
{
	vecstr storeline;

	for (auto& each : output)
	{
		storeline.insert(storeline.end(), each.begin(), each.end());
	}

	return storeline;
}
