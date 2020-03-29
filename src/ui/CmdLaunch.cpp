#include "Global.h"

#include <QObject>
#include <QThread>

#include "connector.h"

#include "ui/CmdLaunch.h"
#include "ui/ErrorMsgBox.h"

#include "update/updateprocess.h"

#include "generate/behaviorprocess.h"

using namespace std;

extern mutex processlock;
extern condition_variable cv;
extern bool processdone;
extern map<string, vecstr> modinfo;

void CmdGenerateInitialize(vecstr modlist)
{
    string modcode, errmsg;

    if (!readMod(errmsg))
    {
        CEMsgBox* msgbox = new CEMsgBox;
        QString msg      = QString::fromStdString(errmsg);
        msgbox->setText(msg);
        msgbox->setWindowTitle("CRITITAL ERROR");
        msgbox->show();
        error = true;
        return;
    }

    vecstr behaviorPriority;
    vecstr hiddenModList = getHiddenMods();
    unordered_map<string, bool> chosenBehavior;

    for (auto& mod : hiddenModList)
    {
        behaviorPriority.push_back(mod);
        chosenBehavior[mod] = true;
    }

    for (auto& mod : modlist)
    {
        if (modinfo.find(mod) != modinfo.end() && modinfo[mod].size() > 0)
        {
            behaviorPriority.insert(behaviorPriority.begin(), mod);
            chosenBehavior[mod] = true;
        }
    }

    QThread* thread       = new QThread;
    BehaviorStart* worker = new BehaviorStart;
    worker->addBehaviorPick(behaviorPriority, chosenBehavior);

    QObject::connect(thread, SIGNAL(started()), worker, SLOT(GenerateBehavior()));
    QObject::connect(worker, SIGNAL(end()), thread, SLOT(quit()));
    QObject::connect(worker, SIGNAL(end()), worker, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    worker->moveToThread(thread);
    thread->start();

    {
        unique_lock<mutex> locker(processlock);

        while (!processdone)
        {
            cv.wait(locker);
        }
    }

    exit(static_cast<int>(error));
}

void CmdUpdateInitialize()
{
    QThread* thread          = new QThread;
    UpdateFilesStart* worker = new UpdateFilesStart;
    worker->cmdline          = true;

    QObject::connect(thread, SIGNAL(started()), worker, SLOT(UpdateFiles()));
    QObject::connect(worker, SIGNAL(end()), thread, SLOT(quit()));
    QObject::connect(worker, SIGNAL(end()), worker, SLOT(deleteLater()));
    QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    worker->moveToThread(thread);
    thread->start();

    {
        unique_lock<mutex> locker(processlock);

        while (!processdone)
        {
            cv.wait(locker);
        }
    }

    exit(static_cast<int>(error));
}
