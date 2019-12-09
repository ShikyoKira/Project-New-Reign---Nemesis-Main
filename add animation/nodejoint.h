#ifndef NODEJOINT_H_
#define NODEJOINT_H_

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <set>

typedef std::unordered_map<std::string, int> id;
typedef std::vector<std::string> vecstr;
typedef std::set<std::string> setstr;
typedef std::unordered_map<std::string, std::string> SSMap;
typedef std::unordered_map<std::string, SSMap> ImportContainer;

struct master;
struct nodeJoint;
struct jointTemplate;
struct animationInfo;

struct lineCheck
{
	bool deleted = true;
	bool process = false;
	unsigned int row;
	std::string line;
	std::shared_ptr<nodeJoint> nested;

	lineCheck(std::string _line, unsigned int _row);
	lineCheck(std::shared_ptr<nodeJoint> _nested, unsigned int _row);

	lineCheck& operator=(std::string _line);
	lineCheck& operator=(std::shared_ptr<nodeJoint> _nested);
};

struct nodePackedParameters
{
	std::string format;
	std::string filename;
	std::string behaviorFile;
	int& nextFunctionID;
	std::string& strID;
	std::vector<std::vector<std::shared_ptr<animationInfo>>>& groupAnimInfo;
	SSMap& IDExist;
	std::shared_ptr<master> subFunctionIDs;
	ImportContainer& import;
	int linecount;
	id eventid;
	id variableid;
	std::string zeroEvent;
	std::string zeroVariable;
	int groupMulti;
	int animMulti;
	int optionMulti;
	bool hasGroup;
	std::string multiOption;
	std::vector<std::vector<std::unordered_map<std::string, bool>>>& optionPicked;
	unsigned __int64& openRange;
	unsigned int& elementCount;
	std::string& line;
	lineCheck* elementCatch;
	bool& negative;

	nodePackedParameters(std::string _format, std::string _filename, std::string _behaviorFile, int& _nextFunctionID, std::string& _strID,
		std::vector<std::vector<std::shared_ptr<animationInfo>>>& _groupAnimInfo, SSMap& _IDExist, std::shared_ptr<master> _subFunctionIDs, ImportContainer& _import, int _linecount,
		id _eventid, id _variableid, std::string _zeroEvent, std::string _zeroVariable, int _groupMulti, int _animMulti, int _optionMulti, bool _hasGroup,
		std::string _multiOption, std::vector<std::vector<std::unordered_map<std::string, bool>>>& _optionPicked, unsigned __int64& _openRange, unsigned int& _elementCount,
		std::string& _line, lineCheck* _elementCatch, bool& _negative);
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

	bool conditionSkip = false;
	unsigned int row;
	funcType type = NONE;
	std::string condt = "";
	std::vector<lineCheck> storeTemplate;
	std::vector<std::vector<lineCheck>> output;
	std::unordered_map<std::string, std::vector<unsigned int>> templateSection;			// template code, list of index of template section
	std::string behaviorFile;
	setstr templateGroup;

	nodeJoint(vecstr& node, std::string format, std::string filename, std::string _behaviorFile, std::unordered_map<std::string, std::string> otherAnimType, unsigned int startline = 0,
		unsigned int size = -1);
	nodeJoint(vecstr& node, std::string format, std::string filename, std::string _behaviorFile, setstr _templateGroup, unsigned int startline = 0, unsigned int size = -1);

	void insertData(std::string format, std::string filename, std::vector<std::vector<std::unordered_map<std::string, bool>>>& optionPicked,
		std::vector<std::vector<std::shared_ptr<animationInfo>>>& groupAnimInfo, int groupMulti, int animMulti, int optionMulti, bool hasMaster, bool hasGroup, bool ignoreGroup,
		std::string multiOption, int& nextFunctionID, std::string& strID, SSMap& IDExist, ImportContainer& import, id eventid, id variableid, std::string zeroEvent,
		std::string zeroVariable, unsigned __int64& openRange, unsigned int elementCount, lineCheck* elementCatch, std::shared_ptr<master> subFunctionIDs,
		bool& negative);

	void forEachProcess(std::vector<std::vector<lineCheck>>& output, lineCheck& storeTemplate, std::string condition, unsigned int sect, std::string format,
		std::string filename, std::vector<std::vector<std::unordered_map<std::string, bool>>>& optionPicked, std::vector<std::vector<std::shared_ptr<animationInfo>>>& groupAnimInfo,
		int groupMulti, int animMulti, int optionMulti, bool hasMaster, bool hasGroup, bool ignoreGroup, std::string multiOption, int& nextFunctionID,
		std::string& strID, SSMap& IDExist, ImportContainer& import, id eventid, id variableid, std::string zeroEvent, std::string zeroVariable, unsigned __int64 openRange,
		unsigned int elementCount, lineCheck* elementCatch, std::shared_ptr<master> subFunctionIDs, bool& negative);
	void optionMultiLoop(std::vector<lineCheck>& output, std::string format, std::string filename,
		std::vector<std::vector<std::unordered_map<std::string, bool>>>& optionPicked, std::vector<std::vector<std::shared_ptr<animationInfo>>>& groupAnimInfo, int groupMulti,
		int animMulti, int optionMulti, bool ignoreGroup, std::string multiOption, int& nextFunctionID, std::string& strID, SSMap& IDExist, ImportContainer& import, id eventid,
		id variableid, std::string zeroEvent, std::string zeroVariable, unsigned __int64 openRange, unsigned int elementCount, lineCheck* elementCatch,
		std::shared_ptr<master> subFunctionIDs, bool& negative, lineCheck temp);

	vecstr unpack();			// output all stored outputs in different layers

private:
	void dataBake(vecstr& node, std::string format, std::string filename, std::unordered_map<std::string, std::string> otherAnimType, unsigned int startline = 0, unsigned int size = -1);
};

#endif
