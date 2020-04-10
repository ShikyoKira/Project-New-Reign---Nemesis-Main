#include "Global.h"

#include "generate/addvariables.h"

using namespace std;

bool AddVariables(string curNum,
                  vecstr& storeline,
                  string variablename,
                  unordered_map<string, bool> orivariable,
                  unordered_map<string, bool>& isExist,
                  int& counter,
                  string ZeroVariable,
                  unordered_map<string, int>& variableid,
                  unordered_map<int, string>& varName,
                  bool isInt)
{
    if (curNum == "variableNames")
    {
        if (isExist[variablename]) { WarningMessage(1011, variablename); }
        else if (!orivariable[variablename])
        {
            if (counter == 0) { ZeroVariable = variablename; }

            string curline           = "				<hkcstring>" + variablename + "</hkcstring>";
            varName[counter]         = variablename;
            variableid[variablename] = counter;
            storeline.push_back(curline);
            isExist[variablename] = true;
            ++counter;
        }
    }
    else if (curNum == "wordVariableValues")
    {
        if (!isExist[variablename] && !orivariable[variablename])
        {
            storeline.push_back("				<hkobject>");
            storeline.push_back("					<hkparam name=\"value\">0</hkparam>");
            storeline.push_back("				</hkobject>");
            isExist[variablename] = true;
            ++counter;
        }
    }
    else if (curNum == "variableInfos")
    {
        if (!isExist[variablename] && !orivariable[variablename])
        {
            storeline.push_back("				<hkobject>");
            storeline.push_back("					<hkparam name=\"role\">");
            storeline.push_back("						<hkobject>");
            storeline.push_back("							<hkparam name=\"role\">ROLE_DEFAULT</hkparam>");
            storeline.push_back("							<hkparam name=\"flags\">0</hkparam>");
            storeline.push_back("						</hkobject>");
            storeline.push_back("					</hkparam>");

            if (isInt)
            {
                storeline.push_back(
                    "					<hkparam name=\"type\">VARIABLE_TYPE_INT32</hkparam>");
            }
            else
            {
                storeline.push_back("					<hkparam name=\"type\">VARIABLE_TYPE_BOOL</hkparam>");
            }

            storeline.push_back("				</hkobject>");
            isExist[variablename] = true;
            ++counter;
        }
    }
    else
    {
        ErrorMessage(1032);
    }

    return true;
}
