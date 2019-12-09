#ifndef ANIMATIONUTILITY_H_
#define ANIMATIONUTILITY_H_

typedef std::unordered_map<std::string, int> id;

struct jointTemplate;
struct groupTemplate;
struct ExistingFunction;
struct nodePackedParameters;

struct animationutility
{
	id eventid;
	id variableid;	
	std::vector<int> fixedStateID;
	std::vector<int> stateCountMultiplier;
	bool hasGroup = false;
	int optionMulti = -1;
	int animMulti = -1;
	int groupMulti = -1;
	std::string multiOption;
	std::string originalCondition;
	jointTemplate* currentProcess;
	std::shared_ptr<nodePackedParameters> nodeProcess;
	bool isExisting = false;

	animationutility() {}
	animationutility(std::string condition, id eventid, id variableid, std::vector<int> stateID, std::vector<int> stateCountMultiplier, bool hasGroup, int optionMulti = -1, int animMulti = -1, std::string multiOption = "");
};

class newStateID
{
	std::vector<std::shared_ptr<int>> lastState;

public:
	newStateID();
	void push_back(std::shared_ptr<int> num);
	void reset();
	std::shared_ptr<int>& operator[](unsigned int number);
	unsigned int size();
	bool stateUpdate(int ID, std::string format, std::string bevaiorFile, int linecount, std::string state, bool hasGroup = false);

};

#endif