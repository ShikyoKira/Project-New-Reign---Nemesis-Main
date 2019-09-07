#ifndef MASTER_H_
#define MASTER_H_

#include "Global.h"

#include "src\process\updateprocess.h"
#include "src\process\behaviorprocess.h"
#include "src\process\behaviorsubprocess.h"

extern std::unordered_map<std::string, vecstr> modinfo;
extern std::mutex processlock;
extern std::condition_variable cv;
extern bool processdone;

class DummyLog : public QObject
{
	Q_OBJECT

public:
	void message(std::string input);

signals:
	void incomingMessage(QString);
};

bool readMod(std::string& errormsg);
vecstr getHiddenMods();

#endif