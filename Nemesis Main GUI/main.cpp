#include "NemesisMainGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	NemesisMainGUI w;
	w.show();
	return a.exec();
}
