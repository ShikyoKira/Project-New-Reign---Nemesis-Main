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
		QAbstractItemModel* model = ui.modView->model();
		QVariant state = model->data(model->index(i, 0), Qt::CheckStateRole);

		if (state == Qt::Checked)
		{
			QString modname = model->data(model->index(i, 0), Qt::DisplayRole).toString();
			chosenBehavior.push_back(modname.toStdString());
		}
	}

	createCache(language, chosenBehavior);
}
