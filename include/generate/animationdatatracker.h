#ifndef ANIMATIONDATATRACKER_H_
#define ANIMATIONDATATRACKER_H_

#include <set>

struct AnimationDataTracker
{
private:
    int order = -1;

public:
    std::string filename;
    std::set<std::string> cliplist;

    AnimationDataTracker()
    {}
    AnimationDataTracker(int _order, std::string _filename);
    int GetOrder();
    void SetOrder(int _order);
};

extern std::map<std::string, std::map<std::string, std::vector<std::shared_ptr<AnimationDataTracker>>>>
    clipPtrAnimData; // character path, clip name, list of animdata
extern std::map<std::string, std::map<std::string, std::shared_ptr<AnimationDataTracker>>>
    charAnimDataInfo; // character path, file name, animdata

#endif
