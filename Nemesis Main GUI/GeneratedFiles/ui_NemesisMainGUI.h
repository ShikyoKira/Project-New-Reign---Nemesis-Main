/********************************************************************************
** Form generated from reading UI file 'NemesisMainGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEMESISMAINGUI_H
#define UI_NEMESISMAINGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include "BehaviorListView.h"
#include "ErrorMsgBox.h"
#include "SettingsSave.h"
#include "BehaviorListRowColor.h"
#include "AnimProgressBar.h"

QT_BEGIN_NAMESPACE

class Ui_NemesisMainGUIClass
{
public:
    QGridLayout *gridLayout;
    BehaviorListView *modView;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QProgressBar *animProgressBar;
    QTextBrowser *textBrowser;
    QProgressBar *progressBar;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonLaunch;
    QComboBox *comboBox;
    QPushButton *buttonCheck;
    QPushButton *buttonUpdate;
	DebugMsg* DMsg;
	DummyLog* DLog;

    void setupUi(QWidget *NemesisMainGUIClass)
    {
        if (NemesisMainGUIClass->objectName().isEmpty())
            NemesisMainGUIClass->setObjectName(QStringLiteral("NemesisMainGUIClass"));
        NemesisMainGUIClass->resize(526, 686);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NemesisMainGUIClass->sizePolicy().hasHeightForWidth());
        NemesisMainGUIClass->setSizePolicy(sizePolicy);
        NemesisMainGUIClass->setMinimumSize(QSize(526, 559));
        NemesisMainGUIClass->setMaximumSize(QSize(571, 800));
        gridLayout = new QGridLayout(NemesisMainGUIClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

        modView = new BehaviorListView(NemesisMainGUIClass);
        modView->setObjectName(QStringLiteral("modView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(modView->sizePolicy().hasHeightForWidth());
        modView->setSizePolicy(sizePolicy1);
        modView->setMinimumSize(QSize(0, 265));
        modView->setMaximumSize(QSize(16777215, 300));
        modView->setEditTriggers(QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        modView->setDragEnabled(true);
        modView->setDragDropMode(QAbstractItemView::DragDrop);
        modView->setDefaultDropAction(Qt::MoveAction);
        modView->setAlternatingRowColors(true);
        modView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        modView->setSortingEnabled(true);
        modView->setExpandsOnDoubleClick(false);
        modView->header()->setDefaultSectionSize(20);
        modView->header()->setStretchLastSection(false);

        gridLayout->addWidget(modView, 2, 0, 1, 9);

        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 4, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 0, 8, 1, 1);

        animProgressBar = new AnimProgressBar(NemesisMainGUIClass);
        animProgressBar->setObjectName(QStringLiteral("animProgressBar"));
        animProgressBar->setMaximum(10000);
        animProgressBar->setValue(0);
        animProgressBar->setAlignment(Qt::AlignCenter);
		animProgressBar->setTextVisible(false);

        gridLayout->addWidget(animProgressBar, 3, 0, 1, 9);

        textBrowser = new QTextBrowser(NemesisMainGUIClass);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        sizePolicy.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy);
        textBrowser->setMaximumSize(QSize(16777215, 1000));
        textBrowser->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        gridLayout->addWidget(textBrowser, 10, 0, 1, 9);

        progressBar = new QProgressBar(NemesisMainGUIClass);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);
		progressBar->setHidden(true);

        gridLayout->addWidget(progressBar, 11, 0, 1, 9);

        label = new QLabel(NemesisMainGUIClass);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setMinimumSize(QSize(0, 40));
        QFont font;
        font.setFamily(QStringLiteral("Cambria"));
        font.setPointSize(16);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setTextFormat(Qt::RichText);

        gridLayout->addWidget(label, 0, 0, 2, 6);

        horizontalSpacer_3 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 4, 8, 1, 1);

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

        gridLayout->addWidget(buttonLaunch, 4, 1, 1, 7);

        comboBox = new QComboBox(NemesisMainGUIClass);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setContextMenuPolicy(Qt::ActionsContextMenu);

        gridLayout->addWidget(comboBox, 1, 7, 1, 2);

        buttonCheck = new QPushButton(NemesisMainGUIClass);
        buttonCheck->setObjectName(QStringLiteral("buttonCheck"));
        sizePolicy1.setHeightForWidth(buttonCheck->sizePolicy().hasHeightForWidth());
        buttonCheck->setSizePolicy(sizePolicy1);
        buttonCheck->setMinimumSize(QSize(251, 35));
        buttonCheck->setMaximumSize(QSize(1234, 16777215));
        QFont font2;
        font2.setFamily(QStringLiteral("Cambria"));
        font2.setPointSize(12);
        buttonCheck->setFont(font2);
        buttonCheck->setCursor(QCursor(Qt::PointingHandCursor));
		buttonCheck->setDisabled(true);

        gridLayout->addWidget(buttonCheck, 5, 0, 1, 4);

        buttonUpdate = new QPushButton(NemesisMainGUIClass);
        buttonUpdate->setObjectName(QStringLiteral("buttonUpdate"));
        sizePolicy1.setHeightForWidth(buttonUpdate->sizePolicy().hasHeightForWidth());
        buttonUpdate->setSizePolicy(sizePolicy1);
        buttonUpdate->setMinimumSize(QSize(165, 35));
        buttonUpdate->setMaximumSize(QSize(1234, 16777215));
        buttonUpdate->setFont(font2);
        buttonUpdate->setCursor(QCursor(Qt::PointingHandCursor));

        gridLayout->addWidget(buttonUpdate, 5, 4, 1, 5);


        retranslateUi(NemesisMainGUIClass);

        QMetaObject::connectSlotsByName(NemesisMainGUIClass);
    } // setupUi

	void retranslateUi(QWidget *NemesisMainGUIClass)
	{
		NemesisMainGUIClass->setWindowTitle(QApplication::translate("NemesisMainGUIClass", "Nemesis Ultimate Behavior Engine", nullptr));

		GetSettings();
		reset(NemesisMainGUIClass);
		backUp(NemesisMainGUIClass);

		QObject::connect(buttonLaunch, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleButton1()));
		QObject::connect(buttonUpdate, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleButton2()));
		QObject::connect(buttonCheck, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleButton3()));
		QObject::connect(comboBox, SIGNAL(currentIndexChange(QString)), NemesisMainGUIClass, SLOT(languageChange(QString)));

		QModelIndexList* indexlist = new QModelIndexList();

		for (int j = 0; j < modView->model()->columnCount(); ++j)
		{
			for (int i = 0; i < modView->model()->rowCount(); ++i)
			{
				indexlist->push_back(modView->model()->index(i, j));
			}
		}

		modView->setItemDelegate(new BehaviorListRowDelegate(modView->model(), indexlist));
		modView->setColumnWidth(0, 380);
		modView->setColumnWidth(1, 75);
		modView->setColumnWidth(2, 47);
		modView->header()->setDefaultAlignment(Qt::AlignCenter);
		modView->header()->setSectionResizeMode(QHeaderView::Fixed);
	} // retranslateUi

	void reset(QWidget* NemesisMainGUIClass)
	{
		NemesisMainGUIClass->setWindowTitle(QApplication::translate("NemesisMainGUIClass", UIMessage(1000).c_str(), nullptr));
		label->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1000).c_str(), nullptr));
		animProgressBar->setToolTip(QApplication::translate("NemesisMainGUIClass", UIMessage(1004).c_str(), nullptr));
		buttonUpdate->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1003).c_str(), nullptr));
		buttonCheck->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1002).c_str(), nullptr));
		buttonLaunch->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1001).c_str(), nullptr));
	}

	void GetSettings()
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

		read_directory("languages", languagelist);

		for (unsigned int i = 0; i < languagelist.size(); ++i)
		{
			comboBox->addItem(QString());
		}

		if (getCache(language, chosenBehavior))
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
			language = "english";

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

	void backUp(QWidget* NemesisMainGUIClass)
	{
		DLog = new DummyLog;
		connectProcess(DLog);

		QObject::connect(DLog, SIGNAL(incomingMessage(QString)), NemesisMainGUIClass, SLOT(sendMessage(QString)));
	}
};

namespace Ui {
    class NemesisMainGUIClass: public Ui_NemesisMainGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEMESISMAINGUI_H
