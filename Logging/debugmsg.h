#ifndef DEBUGMSG_H_
#define DEBUGMSG_H_

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

extern void NewDebugMessage(std::string language);

std::vector<std::string> readUTF8File(std::string filename);
extern void writeUTF8File(std::string filename, std::vector<std::string> storeline);

std::string DLogError(int errorcode);

std::string DLogWarning(int warningcode);

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
		// this->textbox->append("CRITICAL ERROR: Wrong error input. Please re-install Nemesis");
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
		// this->textbox->append("CRITICAL ERROR: Wrong error input. Please re-install Nemesis");
		return;
	}
}

// error
inline void ErrorMessage(int errorcode)
{
	error = true;
	std::string errormsg = "\nERROR(" + std::to_string(errorcode) + "): " + DLogError(errorcode) + "\n\n";

	if (DLogError(errorcode).length() == 0)
	{
		// this->textbox->append("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis");
		return;
	}

	// this->textbox->append(errormsg);
	std::cout << errormsg << std::endl;
}

template <typename ... other>
inline void ErrorMessage(int errorcode, other... rest)
{
	error = true;
	std::string errormsg = "\nERROR(" + std::to_string(errorcode) + "): " + DLogError(errorcode) + "\n\n";

	if (DLogError(errorcode).length() == 0)
	{
		// this->textbox->append("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis");
		return;
	}

	AdditionalInput(errormsg, 1, rest...);

	// this->textbox->append(errormsg);
	std::cout << errormsg << std::endl;
}

// warning
inline void WarningMessage(int warningcode)
{
	std::string warninmsg = "\nWARNING(" + std::to_string(warningcode) + "): " + DLogWarning(warningcode) + "\n\n";

	if (DLogWarning(warningcode).length() == 0)
	{
		// this->textbox->append("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis");
		return;
	}

	// this->textbox->append(warninmsg);
	std::cout << warninmsg << std::endl;
}

template <typename ... other>
inline void WarningMessage(int warningcode, other... rest)
{
	std::string warninmsg = "\nWARNING(" + std::to_string(warningcode) + "): " + DLogWarning(warningcode) + "\n\n";

	if (DLogWarning(warningcode).length() == 0)
	{
		// this->textbox->append("CRITICAL ERROR: Error code not found. Unable to diagnose problem. Please re-install Nemesis");
		return;
	}

	AdditionalInput(warninmsg, 1, rest...);

	// this->textbox->append(warninmsg);
	std::cout << warninmsg << std::endl;
}



#endif
