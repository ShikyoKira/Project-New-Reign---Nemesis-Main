#include <QApplication>

#include "ui/ErrorMsgBox.h"

CEMsgBox::CEMsgBox(QWidget* parent)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	setIcon(QMessageBox::Critical);
	QApplication::beep();
}

CEMsgBox::~CEMsgBox()
{
	exit(1);
	return;
}
