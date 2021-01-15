#include "utilities/scopeinfo.h"

using namespace std;

nemesis::ScopeInfo::ScopeInfo(bool _isGroup, bool _isMaster)
{
    isGroup = _isGroup;
    isMaster = _isMaster;
}

uint nemesis::ScopeInfo::GetOptionIndex(const string option) const
{
    return 0;
}
