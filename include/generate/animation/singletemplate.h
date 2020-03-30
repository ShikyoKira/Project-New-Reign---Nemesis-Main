#ifndef SINGLETEMPLATE_H_
#define SINGLETEMPLATE_H_

#include <boost/thread.hpp>

#include "generate/animation/optionlist.h"
#include "generate/animation/templateprocessing.h"

typedef void (proc::*funcptr)(range, vecstr&);

struct condset;

struct stackline
{
    bool hasProcess;
    size_t linecount;
    std::string line;
    vecstr lineblocks;
    std::vector<condset> nestedcond;

    stackline()
    {}
    stackline(std::string curline, bool process, size_t num);
};

struct addOnInfo
{
    int optionMulti = -1;
    int animMulti   = -1;
    std::string header;
    std::string addition;

    addOnInfo(){};
    addOnInfo(std::string n_h, std::string n_a);
    addOnInfo(std::string n_h, std::string n_a, int n_om);
};

struct condt
{
private:
    int hiddenOrder;
    bool last = false;

public:
    bool oneTime = false;
    bool isNot   = false;
    bool OR      = false;
    bool isOrder = false;

    std::string original;

    size_t cmp1_high;
    size_t cmp2_high;

    vecstr cmp1;
    vecstr cmp2;

    bool cmpbool1 = false;
    bool cmpbool2 = false;

    std::vector<addOnInfo> cmpinfo1;
    std::vector<addOnInfo> cmpinfo2;

    std::map<int, std::vector<std::shared_ptr<range>>> cmp1_block;
    std::map<int, std::vector<std::shared_ptr<range>>> cmp2_block;

    std::shared_ptr<vecstr> optioncondt;
    std::shared_ptr<condt> nestedcond;
    std::shared_ptr<condt> next;

    vecstr olddata;
    std::vector<int> olddataint;

    condt(std::string condition,
          std::string format,
          std::string behaviorFile,
          std::string originalCondition,
          std::string multiOption,
          int numline,
          bool isGroup,
          bool isMaster,
          OptionList& optionlist);

    bool isTrue(proc* process,
                std::string format,
                std::string behaviorFile,
                int numline,
                bool isGroup,
                bool isMaster,
                std::shared_ptr<condt> curptr);
    bool isMultiTrue(proc* process,
                     std::string format,
                     std::string behaviorFile,
                     int numline,
                     int& animMulti,
                     bool isGroup,
                     bool isMaster,
                     int& groupMulti);

private:
    bool specialIsTrueA(proc* process,
                        std::string format,
                        std::string behaviorFile,
                        int numline,
                        bool isGroup,
                        bool isMaster,
                        const std::vector<std::shared_ptr<AnimationInfo>>& groupAnimInfo);
    bool specialIsTrueB(proc* process,
                        std::string format,
                        std::string behaviorFile,
                        int numline,
                        bool isGroup,
                        bool isMaster,
                        const std::vector<std::shared_ptr<AnimationInfo>>& groupAnimInfo);
    void specialCondition(std::string condition,
                          std::string format,
                          std::string behaviorFile,
                          std::string multiOption,
                          int numline,
                          bool isGroup,
                          bool isMaster,
                          OptionList& optionlist);
    void singleCondition(std::string condition,
                         std::string format,
                         std::string behaviorFile,
                         std::string multiOption,
                         int numline,
                         bool isGroup,
                         bool isMaster,
                         OptionList& optionlist);
    void conditionProcess(std::string condition,
                          std::string format,
                          std::string behaviorFile,
                          std::string multiOption,
                          int numline,
                          bool isGroup,
                          bool isMaster);
};

struct condset
{
    bool isMulti  = false;
    bool isOrder  = false;
    size_t curnum = 0;
    size_t linenum;
    std::string conditions;
    std::shared_ptr<condt> n_conditions;
    std::vector<stackline> lines;

    condset()
    {}
};

struct multichoice
{
    bool chosen = false;
    std::shared_ptr<condt> condition;
    size_t locateA;
    size_t locateB;

    multichoice()
    {}
    multichoice(std::string cond,
                std::string format,
                std::string behaviorFile,
                std::string multiOption,
                int numline,
                bool isGroup,
                bool isMaster,
                OptionList& optionlist,
                size_t posA,
                size_t posB);
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
    size_t size  = 0;
    condset lines;
    proc process;

    void ExamineTemplate(std::string n_format,
                         std::string n_file,
                         vecstr templatelines,
                         bool isGroup,
                         bool isMaster,
                         OptionList optionlist);
};

std::string getOption(std::string curline);

#endif
