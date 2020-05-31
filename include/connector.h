#ifndef MASTER_H_
#define MASTER_H_

#include "Global.h"

#include <map>
#include <string>
#include <vector>

#include <QObject>

class UiModInfo;

extern std::map<std::wstring, std::shared_ptr<UiModInfo>> modinfo;
extern std::mutex processlock;
extern std::condition_variable cv;
extern bool processdone;

class DummyLog : public QObject
{
    Q_OBJECT

public:
    void message(std::string input);
    void message(std::wstring input);

signals:
    void incomingMessage(QString);
};

bool readMod(std::wstring& errormsg);
VecStr getHiddenMods();

#endif
