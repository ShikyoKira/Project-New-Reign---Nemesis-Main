#include "progressup.h"
#include "NemesisMainGUI.h"
#include "master.h"
#include <QtCore\QObject.h>

void ProgressUp::newWidget(NemesisMainGUI* _widget)
{
	widget = _widget;
}

void ProgressUp::newProcess(BehaviorSub* _process)
{
	process = _process;
}

void ProgressUp::connect()
{
	QObject::connect(process, SIGNAL(progressAdd()), widget, SLOT(setProgressBarValue()));
}


