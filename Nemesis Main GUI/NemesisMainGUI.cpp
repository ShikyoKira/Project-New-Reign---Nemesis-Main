#include "NemesisMainGUI.h"

NemesisMainGUI::NemesisMainGUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

NemesisMainGUI::~NemesisMainGUI()
{
	std::string language = ui.comboBox->currentText().toStdString();
	vecstr chosenBehavior;

	for (int i = 0; i < ui.modView->model()->rowCount(); ++i)
	{
		BehaviorInfo result = ui.modView->model()->data(ui.modView->model()->index(i, 0)).value<BehaviorInfo>();

		if (result.state == Qt::Checked)
		{
			chosenBehavior.push_back(result.modname.toStdString());
		}
	}

	createCache(language, chosenBehavior);
}
