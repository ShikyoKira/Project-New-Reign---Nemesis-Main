#include "Global.h"


#include "debuglog.h"
#include "debugmsg.h"

#include "utilities/readtextfile.h"
#include "utilities/writetextfile.h"
#include "utilities/wstrconvert.h"

using namespace std;

int filenum;
bool error = false;

mutex err_Mutex;

atomic<int> progressPercentage;

DebugMsg DMLog;
DebugMsg* EnglishLog;

void (*interMsgPtr)(string);

VecStr readUTF8File(wstring filename);
void writeUTF8File(string filename, VecStr storeline);

void NewDebugMessage(DebugMsg NewLog)
{
    if (!EnglishLog) { EnglishLog = new DebugMsg("english"); }

    DMLog = NewLog;
}

DebugMsg::DebugMsg(string language)
{
    setup(StringToWString(language));
}

DebugMsg::DebugMsg(wstring language)
{
    setup(language);
}

void DebugMsg::setup(const wstring& language)
{
    wstring filename = L"languages\\" + language + L".txt";
    VecStr storeline = readUTF8File(filename);

    if (error) throw nemesis::exception();

    for (uint i = 0; i < storeline.size(); ++i)
    {
        if (storeline[i][0] != '\'' && storeline[i].length() != 0)
        {
            __int64 counter = sameWordCount(storeline[i], "\\n");

            for (int j = 0; j < counter; ++j)
            {
                storeline[i].replace(storeline[i].find("\\n"), 2, "\n");
            }

            if (wordFind(storeline[i], "UIT ") == 0)
            {
                if (storeline[i].find("=") != NOT_FOUND)
                {
                    string code = storeline[i].substr(4, storeline[i].find("=") - 4);

                    if (isOnlyNumber(code))
                    { uilist[stoi(code)] = storeline[i].substr(storeline[i].find("=") + 1); }
                }
            }
            else if (wordFind(storeline[i], "TBT ") == 0)
            {
                if (storeline[i].find("=") != NOT_FOUND)
                {
                    string code = storeline[i].substr(4, storeline[i].find("=") - 4);

                    if (isOnlyNumber(code))
                    { textlist[stoi(code)] = storeline[i].substr(storeline[i].find("=") + 1); }
                }
            }
            else if (wordFind(storeline[i], "ERR ") == 0)
            {
                if (storeline[i].find("=") != NOT_FOUND)
                {
                    string code = storeline[i].substr(4, storeline[i].find("=") - 4);

                    if (isOnlyNumber(code))
                    { errorlist[stoi(code)] = storeline[i].substr(storeline[i].find("=") + 1); }
                }
            }
            else if (wordFind(storeline[i], "WAR ") == 0)
            {
                if (storeline[i].find("=") != NOT_FOUND)
                {
                    string code = storeline[i].substr(4, storeline[i].find("=") - 4);

                    if (isOnlyNumber(code))
                    { warninglist[stoi(code)] = storeline[i].substr(storeline[i].find("=") + 1); }
                }
            }
        }
    }
}

VecStr readUTF8File(wstring filename)
{
    VecStr storeline;
    FileReader file(filename);
    string line;

    if (file.GetFile())
    {
        while (file.GetLines(line))
        {
            storeline.push_back(line);
        }
    }
    else
    {
        interMsg("CRITICAL ERROR: Failed to read language pack. Please re-install Nemesis\n");
        error = true;
    }

    return storeline;
}

void writeUTF8File(string filename, VecStr storeline)
{
    FileWriter output(filename);

    if (output.is_open())
    {
        for (uint i = 0; i < storeline.size(); ++i)
        {
            output << storeline[i] << "\n";
        }
    }
    else
    {
        interMsg("CRITICAL ERROR: Failed to write file. Please re-install Nemesis\n");
        error = true;
    }
}

string DMLogError(int errorcode)
{
    return DMLog.errorlist[errorcode];
}

string DMLogWarning(int warningcode)
{
    return DMLog.warninglist[warningcode];
}

string EngLogError(int errorcode)
{
    return EnglishLog->errorlist[errorcode];
}

string EngLogWarning(int warningcode)
{
    return EnglishLog->warninglist[warningcode];
}

void ErrorMessage(int errorcode)
{
    std::scoped_lock<std::mutex> err_Lock(err_Mutex);

    if (error) { throw nemesis::exception(); }

    error                = true;
    std::string errormsg = "ERROR(" + std::to_string(errorcode) + "): " + DMLogError(errorcode);

    if (DMLogError(errorcode).length() == 0)
    {
        interMsg(
            "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
        return;
    }

    interMsg(errormsg + "\n");
    DebugLogging("ERROR(" + std::to_string(errorcode) + "): " + EngLogError(errorcode));
    throw nemesis::exception();
}

string TextBoxMessage(int textcode)
{
    if (DMLogWarning(textcode).length() == 0)
    {
        interMsg(
            "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
        error = true;
        return "";
    }

    return DMLog.textlist[textcode];
}

string EngTextBoxMessage(int textcode)
{
    if (DMLogWarning(textcode).length() == 0)
    {
        interMsg(
            "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
        error = true;
        return "";
    }

    return EnglishLog->textlist[textcode];
}

string UIMessage(int uicode)
{
    if (DMLogWarning(uicode).length() == 0)
    {
        interMsg(
            "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
        error = true;
        return "";
    }

    return DMLog.uilist[uicode];
}

void interMsg(string input)
{
    // connect to UI Message Handler
    (*interMsgPtr)(input);
}
