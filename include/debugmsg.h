#ifndef DEBUGMSG_H_
#define DEBUGMSG_H_

#include <mutex>
#include <memory>
#include <sstream>

#include "debuglog.h"

#include "utilities/types.h"
#include "utilities/algorithm.h"

#include "ui/MessageHandler.h"

extern bool error; // get error warning
extern bool isPatch;

extern VecWstr warningMsges;
extern std::mutex err_Mutex;

namespace nemesis
{
    class exception
    {};
} // namespace nemesis

// debugging
struct DebugMsg
{
    std::unordered_map<int, std::wstring> errorlist;
    std::unordered_map<int, std::wstring> warninglist;
    std::unordered_map<int, std::wstring> uilist;
    std::unordered_map<int, std::wstring> textlist;

    DebugMsg()
    {}
    DebugMsg(std::string language);
    DebugMsg(std::wstring language);

private:
    void setup(const std::wstring& language);
};

namespace nemesis
{
    struct Message
    {

    };
}

// add new language pack
void NewDebugMessage(DebugMsg NewLog);

std::wstring DMLogError(int errorcode);
std::wstring DMLogWarning(int warningcode);

std::string EngLogError(int errorcode);
std::string EngLogWarning(int warningcode);

void InsertMessageParam(std::string& message, const std::string& newinput, const std::string& replacement);
void InsertMessageParam(std::wstring& message, const std::wstring& newinput, const std::wstring& replacement);

void AdditionalInput(std::string& message, int counter, const std::string& input);
void AdditionalInput(std::string& message, int counter, const std::string_view& input);
void AdditionalInput(std::string& message, int counter, const std::wstring& input);
void AdditionalInput(std::string& message, int counter, const std::wstring_view& input);
void AdditionalInput(std::string& message, int counter, const std::filesystem::path& input);
void AdditionalInput(std::string& message, int counter, const nemesis::Line& input);

template <typename type>
inline void AdditionalInput(std::string& message, int counter, type input)
{
    std::string newinput    = "<" + std::to_string(counter) + ">";
    std::string replacement = (std::ostringstream() << input).str();
    InsertMessageParam(message, newinput, replacement);
}

template <typename... other>
inline void AdditionalInput(std::string& message, int counter, const std::wstring& input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

template <typename... other>
inline void AdditionalInput(std::string& message, int counter, const std::wstring_view& input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

template <typename... other>
inline void
AdditionalInput(std::string& message, int counter, const std::filesystem::path& input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

template <typename... other>
inline void
AdditionalInput(std::string& message, int counter, const nemesis::Line& input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

template <typename type, typename... other>
inline void AdditionalInput(std::string& message, int counter, type input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

void AdditionalInput(std::wstring& message, int counter, const std::wstring& input);
void AdditionalInput(std::wstring& message, int counter, const std::wstring_view& input);
void AdditionalInput(std::wstring& message, int counter, const char* input);
void AdditionalInput(std::wstring& message, int counter, const std::string& input);
void AdditionalInput(std::wstring& message, int counter, const std::string_view& input);
void AdditionalInput(std::wstring& message, int counter, const std::filesystem::path& input);
void AdditionalInput(std::wstring& message, int counter, const nemesis::Line& input);

template <typename type>
inline void AdditionalInput(std::wstring& message, int counter, type input)
{
    std::wstring newinput    = L"<" + std::to_wstring(counter) + L">";
    std::wstring replacement = (std::wostringstream() << input).str();
    InsertMessageParam(message, newinput, replacement);
}

template <typename... other>
inline void AdditionalInput(std::wstring& message, int counter, const char* input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

template <typename... other>
inline void AdditionalInput(std::wstring& message, int counter, const std::string& input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

template <typename... other>
inline void AdditionalInput(std::wstring& message, int counter, const std::string_view& input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

template <typename... other>
inline void
AdditionalInput(std::wstring& message, int counter, const std::filesystem::path& input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

template <typename... other>
inline void
AdditionalInput(std::wstring& message, int counter, const nemesis::Line& input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

template <typename type, typename... other>
inline void AdditionalInput(std::wstring& message, int counter, type input, other... rest)
{
    AdditionalInput(message, counter, input);
    AdditionalInput(message, counter + 1, rest...);
}

// error
void ErrorMessage(int errorcode);

template <typename... other>
inline void ErrorMessage(int errorcode, other... rest)
{
    std::scoped_lock<std::mutex> err_Lock(err_Mutex);

    if (error) throw nemesis::exception();

    error                 = true;
    std::wstring errormsg = L"ERROR(" + std::to_wstring(errorcode) + L"): " + DMLogError(errorcode);
    std::string englog    = "ERROR(" + std::to_string(errorcode) + "): " + EngLogError(errorcode);

    if (DMLogError(errorcode).length() == 0)
    {
        errormsg
            = L"CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis";
        interMsg(errormsg + L"\n");
        return;
    }

    AdditionalInput(errormsg, 1, rest...);
    AdditionalInput(englog, 1, rest...);
    interMsg(errormsg + L"\n");
    DebugLogging(englog);
    throw nemesis::exception();
}

// warning
void WarningMessage(int warningcode);

template <typename... other>
inline void WarningMessage(int warningcode, other... rest)
{
    std::scoped_lock<std::mutex> err_Lock(err_Mutex);

    if (error) throw nemesis::exception();

    std::wstring warninmsg = L"WARNING(" + std::to_wstring(warningcode) + L"): " + DMLogWarning(warningcode);
    std::string englog     = "WARNING(" + std::to_string(warningcode) + "): " + EngLogWarning(warningcode);

    if (DMLogWarning(warningcode).length() == 0)
    {
        warninmsg
            = L"CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis";
        interMsg(warninmsg + L"\n");
        error = true;
        return;
    }

    AdditionalInput(warninmsg, 1, rest...);
    AdditionalInput(englog, 1, rest...);
    warningMsges.push_back(warninmsg + L"\n");
    DebugLogging(englog);
}

// TextBox
std::wstring TextBoxMessage(int textcode);
std::string EngTextBoxMessage(int textcode);

// UI
std::wstring UIMessage(int uicode);

#endif
