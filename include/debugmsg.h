#ifndef DEBUGMSG_H_
#define DEBUGMSG_H_

#include <memory>
#include <mutex>
#include <unordered_map>

#include "debuglog.h"

extern bool error; // get error warning
extern bool isPatch;

extern vecstr warningMsges;
extern std::mutex err_Mutex;

namespace nemesis
{
    class exception
    {};
} // namespace nemesis

// debugging
struct DebugMsg
{
    std::unordered_map<int, std::string> errorlist;
    std::unordered_map<int, std::string> warninglist;
    std::unordered_map<int, std::string> uilist;
    std::unordered_map<int, std::string> textlist;

    DebugMsg()
    {}
    DebugMsg(std::string language);
    DebugMsg(std::wstring language);

private:
    void setup(const std::wstring& language);
};

// send direct message on log
void interMsg(std::string);

// add new language pack
void NewDebugMessage(DebugMsg NewLog);

std::string DMLogError(int errorcode);
std::string DMLogWarning(int warningcode);

std::string EngLogError(int errorcode);
std::string EngLogWarning(int warningcode);

int sameWordCount(std::string, std::string); //Forward declare

template <typename current>
void AdditionalInput(std::string& message, int counter, current input)
{
    std::string newInput    = "<" + std::to_string(counter) + ">";
    std::string replacement = static_cast<std::ostringstream*>(&(std::ostringstream() << input))->str();
    int ref                 = sameWordCount(message, newInput);

    if (ref != 0)
    {
        for (int i = 0; i < ref; ++i)
        {
            message.replace(message.find(newInput), newInput.size(), replacement);
        }
    }
    else
    {
        std::string msg = "CRITICAL ERROR: Wrong error input. Please re-install Nemesis";
        interMsg(msg + "\n");
        DebugLogging(msg);
        error = true;
        return;
    }
}

template <typename current, typename... other>
void AdditionalInput(std::string& message, int counter, current input, other... rest)
{
    std::string newInput    = "<" + std::to_string(counter) + ">";
    std::string replacement = static_cast<std::ostringstream*>(&(std::ostringstream() << input))->str();
    int ref                 = sameWordCount(message, newInput);

    if (ref != 0)
    {
        for (int i = 0; i < ref; ++i)
        {
            message.replace(message.find(newInput), newInput.size(), replacement);
        }

        AdditionalInput(message, counter + 1, rest...);
    }
    else
    {
        std::string msg = "CRITICAL ERROR: Wrong error input. Please re-install Nemesis";
        interMsg(msg + "\n");
        DebugLogging(msg);
        error = true;
        return;
    }
}

// error
void ErrorMessage(int errorcode);

template <typename... other>
inline void ErrorMessage(int errorcode, other... rest)
{
    std::scoped_lock<std::mutex> err_Lock(err_Mutex);

    if (error) throw nemesis::exception();

    error                = true;
    std::string errormsg = "ERROR(" + std::to_string(errorcode) + "): " + DMLogError(errorcode);
    std::string englog   = "ERROR(" + std::to_string(errorcode) + "): " + EngLogError(errorcode);

    if (DMLogError(errorcode).length() == 0)
    {
        errormsg
            = "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis";
        interMsg(errormsg + "\n");
        return;
    }

    AdditionalInput(errormsg, 1, rest...);
    AdditionalInput(englog, 1, rest...);
    interMsg(errormsg + "\n");
    DebugLogging(englog);
    throw nemesis::exception();
}

// warning
inline void WarningMessage(int warningcode)
{
    std::scoped_lock<std::mutex> err_Lock(err_Mutex);

    if (error) throw nemesis::exception();

    std::string warninmsg = "WARNING(" + std::to_string(warningcode) + "): " + DMLogWarning(warningcode);

    if (DMLogWarning(warningcode).length() == 0)
    {
        warninmsg
            = "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis";
        interMsg(warninmsg + "\n");
        error = true;
        return;
    }

    warningMsges.push_back(warninmsg + "\n");
    DebugLogging("WARNING(" + std::to_string(warningcode) + "): " + EngLogWarning(warningcode));
}

template <typename... other>
inline void WarningMessage(int warningcode, other... rest)
{
    std::scoped_lock<std::mutex> err_Lock(err_Mutex);

    if (error) throw nemesis::exception();

    std::string warninmsg = "WARNING(" + std::to_string(warningcode) + "): " + DMLogWarning(warningcode);
    std::string englog    = "WARNING(" + std::to_string(warningcode) + "): " + EngLogWarning(warningcode);

    if (DMLogWarning(warningcode).length() == 0)
    {
        warninmsg
            = "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis";
        interMsg(warninmsg + "\n");
        error = true;
        return;
    }

    AdditionalInput(warninmsg, 1, rest...);
    AdditionalInput(englog, 1, rest...);
    warningMsges.push_back(warninmsg + "\n");
    DebugLogging(englog);
}

// TextBox
std::string TextBoxMessage(int textcode);
std::string EngTextBoxMessage(int textcode);

// UI
std::string UIMessage(int uicode);

#endif
