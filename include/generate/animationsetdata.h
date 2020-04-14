#ifndef ANIMATIONSETDATA_H_
#define ANIMATIONSETDATA_H_

#include "utilities/alphanum.hpp"
#include "utilities/writetextfile.h"

#include "generate/generator_utility.h"
#include "generate/animationsetdatatype.h"

#include "generate/animation/newanimation.h"
#include "generate/animation/templateinfo.h"

struct datapack
{
    std::vector<equip> equiplist;   // anim data set, equip list
    std::vector<typepack> typelist; // anim data set, type list
    std::vector<animpack> animlist; // anim data set, list of animpack
    std::vector<crc32> crc32list;   // anim data set, crc32 list
};

struct AnimationDataProject
{
    bool isNew = false;                                      // is this new set of animdata data
    std::string mod;                                         // modcode
    std::map<std::string, datapack, alphanum_less> datalist; // anim data set

    AnimationDataProject()
    {}
    AnimationDataProject(int& startline,
                         VecStr& animdatafile,
                         std::string filename,
                         std::string projectname,
                         const NemesisInfo* nemesisInfo);
};

struct ASDFunct
{
    bool isNew          = false;
    bool isCondition    = false;
    bool isConditionOri = false;
    bool nextCondition  = false;
    bool skip           = false;

    int conditionOpen = false;
};

namespace ASDFormat
{
    enum position
    {
        V3,
        equipcount,
        equiplist,
        typecount,
        typelist,
        unknown1,
        unknown2,
        animpackcount,
        animpackname,
        unknown3,
        attackcount,
        attacklist,
        crc32count,
        crc32list,
        xerror,
        null
    };
}

struct MasterAnimSetData
{
    VecStr projectList; // order of the project
    std::unordered_map<std::string, std::map<std::string, VecStr, alphanum_less>>
        newAnimSetData; // project, header, vector<string>; memory to access each node
};

extern void
combineExtraction(VecStr& storeline, std::map<int, VecStr> extract, std::string project, std::string header);

ASDFormat::position ASDPosition(VecStr animData,
                                std::string character,
                                std::string header,
                                std::string modcode,
                                int linecount,
                                bool muteError);
ASDFormat::position ASDConvert(int position, bool muteError);

int PositionLineCondition(int& i,
                          double curID,
                          int linecount,
                          VecStr animDataSet,
                          std::unordered_map<int, ASDFunct>& marker,
                          std::string modcode,
                          std::string header,
                          bool last,
                          bool muteError);

#endif