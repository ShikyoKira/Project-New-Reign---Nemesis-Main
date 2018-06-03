#ifndef DEBUGMSG_H_
#define DEBUGMSG_H_

#include <unordered_map>
#include <string>
#include <vector>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QMessageBox>

class UpdateFilesStart;
class BehaviorStart;
class DummyLog;

extern bool error;	// get error warning

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
extern void NewDebugMessage(DebugMsg NewLog);

std::vector<std::string> readUTF8File(std::string filename);
extern void writeUTF8File(std::string filename, std::vector<std::string> storeline);

std::string DMLogError(int errorcode);
std::string DMLogWarning(int warningcode);

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
		interMsg("CRITICAL ERROR: Wrong error input. Please re-install Nemesis\n");
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
		interMsg("CRITICAL ERROR: Wrong error input. Please re-install Nemesis\n");
		error = true;
		return;
	}
}

// error
inline void ErrorMessage(int errorcode)
{
	error = true;
	std::string errormsg = "ERROR(" + std::to_string(errorcode) + "): " + DMLogError(errorcode) + "\n";

	if (DMLogError(errorcode).length() == 0)
	{
		interMsg("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
		return;
	}
	
	interMsg(errormsg);
}

template <typename ... other>
inline void ErrorMessage(int errorcode, other... rest)
{
	error = true;
	std::string errormsg = "ERROR(" + std::to_string(errorcode) + "): " + DMLogError(errorcode) + "\n";

	if (DMLogError(errorcode).length() == 0)
	{
		interMsg("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
		return;
	}

	AdditionalInput(errormsg, 1, rest...);

	interMsg(errormsg);
}

// warning
inline void WarningMessage(int warningcode)
{
	std::string warninmsg = "WARNING(" + std::to_string(warningcode) + "): " + DMLogWarning(warningcode) + "\n";

	if (DMLogWarning(warningcode).length() == 0)
	{
		interMsg("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
		error = true;
		return;
	}

	interMsg(warninmsg);
}

template <typename ... other>
inline void WarningMessage(int warningcode, other... rest)
{
	std::string warninmsg = "WARNING(" + std::to_string(warningcode) + "): " + DMLogWarning(warningcode) + "\n";

	if (DMLogWarning(warningcode).length() == 0)
	{
		interMsg("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis\n");
		error = true;
		return;
	}

	AdditionalInput(warninmsg, 1, rest...);

	interMsg(warninmsg);
}

// TextBox
std::string TextBoxMessage(int textcode);

// UI
std::string UIMessage(int uicode);

// connect, get and disconnect running process
void connectProcess(UpdateFilesStart* newProcess);
void connectProcess(BehaviorStart* newProcess);
void connectProcess(DummyLog* newProcess);
void disconnectProcess();

#endif
