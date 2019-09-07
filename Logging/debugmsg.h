#ifndef DEBUGMSG_H_
#define DEBUGMSG_H_

#include <unordered_map>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QMessageBox>
#include "Nemesis Main GUI/src/DebugLog.h"

class UpdateFilesStart;
class BehaviorStart;
class DummyLog;

extern bool error;	// get error warning
extern bool isPatch;

extern vecstr warningMsges;

namespace nemesis
{
	class exception {};
}

// debugging
struct DebugMsg
{
	std::unordered_map<int, std::string> errorlist;
	std::unordered_map<int, std::string> warninglist;
	std::unordered_map<int, std::string> uilist;
	std::unordered_map<int, std::string> textlist;

	DebugMsg() {}
	DebugMsg(std::string language);
};

// send direct message on log
void interMsg(std::string);

// add new language pack
void NewDebugMessage(DebugMsg NewLog);

// error release/lock
void ErrorLock();
void ErrorRelease();

std::string DMLogError(int errorcode);
std::string DMLogWarning(int warningcode);

std::string EngLogError(int errorcode);
std::string EngLogWarning(int warningcode);

// declaration
template <typename current>
void AdditionalInput(std::string& message, int counter, current input);

template <typename current, typename ... other>
void AdditionalInput(std::string& message, int counter, current input, other... rest);

// definition
template <typename current>
void AdditionalInput(std::string& message, int counter, current input)
{
	std::string newInput = "<" + std::to_string(counter) + ">";
	std::string replacement = static_cast<std::ostringstream*>(&(std::ostringstream() << input))->str();
	int ref = sameWordCount(message, newInput);
	
	if (ref != 0)
	{
		for (int i = 0; i < ref; ++i)
		{
			message.replace(message.find(newInput), newInput.size(), replacement);
		}
	}
	else
	{
		string msg = "CRITICAL ERROR: Wrong error input. Please re-install Nemesis";
		interMsg(msg + "\n");
		DebugLogging(msg);
		error = true;
		return;
	}
}

template <typename current, typename ... other>
void AdditionalInput(std::string& message, int counter, current input, other... rest)
{
	std::string newInput = "<" + std::to_string(counter) + ">";
	std::string replacement = static_cast<std::ostringstream*>(&(std::ostringstream() << input))->str();
	int ref = sameWordCount(message, newInput);

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
		string msg = "CRITICAL ERROR: Wrong error input. Please re-install Nemesis";
		interMsg(msg + "\n");
		DebugLogging(msg);
		error = true;
		return;
	}
}

// error
inline void ErrorMessage(int errorcode)
{
	ErrorLock();

	if (error)
	{
		ErrorRelease();
		throw nemesis::exception();
	}

	error = true;
	std::string errormsg = "ERROR(" + std::to_string(errorcode) + "): " + DMLogError(errorcode);

	if (DMLogError(errorcode).length() == 0)
	{
		interMsg("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
		return;
	}

	interMsg(errormsg + "\n");
	DebugLogging("ERROR(" + std::to_string(errorcode) + "): " + EngLogError(errorcode));
	ErrorRelease();
	throw nemesis::exception();
}

template <typename ... other>
inline void ErrorMessage(int errorcode, other... rest)
{
	ErrorLock();

	if (error)
	{
		ErrorRelease();
		throw nemesis::exception();
	}

	error = true;
	std::string errormsg = "ERROR(" + std::to_string(errorcode) + "): " + DMLogError(errorcode);
	std::string englog = "ERROR(" + std::to_string(errorcode) + "): " + EngLogError(errorcode);

	if (DMLogError(errorcode).length() == 0)
	{
		errormsg = "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis";
		interMsg(errormsg + "\n");
		return;
	}

	AdditionalInput(errormsg, 1, rest...);
	AdditionalInput(englog, 1, rest...);
	interMsg(errormsg + "\n");
	DebugLogging(englog);
	ErrorRelease();
	throw nemesis::exception();
}

// warning
inline void WarningMessage(int warningcode)
{
	if (error) throw nemesis::exception();

	std::string warninmsg = "WARNING(" + std::to_string(warningcode) + "): " + DMLogWarning(warningcode);

	if (DMLogWarning(warningcode).length() == 0)
	{
		warninmsg = "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis";
		interMsg(warninmsg + "\n");
		error = true;
		return;
	}

	warningMsges.push_back(warninmsg + "\n");
	DebugLogging("WARNING(" + std::to_string(warningcode) + "): " + EngLogWarning(warningcode));
}

template <typename ... other>
inline void WarningMessage(int warningcode, other... rest)
{
	if (error) throw nemesis::exception();

	std::string warninmsg = "WARNING(" + std::to_string(warningcode) + "): " + DMLogWarning(warningcode);
	std::string englog = "WARNING(" + std::to_string(warningcode) + "): " + EngLogWarning(warningcode);

	if (DMLogWarning(warningcode).length() == 0)
	{
		warninmsg = "CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis";
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

// connect, get and disconnect running process
void connectProcess(UpdateFilesStart* newProcess);
void connectProcess(BehaviorStart* newProcess);
void connectProcess(DummyLog* newProcess);
void disconnectProcess();

#endif
