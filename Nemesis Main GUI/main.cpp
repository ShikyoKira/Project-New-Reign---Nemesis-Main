#include "NemesisMainGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NemesisMainGUI w;

#ifndef DEBUG
	skyrimDataPath = new DataPath;

	if (error)
	{
		w.ui.buttonCheck->setDisabled(true);
		w.ui.buttonLaunch->setDisabled(true);
		w.ui.buttonUpdate->setDisabled(true);
		w.ui.modView->setDisabled(true);
		w.ui.comboBox->setDisabled(true);
	}
#endif

	w.show();
	return a.exec();
}
