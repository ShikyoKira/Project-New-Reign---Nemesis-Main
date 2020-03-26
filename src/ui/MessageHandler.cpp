#include "header/connector.h"

#include "header/debuglog.h"

#include "header/ui/ErrorMsgBox.h"
#include "header/ui/MessageHandler.h"

#include "header/update/updateprocess.h"

#include "header/generate/behaviorprocess.h"

UpdateFilesStart* process1;
BehaviorStart* process2;
DummyLog* process3;

extern void (*interMsgPtr)(std::string);

void exterMsg(std::string input)
{
	if (process1)
	{
		process1->message(input);
	}
	else if (process2)
	{
		process2->message(input);
	}
	else if (process3)
	{
		process3->message(input);
	}
	else
	{
		DebugLogging("Non-captured message: " + input);
		CEMsgBox* msgbox = new CEMsgBox;
		msgbox->setWindowTitle("CRITICAL ERROR");
		msgbox->setText("Access process violation. Running process not found. Report to Nemesis' author immediately.");
		msgbox->show();
		error = true;
	}
}

void initializeHandler()
{
	interMsgPtr = &exterMsg;
}

void connectProcess(UpdateFilesStart* newProcess)
{
	process1 = newProcess;
}

void connectProcess(BehaviorStart* newProcess)
{
	process2 = newProcess;
}

void connectProcess(DummyLog* newProcess)
{
	process3 = newProcess;
}

void disconnectProcess()
{
	DebugLogging("Standard log disconnected");
	process1 = nullptr;
	process2 = nullptr;
	process3 = nullptr;
}
