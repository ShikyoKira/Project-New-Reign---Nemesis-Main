#ifndef SINGLETEMPLATE_H_
#define SINGLETEMPLATE_H_

#include <boost/thread.hpp>

#include "utilities/linkedvar.h"

#include "generate/animation/optionlist.h"
#include "generate/animation/templateprocessing.h"

struct AddOnInfo
{
    int optionMulti = -1;
    int animMulti   = -1;
    std::string header;
    std::string addition;

	AddOnInfo() {};
	AddOnInfo(std::string n_h, std::string n_a);
	AddOnInfo(std::string n_h, std::string n_a, int n_om);
};

class AnimTemplate
{
    std::string format;
    std::string behaviorFile;

    void Process(std::string& line,
                 std::string multiOption,
                 bool& norElement,
                 bool& isEnd,
                 bool isGroup,
                 bool isMaster,
                 int& openRange,
                 int numline,
                 OptionList& optionlist,
                 condset* generatedlines);

public:
	bool hasDuration;
	bool isGroup = false;
	size_t size = 0;
	nemesis::CondVar<std::string> lines;
	proc process;

    void ExamineTemplate(std::string n_format,
                         std::string n_file,
                         VecStr templatelines,
                         bool isGroup,
                         bool isMaster,
                         OptionList optionlist);
};

std::string getOption(std::string curline);

#endif
