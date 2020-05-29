#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_

class UpdateFilesStart;
class BehaviorStart;
class DummyLog;

// send direct message on log
void interMsg(std::string);
void interMsg(std::wstring);

// connect, get and disconnect running process
void connectProcess(UpdateFilesStart* newProcess);
void connectProcess(BehaviorStart* newProcess);
void connectProcess(DummyLog* newProcess);
void disconnectProcess();

#endif
