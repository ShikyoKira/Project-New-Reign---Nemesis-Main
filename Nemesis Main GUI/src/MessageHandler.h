#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_

class UpdateFilesStart;
class BehaviorStart;
class DummyLog;

void initializeHandler();

// connect, get and disconnect running process
void connectProcess(UpdateFilesStart* newProcess);
void connectProcess(BehaviorStart* newProcess);
void connectProcess(DummyLog* newProcess);
void disconnectProcess();

#endif
