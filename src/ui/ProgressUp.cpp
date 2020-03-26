#include <QtCore/QObject>

#include "connector.h"

#include "ui/ProgressUp.h"
#include "ui/NemesisEngine.h"

#include "generate/behaviorsubprocess.h"

void ProgressUp::newWidget(NemesisEngine* _widget)
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


