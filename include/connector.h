#ifndef MASTER_H_
#define MASTER_H_

#include <map>
#include <string>
#include <vector>

#include <QObject>

extern std::map<std::string, std::vector<std::string>> modinfo;
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