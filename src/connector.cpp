#include <QtCore/QString>

#include "Global.h"

#include "connector.h"

#include "core/modinfo.h"

#include "ui/Terminator.h"

#include "utilities/algorithm.h"

#pragma warning(disable : 4503)

using namespace std;

mutex processlock;
condition_variable cv;
bool processdone = false;

VecStr hiddenMods;
atomic<int> m_RunningThread;
Terminator* p_terminate = new Terminator;

bool isRunning(Terminator*& curEvent)
{
	if (m_RunningThread == 0) return false;

	curEvent = p_terminate;
	return true;
}

void DummyLog::message(std::string input)
{
	emit incomingMessage(QString::fromStdString(input));
}

void DummyLog::message(std::wstring input)
{
    emit incomingMessage(QString::fromStdWString(input));
}

VecStr getHiddenMods()
{
	return hiddenMods;
}
