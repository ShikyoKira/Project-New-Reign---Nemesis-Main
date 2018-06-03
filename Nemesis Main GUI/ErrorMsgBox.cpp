#include "ErrorMsgBox.h"

CEMsgBox::CEMsgBox(QWidget* parent)
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	setIcon(QMessageBox::Critical);
}

CEMsgBox::~CEMsgBox()
{
	exit(1);
}
