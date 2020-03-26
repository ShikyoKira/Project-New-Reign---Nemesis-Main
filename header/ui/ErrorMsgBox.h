#ifndef ERRORMSGBOX_H_
#define ERRORMSGBOX_H_

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>

class CEMsgBox : public QMessageBox
{
public:
	CEMsgBox(QWidget *parent = nullptr);
	~CEMsgBox();
};

#endif