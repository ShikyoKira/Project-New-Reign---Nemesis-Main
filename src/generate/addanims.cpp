#include "Global.h"

#include "generate/addanims.h"
#include "generate/generator_utility.h"
#include "generate/animationdatatracker.h"
#include "generate/generator_utility.h"

#include "utilities/atomiclock.h"

using namespace std;

namespace sf = std::filesystem;

extern atomic_flag animdata_lock;

bool AddAnims(string& line,
              const string& _animPath,
              const wstring& outputdir,
              const string& behaviorFile,
              const string& lowerBehaviorFile,
              const string& _newMod,
              vector<string>& storeline,
              int& counter,
              unordered_map<string, bool>& isAdded,
              bool& addAnim)
{

    return true;
}
