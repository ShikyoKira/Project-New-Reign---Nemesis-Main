#ifndef ERRORMSGBOX_H_
#define ERRORMSGBOX_H_

#include <QMessageBox>
#include <QWidget>

class CEMsgBox : public QMessageBox
{
public:
	CEMsgBox(QWidget *parent = nullptr) { setAttribute(Qt::WA_DeleteOnClose, true); }
	~CEMsgBox()
	{
		exit(1);
	}
};

#endif