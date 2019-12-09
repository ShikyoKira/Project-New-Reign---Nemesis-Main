#include "ui_NemesisMainGUI.h"

void Ui_NemesisMainGUIClass::setupUi(QWidget* NemesisMainGUIClass)
{
	if (NemesisMainGUIClass->objectName().isEmpty())
		NemesisMainGUIClass->setObjectName(QStringLiteral("NemesisMainGUIClass"));
	NemesisMainGUIClass->resize(528, 782);
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(NemesisMainGUIClass->sizePolicy().hasHeightForWidth());
	NemesisMainGUIClass->setSizePolicy(sizePolicy);
	NemesisMainGUIClass->setMinimumSize(QSize(528, 559));
	NemesisMainGUIClass->setMaximumSize(QSize(700, 950));

	gridLayout = new QGridLayout(NemesisMainGUIClass);
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(11, 11, 11, 11);
	gridLayout->setObjectName(QStringLiteral("gridLayout"));
	gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

	buttonCheck = new QPushButton(NemesisMainGUIClass);
	buttonCheck->setObjectName(QStringLiteral("buttonCheck"));
	QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
	sizePolicy1.setHorizontalStretch(0);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(buttonCheck->sizePolicy().hasHeightForWidth());
	buttonCheck->setSizePolicy(sizePolicy1);
	buttonCheck->setDisabled(true);
	buttonCheck->setMinimumSize(QSize(250, 35));
	buttonCheck->setMaximumSize(QSize(500, 16777215));
	QFont font;
	font.setFamily(QStringLiteral("Cambria"));
	font.setPointSize(12);
	buttonCheck->setFont(font);
	buttonCheck->setCursor(QCursor(Qt::PointingHandCursor));

	gridLayout->addWidget(buttonCheck, 10, 0, 1, 5);

	textBrowser = new QTextBrowser(NemesisMainGUIClass);
	textBrowser->setObjectName(QStringLiteral("textBrowser"));
	QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
	textBrowser->setSizePolicy(sizePolicy2);
	textBrowser->setMaximumSize(QSize(16777215, 1000));
	textBrowser->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

	gridLayout->addWidget(textBrowser, 15, 0, 1, 10);

	horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer, 7, 0, 1, 1);

	animProgressBar = new AnimProgressBar(NemesisMainGUIClass);
	animProgressBar->setObjectName(QStringLiteral("animProgressBar"));
	animProgressBar->setMaximum(10000);
	animProgressBar->setValue(0);
	animProgressBar->setAlignment(Qt::AlignCenter);

	gridLayout->addWidget(animProgressBar, 6, 0, 1, 10);

	progressBar = new QProgressBar(NemesisMainGUIClass);
	progressBar->setObjectName(QStringLiteral("progressBar"));
	progressBar->setValue(0);
	progressBar->setAlignment(Qt::AlignCenter);

	gridLayout->addWidget(progressBar, 16, 0, 1, 10);

	buttonUpdate = new QPushButton(NemesisMainGUIClass);
	buttonUpdate->setObjectName(QStringLiteral("buttonUpdate"));
	sizePolicy1.setHeightForWidth(buttonUpdate->sizePolicy().hasHeightForWidth());
	buttonUpdate->setSizePolicy(sizePolicy1);
	buttonUpdate->setMinimumSize(QSize(250, 35));
	buttonUpdate->setMaximumSize(QSize(500, 16777215));
	buttonUpdate->setFont(font);
	buttonUpdate->setCursor(QCursor(Qt::PointingHandCursor));

	gridLayout->addWidget(buttonUpdate, 10, 5, 1, 5);

	buttonLaunch = new QPushButton(NemesisMainGUIClass);
	buttonLaunch->setObjectName(QStringLiteral("buttonLaunch"));
	sizePolicy1.setHeightForWidth(buttonLaunch->sizePolicy().hasHeightForWidth());
	buttonLaunch->setSizePolicy(sizePolicy1);
	buttonLaunch->setMinimumSize(QSize(200, 45));
	QFont font1;
	font1.setFamily(QStringLiteral("Cambria"));
	font1.setPointSize(12);
	font1.setBold(true);
	font1.setWeight(75);
	buttonLaunch->setFont(font1);
	buttonLaunch->setCursor(QCursor(Qt::PointingHandCursor));

	gridLayout->addWidget(buttonLaunch, 7, 1, 1, 8);

	horizontalSpacer_2 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer_2, 0, 0, 2, 1);

	horizontalSpacer_3 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer_3, 7, 9, 1, 1);

	horizontalSpacer_4 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer_4, 0, 9, 1, 1);

	horizontalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer_5, 0, 8, 1, 1);

	horizontalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer_6, 0, 1, 2, 1);

	horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer_7, 0, 2, 2, 1);

	horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer_8, 0, 7, 2, 1);

	comboBox = new QComboBox(NemesisMainGUIClass);
	comboBox->setObjectName(QStringLiteral("comboBox"));
	QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
	sizePolicy3.setHorizontalStretch(0);
	sizePolicy3.setVerticalStretch(0);
	sizePolicy3.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
	comboBox->setSizePolicy(sizePolicy3);
	comboBox->setMinimumSize(QSize(0, 0));
	comboBox->setMaximumSize(QSize(500, 16777215));
	comboBox->setContextMenuPolicy(Qt::ActionsContextMenu);
	comboBox->setLayoutDirection(Qt::LeftToRight);

	gridLayout->addWidget(comboBox, 1, 8, 1, 2);

	label = new QLabel(NemesisMainGUIClass);
	label->setObjectName(QStringLiteral("label"));
	QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy4.setHorizontalStretch(0);
	sizePolicy4.setVerticalStretch(0);
	sizePolicy4.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
	label->setSizePolicy(sizePolicy4);
	label->setMinimumSize(QSize(0, 40));
	label->setMaximumSize(QSize(206, 55));
	label->setAlignment(Qt::AlignCenter);
	label->setScaledContents(true);

	gridLayout->addWidget(label, 0, 3, 2, 4);

	modView = new BehaviorListView(NemesisMainGUIClass);

	if (error) return;

	modView->setObjectName(QStringLiteral("modView"));
	QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Fixed);
	sizePolicy5.setHorizontalStretch(0);
	sizePolicy5.setVerticalStretch(0);
	sizePolicy5.setHeightForWidth(modView->sizePolicy().hasHeightForWidth());
	modView->setSizePolicy(sizePolicy5);
	modView->setMinimumSize(QSize(0, 265));
	modView->setMaximumSize(QSize(16777215, 300));
	modView->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
	modView->setDragEnabled(true);
	modView->setAcceptDrops(true);
	modView->setDragDropMode(QAbstractItemView::InternalMove);
	modView->setDropIndicatorShown(true);
	modView->setDefaultDropAction(Qt::MoveAction);
	modView->setAlternatingRowColors(true);
	modView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	modView->setSortingEnabled(true);
	modView->setExpandsOnDoubleClick(false);

	backUp(NemesisMainGUIClass);
	GetSettings();
	reset(NemesisMainGUIClass);

	bool exception = false;
	nemesisInfo = new NemesisInfo(exception);

	if (exception)
	{
		buttonCheck->setDisabled(true);
		buttonLaunch->setDisabled(true);
		buttonUpdate->setDisabled(true);
		comboBox->setDisabled(true);
		modView->setDisabled(true);
		error = false;
	}

	NemesisMainGUIClass->resize(nemesisInfo->GetWidth(), nemesisInfo->GetHeight());

	modView->header()->setStyleSheet("background: transparent");
	modView->header()->setDefaultAlignment(Qt::AlignCenter);
	modView->header()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);

	modView->setColumnWidth(0, nemesisInfo->GetModNameWidth());
	modView->setColumnWidth(1, nemesisInfo->GetAuthorWidth());
	modView->setColumnWidth(2, nemesisInfo->GetPriorityWidth());

	QObject::connect(modView, SIGNAL(dropModelUpdate(bool)), modView->model(), SLOT(dropModelUpdate(bool)));

	gridLayout->addWidget(modView, 5, 0, 1, 10);

	retranslateUi(NemesisMainGUIClass);

	QMetaObject::connectSlotsByName(NemesisMainGUIClass);
} // setupUi

void Ui_NemesisMainGUIClass::retranslateUi(QWidget* NemesisMainGUIClass)
{
	QObject::connect(buttonLaunch, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleLaunch()));
	QObject::connect(buttonUpdate, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleUpdate()));
	QObject::connect(buttonCheck, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleCheck()));
	QObject::connect(comboBox, SIGNAL(currentIndexChanged(QString)), NemesisMainGUIClass, SLOT(languageChange(QString)));

	QModelIndexList* indexlist = new QModelIndexList();

	for (int j = 0; j < modView->model()->columnCount(); ++j)
	{
		for (int i = 0; i < modView->model()->rowCount(); ++i)
		{
			indexlist->push_back(modView->model()->index(i, j));
		}
	}

	buttonLaunch->setDisabled(nemesisInfo->IsFirst());

	animProgressBar->setMaximum(MAX_ANIM);
	animProgressBar->setValue(0);
}

void Ui_NemesisMainGUIClass::reset(QWidget* NemesisMainGUIClass)
{
	NemesisMainGUIClass->setWindowTitle(QApplication::translate("NemesisMainGUIClass", UIMessage(1000).c_str(), nullptr));
	label->setPixmap(QPixmap(":/background/Resources/title header 2.jpg"));
	animProgressBar->setToolTip(QApplication::translate("NemesisMainGUIClass", UIMessage(1004).c_str(), nullptr));
	buttonUpdate->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1003).c_str(), nullptr));
	buttonCheck->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1002).c_str(), nullptr));
	buttonLaunch->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1001).c_str(), nullptr));
}

void Ui_NemesisMainGUIClass::GetSettings()
{
	std::string language;
	std::unordered_map<std::string, bool> chosenBehavior;

	if (!isFileExist("languages"))
	{
		CEMsgBox* msg = new CEMsgBox;
		msg->setWindowTitle("ERROR");
		msg->setText("Error: \"languages\" folder not found. Please reinstall Nemesis");
		msg->show();
	}
	else if (!isFileExist("languages\\english.txt"))
	{
		CEMsgBox* msg = new CEMsgBox;
		msg->setWindowTitle("ERROR");
		msg->setText("Error: \"english.txt\" file not found in language folder. Please reinstall Nemesis");
		msg->show();
	}

	vecstr languagelist;
	int curindex = -1;
	bool cacheResult;

	read_directory("languages", languagelist);

	for (unsigned int i = 0; i < languagelist.size(); ++i)
	{
		comboBox->addItem(QString());
	}

	try
	{
		cacheResult = getCache(language, chosenBehavior);
	}
	catch (nemesis::exception&)
	{
		cacheResult = false;
	}

	if (cacheResult)
	{
		int english = 0;

		for (unsigned int i = 0; i < languagelist.size(); ++i)
		{
			std::string curLang = GetFileName(languagelist[i]);
			comboBox->setItemText(i, QApplication::translate("NemesisMainGUIClass", curLang.c_str(), nullptr));

			if (curLang == language)
			{
				curindex = i;
				DMsg = new DebugMsg(language);
			}
			else if (language == "english")
			{
				english = i;
			}
		}

		if (curindex < 0)
		{
			DMsg = new DebugMsg("english");
			interMsg("Previous language pack is not found. Default language is being used instead");
			comboBox->setCurrentIndex(english);
		}
		else
		{
			comboBox->setCurrentIndex(curindex);
		}

		// check the check state of mods in previous runtime
		for (int i = 0; i < modView->model()->rowCount(); ++i)
		{
			QAbstractItemModel* model = modView->model();
			std::string mod = (model->data(model->index(i, 0), Qt::DisplayRole)).toString().toStdString();

			if (chosenBehavior.find(mod) != chosenBehavior.end())
			{
				if (chosenBehavior[mod])
				{
					model->setData(model->index(i, 0), Qt::Checked, Qt::CheckStateRole);
				}
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < languagelist.size(); ++i)
		{
			std::string curLang = GetFileName(languagelist[i]);
			comboBox->setItemText(i, QApplication::translate("NemesisMainGUIClass", curLang.c_str(), nullptr));

			if (curLang == language)
			{
				curindex = i;
			}
		}

		DMsg = new DebugMsg(language);
		comboBox->setCurrentIndex(curindex);
	}

	NewDebugMessage(*DMsg);
}

void Ui_NemesisMainGUIClass::backUp(QWidget* NemesisMainGUIClass)
{
	DLog = new DummyLog;
	connectProcess(DLog);
	QObject::connect(DLog, SIGNAL(incomingMessage(QString)), NemesisMainGUIClass, SLOT(sendMessage(QString)));
}

