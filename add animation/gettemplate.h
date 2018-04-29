#ifndef GETTEMPLATE_H_
#define GETTEMPLATE_H_

#include "optionlist.h"

struct getTemplate
{
	std::unordered_map<std::string, OptionList> optionlist;													// animation code, option list; access point to option list from animation code
	std::unordered_map<std::string, vecstr> grouplist;														// behavior file, list of animation code(in vector of string); assign animation to their respective behavior
	std::unordered_map<std::string, bool> templatelist;														// animation code, true/false; register template
	std::unordered_map<std::string, std::unordered_map<std::string, vecstr>> behaviortemplate;				// animation code, behavior, template file(in vector of string); store template in memory
	std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>> existingFunctionID;	// animation code, behavior, list of node ID(in vector of int); node(function) from the master branch that is/are used in the generation of animation
	std::unordered_map<std::string, std::unordered_map<std::string, std::map<int, int>>> mainBehaviorJoint;	// animation code, behavior, state number, node(function) ID; record the node that is joint with the master branch

	getTemplate();
};


#endif