#include <QtCore/QObject>

#include "header/connector.h"

#include "header/ui/ProgressUp.h"
#include "header/ui/NemesisEngine.h"

#include "header/generate/behaviorsubprocess.h"

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


