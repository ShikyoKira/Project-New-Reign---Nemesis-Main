#include <locale>
#include <codecvt>

#include "Global.h"

#include "debuglog.h"
#include "debugmsg.h"

#include "utilities/algorithm.h"
#include "utilities/readtextfile.h"
#include "utilities/writetextfile.h"

using namespace std;

int filenum;
bool error = false;

mutex err_Mutex;

atomic<int> progressPercentage;

DebugMsg* DMLog;
DebugMsg* EnglishLog;

VecWstr readUTF8File(wstring filename);

void writeUTF8File(string filename, VecStr storeline);

void NewDebugMessage(DebugMsg& NewLog)
{
    if (!EnglishLog) EnglishLog = new DebugMsg("english");

    DMLog = &NewLog;
}

DebugMsg::DebugMsg(string language)
{
    setup(nemesis::transform_to<wstring>(language));
}

DebugMsg::DebugMsg(wstring language)
{
    setup(language);
}

void DebugMsg::setup(const wstring& language)
{
    wstring filename = L"languages\\" + language + L".txt";
    VecWstr storeline = readUTF8File(filename);

    if (error) throw nemesis::exception();

    for (size_t i = 0; i < storeline.size(); ++i)
    {
        if (storeline[i][0] != '\'' && storeline[i].length() != 0)
        {
            __int64 counter = sameWordCount(storeline[i], L"\\n");

            for (int j = 0; j < counter; ++j)
            {
                storeline[i].replace(storeline[i].find(L"\\n"), 2, L"\n");
            }

            if (wordFind(storeline[i], L"UIT ") == 0)
            {
                if (storeline[i].find(L"=") != NOT_FOUND)
                {
                    wstring code = storeline[i].substr(4, storeline[i].find(L"=") - 4);

                    if (isOnlyNumber(code)) uilist[stoi(code)] = storeline[i].substr(storeline[i].find(L"=") + 1);

                }
            }
            else if (wordFind(storeline[i], L"TBT ") == 0)
            {
                if (storeline[i].find(L"=") != NOT_FOUND)
                {
                    wstring code = storeline[i].substr(4, storeline[i].find(L"=") - 4);

                    if (isOnlyNumber(code)) textlist[stoi(code)] = storeline[i].substr(storeline[i].find(L"=") + 1);
                }
            }
            else if (wordFind(storeline[i], L"ERR ") == 0)
            {
                if (storeline[i].find(L"=") != NOT_FOUND)
                {
                    wstring code = storeline[i].substr(4, storeline[i].find(L"=") - 4);

                    if (isOnlyNumber(code)) errorlist[stoi(code)] = storeline[i].substr(storeline[i].find(L"=") + 1);
                }
            }
            else if (wordFind(storeline[i], L"WAR ") == 0)
            {
                if (storeline[i].find(L"=") != NOT_FOUND)
                {
                    wstring code = storeline[i].substr(4, storeline[i].find(L"=") - 4);

                    if (isOnlyNumber(code)) warninglist[stoi(code)] = storeline[i].substr(storeline[i].find(L"=") + 1);
                }
            }
        }
    }
}

VecWstr readUTF8File(wstring filename)
{
    VecWstr storeline;
    FileReader file(filename);
    wstring line;

    if (file.TryGetFile())
    {
        while (file.TryGetLines(line))
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
        for (size_t i = 0; i < storeline.size(); ++i)
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

wstring DMLogError(int errorcode)
{
    return DMLog->errorlist[errorcode];
}

wstring DMLogWarning(int warningcode)
{
    return DMLog->warninglist[warningcode];
}

string EngLogError(int errorcode)
{
    return nemesis::transform_to<string>(EnglishLog->errorlist[errorcode]);
}

string EngLogWarning(int warningcode)
{
    return nemesis::transform_to<string>(EnglishLog->warninglist[warningcode]);
}

void InsertMessageParam(string& message, const string& newinput, const string& replacement)
{
    int ref = sameWordCount(message, newinput);

    if (ref == 0)
    {
        string msg = "CRITICAL ERROR: Wrong error input. Please re-install Nemesis";
        interMsg(msg + "\n");
        DebugLogging(msg);
        error = true;
        return;
    }

    for (int i = 0; i < ref; ++i)
    {
        message.replace(message.find(newinput), newinput.size(), replacement);
    }
}

void InsertMessageParam(wstring& message, const wstring& newinput, const wstring& replacement)
{
    int ref = sameWordCount(message, newinput);

    if (ref == 0)
    {
        wstring msg = L"CRITICAL ERROR: Wrong error input. Please re-install Nemesis";
        interMsg(msg + L"\n");
        DebugLogging(msg);
        error = true;
        return;
    }

    for (int i = 0; i < ref; ++i)
    {
        message.replace(message.find(newinput), newinput.size(), replacement);
    }
}

void AdditionalInput(std::string& message, int counter, const std::string& input)
{
    string newinput = "<" + to_string(counter) + ">";
    InsertMessageParam(message, newinput, input);
}

void AdditionalInput(std::string& message, int counter, const std::string_view& input)
{
    string newinput    = "<" + to_string(counter) + ">";
    InsertMessageParam(message, newinput, string(input));
}

void AdditionalInput(string& message, int counter, const wstring& input)
{
    string newinput    = "<" + to_string(counter) + ">";
    InsertMessageParam(message, newinput, nemesis::transform_to(input));
}

void AdditionalInput(string& message, int counter, const wstring_view& input)
{
    string newinput    = "<" + to_string(counter) + ">";
    InsertMessageParam(message, newinput, nemesis::transform_to(wstring(input)));
}

void AdditionalInput(string& message, int counter, const filesystem::path& input)
{
    string newinput    = "<" + to_string(counter) + ">";
    string replacement = input.string();
    InsertMessageParam(message, newinput, replacement);
}

void AdditionalInput(std::string& message, int counter, const nemesis::Line& input)
{
    string newinput    = "<" + to_string(counter) + ">";
    string replacement = input.ToString();
    InsertMessageParam(message, newinput, replacement);
}

void AdditionalInput(std::wstring& message, int counter, const std::wstring& input)
{
    wstring newinput = L"<" + to_wstring(counter) + L">";
    InsertMessageParam(message, newinput, input);
}

void AdditionalInput(std::wstring& message, int counter, const std::wstring_view& input)
{
    wstring newinput = L"<" + to_wstring(counter) + L">";
    InsertMessageParam(message, newinput, wstring(input));
}

void AdditionalInput(std::wstring& message, int counter, const char* input)
{
    wstring newinput = L"<" + to_wstring(counter) + L">";
    InsertMessageParam(message, newinput, nemesis::transform_to(input));
}

void AdditionalInput(wstring& message, int counter, const string& input)
{
    wstring newinput    = L"<" + to_wstring(counter) + L">";
    wstring replacement = nemesis::transform_to<wstring>(input);
    InsertMessageParam(message, newinput, replacement);
}

void AdditionalInput(wstring& message, int counter, const string_view& input)
{
    wstring newinput    = L"<" + to_wstring(counter) + L">";
    wstring replacement = nemesis::transform_to<wstring>(input);
    InsertMessageParam(message, newinput, replacement);
}

void AdditionalInput(wstring& message, int counter, const filesystem::path& input)
{
    wstring newinput    = L"<" + to_wstring(counter) + L">";
    wstring replacement = input.wstring();
    InsertMessageParam(message, newinput, replacement);
}

void AdditionalInput(wstring& message, int counter, const nemesis::Line& input)
{
    wstring newinput    = L"<" + to_wstring(counter) + L">";
    wstring replacement = nemesis::transform_to<wstring>(input.ToString());
    InsertMessageParam(message, newinput, replacement);
}

void ErrorMessage(int errorcode)
{
    scoped_lock<mutex> err_Lock(err_Mutex);

    if (error) throw nemesis::exception();

    error                 = true;
    wstring errormsg = L"ERROR(" + to_wstring(errorcode) + L"): " + DMLogError(errorcode);

    if (DMLogError(errorcode).length() == 0)
    {
        interMsg(
            "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
        return;
    }

    interMsg(errormsg + L"\n");
    DebugLogging("ERROR(" + to_string(errorcode) + "): " + EngLogError(errorcode));
    throw nemesis::exception();
}

void WarningMessage(int warningcode)
{
    scoped_lock<mutex> err_Lock(err_Mutex);

    if (error) throw nemesis::exception();

    wstring warninmsg = L"WARNING(" + to_wstring(warningcode) + L"): " + DMLogWarning(warningcode);

    if (DMLogWarning(warningcode).length() == 0)
    {
        warninmsg
            = L"CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis";
        interMsg(warninmsg + L"\n");
        error = true;
        return;
    }

    warningMsges.push_back(warninmsg + L"\n");
    DebugLogging("WARNING(" + to_string(warningcode) + "): " + EngLogWarning(warningcode));
}


wstring TextBoxMessage(int textcode)
{
    if (DMLog->textlist[textcode].length() == 0)
    {
        interMsg(
            "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
        error = true;
        return L"";
    }

    return DMLog->textlist[textcode];
}

string EngTextBoxMessage(int textcode)
{
    if (EnglishLog->textlist[textcode].length() == 0)
    {
        interMsg(
            "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
        error = true;
        return "";
    }

    return nemesis::transform_to<string>(EnglishLog->textlist[textcode]);
}

wstring UIMessage(int uicode)
{
    if (DMLogWarning(uicode).length() == 0)
    {
        interMsg(
            "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
        error = true;
        return L"";
    }

    return DMLog->uilist[uicode];
}
