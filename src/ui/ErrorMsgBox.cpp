#include "Global.h"

#include <QApplication>

#include "ui/ErrorMsgBox.h"

CEMsgBox::CEMsgBox(QWidget* parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setIcon(QMessageBox::Critical);
    setStandardButtons(QMessageBox::Ok);
    QApplication::beep();
}

CEMsgBox::~CEMsgBox()
{
    exit(1);
    return;
}
