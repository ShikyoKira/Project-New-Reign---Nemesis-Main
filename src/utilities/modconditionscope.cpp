#include "utilities/conditionscope.h"

nemesis::ConditionScope::ConditionScope(const string& _modcode, const filesystem::path& _path)
{
    modcode = _modcode;
    path    = _path;
}

void nemesis::ConditionScope::ScopeValidation(const nemesis::Line& line)
{
    if (line.find(ns::If()) != NOT_FOUND)
    {
        scopeLayers.push_back(nemesis::IF);
    }
    else if (line.find(ns::EndIf()) != NOT_FOUND)
    {
        if (scopeLayers.size() == 0) ErrorMessage(1118, modcode, path, line.linenum);

        auto& back = scopeLayers.back();

        if (back != nemesis::IF || back != nemesis::ELSEIF || back != nemesis::ELSE)
        {
            ErrorMessage(1118, modcode, path, line.linenum);
        }

        scopeLayers.pop_back();
    }
    else if (line.find(ns::ForEach()) != NOT_FOUND)
    {
        scopeLayers.push_back(nemesis::FOREACH);
    }
    else if (line.find(ns::Close()) != NOT_FOUND)
    {
        if (scopeLayers.back() != nemesis::FOREACH) ErrorMessage(1118, modcode, path, line.linenum);

        scopeLayers.pop_back();
    }
}

void nemesis::ConditionScope::ScopeEnd(int lastline)
{
    if (scopeLayers.size() != 0) ErrorMessage(1114, modcode, path, lastline);
}
