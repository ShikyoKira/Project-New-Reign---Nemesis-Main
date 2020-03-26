#include "generate/animationdatatracker.h"

using namespace std;

map<string, map<string, vector<shared_ptr<AnimationDataTracker>>>> clipPtrAnimData;
map<string, map<string, shared_ptr<AnimationDataTracker>>> charAnimDataInfo;

AnimationDataTracker::AnimationDataTracker(int _order, string _filename)
{
	order = _order;
	filename = _filename;
}

int AnimationDataTracker::GetOrder()
{
	return order;
}

void AnimationDataTracker::SetOrder(int _order)
{
	order = _order;
}
