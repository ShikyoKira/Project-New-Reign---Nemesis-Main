#include "NemesisMainGUI.h"

NemesisMainGUI::NemesisMainGUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

NemesisMainGUI::~NemesisMainGUI()
{
}

void NemesisMainGUI::closeEvent(QCloseEvent* curEvent)
{
	error = true;
	terminated = true;
	Terminator* terminate_ptr;
	
	if (isRunning(terminate_ptr))
	{
		curEvent->ignore();
		connect(terminate_ptr, SIGNAL(end()), this, SLOT(close()));
	}	
	else
	{
		curEvent->accept();
	}
}
