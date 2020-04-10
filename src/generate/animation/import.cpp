#include "Global.h"
#include <utility>

#include "generate/animation/animationinfo.h"
#include "generate/animation/import.h"
#include "generate/animation/newanimation.h"

#pragma warning(disable : 4503)

using namespace std;

vecstr importOutput(vector<ImportContainer>& ExportID, int counter, int nextID, string file)
{
    vecstr behaviorlines;
    ImportContainer newExportID;
    int lastID = nextID;

    for (auto it = ExportID[counter].begin(); it != ExportID[counter].end(); ++it)
    {
        string filename = "behavior templates\\" + it->first + ".txt";

        if (!isFileExist(filename)) { ErrorMessage(1027, filename); }

        vecstr exportFormat;

        if (!GetFunctionLines(filename, exportFormat))
        {
            behaviorlines.shrink_to_fit();
            return behaviorlines;
        }

        behaviorlines.reserve(exportFormat.size());

        for (auto iter = it->second.begin(); iter != it->second.end(); ++iter)
        {
            if (iter->second.length() > 0)
            {
                bool norElement = false;
                int openRange   = 0;
                int elementLine = 0;
                int eleCounter  = 0;
                SSMap IDExist;
                behaviorlines.reserve(behaviorlines.size() + exportFormat.size() + 1);

                for (unsigned int j = 0; j < exportFormat.size(); ++j)
                {
                    bool elementCatch = false;
                    string line       = exportFormat[j];

                    if (line.find("<hkparam name=\"") != NOT_FOUND && line.find("numelements=\"") != NOT_FOUND
                        && line.find("</hkparam>") == NOT_FOUND
                        && line.find("<!-- COMPUTE -->", line.find("numelements=\"")) != NOT_FOUND)
                    {
                        if (!norElement)
                        {
                            norElement      = true;
                            elementCatch    = true;
                            string templine = line.substr(0, line.find("<hkparam name=\"", 0));
                            openRange       = count(templine.begin(), templine.end(), '\t');
                        }
                        else
                        {
                            ErrorMessage(1135, filename, j + 1);
                        }
                    }
                    else if (line.find("</hkparam>") != NOT_FOUND && norElement)
                    {
                        string templine = line.substr(0, line.find("</hkparam>"));
                        __int64 range   = count(templine.begin(), templine.end(), '\t');

                        if (openRange == range)
                        {
                            string oldElement;

                            if (exportFormat[elementLine].find("numelements=\"$elements$\">", 0) == NOT_FOUND)
                            {
                                size_t position = exportFormat[elementLine].find("numelements=\"") + 13;
                                oldElement      = exportFormat[elementLine].substr(
                                    position, exportFormat[elementLine].find("\">", position) - position);
                            }
                            else
                            {
                                oldElement = "$elements$";
                            }

                            if (oldElement != to_string(eleCounter))
                            {
                                exportFormat[elementLine].replace(exportFormat[elementLine].find(oldElement),
                                                                  oldElement.length(),
                                                                  to_string(eleCounter));
                            }

                            norElement  = false;
                            eleCounter  = 0;
                            elementLine = -1;
                        }
                    }

                    if (norElement)
                    {
                        string templine = line;

                        if (templine.find("<hkobject>") != NOT_FOUND)
                        {
                            templine      = templine.substr(0, templine.find("<hkobject>"));
                            __int64 range = count(templine.begin(), templine.end(), '\t');

                            if (range == openRange + 1) { eleCounter++; }
                        }
                        else if (templine.find("\t\t\t#") != NOT_FOUND)
                        {
                            templine          = templine.substr(0, templine.find('#', 0));
                            __int64 reference = count(templine.begin(), templine.end(), '\t');

                            if (reference == openRange + 1)
                            {
                                __int64 number = count(line.begin(), line.end(), '#');
                                eleCounter += int(number);
                            }
                        }
                    }

                    if (line.find("$import[1][2]$", 0) != NOT_FOUND)
                    {
                        int reference = sameWordCount(line, "$import[1][2]$");

                        for (int k = 0; k < reference; ++k)
                        {
                            line.replace(line.find("$import[1][2]$"), 14, iter->second);
                        }
                    }

                    if (line.find("$import[1]$", 0) != NOT_FOUND)
                    {
                        int reference = sameWordCount(line, "$import[1]$");

                        for (int k = 0; k < reference; ++k)
                        {
                            line.replace(line.find("$import[1]$"), 11, it->first);
                        }
                    }

                    if (line.find("$import[", 0) != NOT_FOUND
                        && line.find("]$", line.find("$import[" + 1)) != NOT_FOUND)
                    {
                        int reference = sameWordCount(line, "$import[");

                        for (int k = 0; k < reference; ++k)
                        {
                            string number = boost::regex_replace(string(line.substr(line.find("$import["))),
                                                                 boost::regex("[^0-9]*([0-9]+).*"),
                                                                 string("\\1"));

                            if (line.find("$import[" + number + "]$", 0) != NOT_FOUND)
                            {
                                if (!isOnlyNumber(number)) { ErrorMessage(1154, it->first, j + 1); }

                                int num = stoi(number);
                                vecstr keywords;
                                string templine = iter->first;
                                size_t nextWord;
                                size_t previousWord = 0;

                                while (true)
                                {
                                    nextWord = templine.find("!~^!", previousWord);

                                    if (nextWord != NOT_FOUND)
                                    {
                                        keywords.push_back(
                                            templine.substr(previousWord, nextWord - previousWord));
                                        previousWord = nextWord + 4;
                                    }
                                    else
                                    {
                                        keywords.push_back(templine.substr(previousWord));
                                        break;
                                    }
                                }

                                if (num - 2 >= int(keywords.size())) { ErrorMessage(1169, it->first, j + 1); }

                                line.replace(line.find("$import[" + number + "]$"),
                                             10 + number.length(),
                                             keywords[num - 2]);
                            }
                        }
                    }

                    if (line.find("$crc32[") != NOT_FOUND
                        && line.find("]$", line.find("crc32[")) != NOT_FOUND)
                    { CRC32Replacer(line, "import", it->first, j + 1); }

                    if (line.find("$import[", 0) != NOT_FOUND
                        && line.find("]$", line.find("$import[" + 1)) != NOT_FOUND)
                    {
                        size_t nextpos         = line.find("$import[");
                        string importer        = line.substr(nextpos + 1, line.find("]$") - nextpos);
                        size_t bracketCount    = count(importer.begin(), importer.end(), '[');
                        size_t altBracketCount = count(importer.begin(), importer.end(), ']');

                        if (IDExist[importer].length() == 0)
                        {
                            if (bracketCount != altBracketCount)
                            { ErrorMessage(1139, "import", it->first, j + 1, importer); }

                            size_t pos  = importer.find('[') + 1;
                            string file = importer.substr(pos, importer.find(']', pos) - pos);
                            string keyword;
                            string tempID;

                            if (bracketCount > 1)
                            {
                                pos = importer.find('[', pos);
                                string tempKeyword
                                    = importer.substr(pos, importer.find_last_of(']') + 1 - pos);
                                int openBrack = 0;

                                for (char curChar : tempKeyword)
                                {
                                    if (curChar == '[') { ++openBrack; }
                                    else if (curChar == ']')
                                    {
                                        --openBrack;

                                        if (openBrack == 0) { keyword.append("!~^!"); }
                                    }
                                    else
                                    {
                                        keyword = keyword + curChar;
                                    }
                                }

                                pos = keyword.rfind("!~^!");

                                if (openBrack != 0 || pos == NOT_FOUND || pos != keyword.length() - 4)
                                { ErrorMessage(1139, "import", it->first, j + 1, importer); }
                                else
                                {
                                    keyword = keyword.substr(0, keyword.length() - 4);
                                }
                            }
                            else
                            {
                                keyword = "";
                            }

                            for (auto& val : ExportID)
                            {
                                if (val[file][keyword].length() > 0)
                                {
                                    tempID = val[file][keyword];
                                    break;
                                }
                            }

                            if (tempID.length() == 0)
                            {
                                tempID = to_string(lastID);

                                while (tempID.length() < 4)
                                {
                                    tempID = "0" + tempID;
                                }

                                IDExist[importer]          = tempID;
                                newExportID[file][keyword] = tempID;
                                ++lastID;

                                if (lastID == 9216) { ++lastID; }
                            }

                            line.replace(nextpos, importer.length() + 2, tempID);
                        }
                        else
                        {
                            line.replace(nextpos, importer.length() + 2, IDExist[importer]);
                        }
                    }

                    if (line.find("$MD$") != NOT_FOUND) { ErrorMessage(1096, "import", filename, j + 1); }

                    if (line.find("$RD$") != NOT_FOUND) { ErrorMessage(1097, "import", filename, j + 1); }

                    if (line.find("MID$", 0) != NOT_FOUND)
                    {
                        int reference = sameWordCount(line, "MID$");

                        for (int k = 0; k < reference; ++k)
                        {
                            string tempID = line.substr(line.find("MID$"));
                            string number = boost::regex_replace(
                                string(tempID), boost::regex("[^0-9]*([0-9]+).*"), string("\\1"));
                            string oldID = "MID$" + number;

                            if (line.find(oldID) != NOT_FOUND)
                            {
                                size_t nextpos = line.find(oldID);

                                if (IDExist[oldID].length() > 0) { tempID = IDExist[oldID]; }
                                else
                                {
                                    tempID = to_string(lastID);

                                    while (tempID.length() < 4)
                                    {
                                        tempID = "0" + tempID;
                                    }

                                    IDExist[oldID] = tempID;
                                    ++lastID;

                                    if (lastID == 9216) { ++lastID; }
                                }

                                line.replace(nextpos, oldID.length(), tempID);
                            }
                            else
                            {
                                ErrorMessage(1028, iter->first, j + 1);
                            }
                        }
                    }

                    if (elementCatch) { elementLine = behaviorlines.size(); }

                    behaviorlines.push_back(line);
                }

                if (!behaviorlines.empty() && behaviorlines.back().length() != 0)
                { behaviorlines.push_back(""); }
            }
        }
    }

    if (!newExportID.empty())
    {
        ExportID.push_back(newExportID);
        vecstr additionlines = importOutput(ExportID, int(ExportID.size() - 1), lastID, std::move(file));
        behaviorlines.reserve(behaviorlines.size() + additionlines.size());
        behaviorlines.insert(behaviorlines.end(), additionlines.begin(), additionlines.end());
    }
    else
    {
        behaviorlines.shrink_to_fit();
    }

    return behaviorlines;
}
