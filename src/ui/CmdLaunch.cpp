#include "Global.h"

#include <conio.h>

#include <QObject>
#include <QThread>

#include "connector.h"

#include "core/modinfomanager.h"

#include "ui/CmdLaunch.h"
#include "ui/ErrorMsgBox.h"

#include "update/updateprocess.h"

#include "generate/behaviorprocess.h"

using namespace std;

void CmdGenerateInitialize(VecStr modlist)
{
    string modcode;
    wstring errmsg;
    nemesis::ModInfoManager modinfo_manager;

    try
    {
        modinfo_manager.ReadAllInfo();
    }
    catch (const std::exception& ex)
    {
        CEMsgBox* msgbox = new CEMsgBox;
        QString msg      = QString::fromStdString(ex.what());
        msgbox->setText(msg);
        msgbox->setWindowTitle("CRITITAL ERROR");
        msgbox->show();
        error = true;
        return;
    }

    string version;

    if (!isEngineUpdated(version))
    {
        wcout << TextBoxMessage(1000) + L"\n";
        return;
    }

    wcout << TextBoxMessage(1003) + L"\n";
    wcout << TextBoxMessage(1017) + L": " + nemesis::transform_to<wstring>(version) + L"\n\n";
    wcout << TextBoxMessage(1004) + L"\n";
    VecStr behaviorPriority;
    VecStr hiddenModList = getHiddenMods();
    unordered_map<string, bool> chosenBehavior;

    for (auto& mod : hiddenModList)
    {
        behaviorPriority.push_back(mod);
        chosenBehavior[mod] = true;
    }

    for (auto& each : modlist)
    {
        nemesis::to_lower(each);
        wstring mod  = nemesis::transform_to<wstring>(mod);
        auto modinfo = modinfo_manager.GetModInfo(mod);

        if (!modinfo) continue;

        behaviorPriority.insert(behaviorPriority.begin(), each);
        chosenBehavior[each] = true;
    }

    QThread* thread       = new QThread;
    BehaviorStart* worker = new BehaviorStart();
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

    cout << "End\n";
    _getch();
    exit(static_cast<int>(error));
}

void CmdUpdateInitialize()
{
    QThread* thread          = new QThread;
    UpdateFilesStart* worker = new UpdateFilesStart();
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
