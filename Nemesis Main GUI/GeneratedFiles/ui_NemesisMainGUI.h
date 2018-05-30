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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include "behaviorlistview.h"
#include "ErrorMsgBox.h"
#include "SettingsSave.h"

QT_BEGIN_NAMESPACE

class Ui_NemesisMainGUIClass
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_3;
    QComboBox *comboBox;
    BehaviorListView *modView;
    QSpacerItem *horizontalSpacer;
    QProgressBar *progressBar;
    QLabel *label;
    QCheckBox *checkBox;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonUpdate;
    QPushButton *buttonCheck;
    QPushButton *buttonLaunch;
    QTextBrowser *textBrowser;

    void setupUi(QWidget *NemesisMainGUIClass)
    {
        if (NemesisMainGUIClass->objectName().isEmpty())
            NemesisMainGUIClass->setObjectName(QStringLiteral("NemesisMainGUIClass"));
        NemesisMainGUIClass->resize(526, 740);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NemesisMainGUIClass->sizePolicy().hasHeightForWidth());
        NemesisMainGUIClass->setSizePolicy(sizePolicy);
        NemesisMainGUIClass->setMinimumSize(QSize(526, 532));
        NemesisMainGUIClass->setMaximumSize(QSize(526, 800));
        gridLayout = new QGridLayout(NemesisMainGUIClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout->addItem(verticalSpacer, 0, 7, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 3, 6, 1, 2);

        comboBox = new QComboBox(NemesisMainGUIClass);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setContextMenuPolicy(Qt::ActionsContextMenu);

        gridLayout->addWidget(comboBox, 1, 5, 1, 3);

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

        gridLayout->addWidget(modView, 2, 0, 1, 8);

        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 1);

        progressBar = new QProgressBar(NemesisMainGUIClass);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);
        progressBar->setHidden(true);

        gridLayout->addWidget(progressBar, 10, 0, 1, 8);

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

        gridLayout->addWidget(label, 0, 0, 2, 4);

        checkBox = new QCheckBox(NemesisMainGUIClass);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy3);
        checkBox->setMinimumSize(QSize(0, 0));
        checkBox->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(checkBox, 4, 5, 1, 3);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 4, 4, 1, 1);

        buttonUpdate = new QPushButton(NemesisMainGUIClass);
        buttonUpdate->setObjectName(QStringLiteral("buttonUpdate"));
        sizePolicy1.setHeightForWidth(buttonUpdate->sizePolicy().hasHeightForWidth());
        buttonUpdate->setSizePolicy(sizePolicy1);
        buttonUpdate->setMinimumSize(QSize(165, 35));
        buttonUpdate->setMaximumSize(QSize(1234, 16777215));
        QFont font1;
        font1.setFamily(QStringLiteral("Cambria"));
        font1.setPointSize(12);
        buttonUpdate->setFont(font1);

        gridLayout->addWidget(buttonUpdate, 4, 3, 1, 1);

        buttonCheck = new QPushButton(NemesisMainGUIClass);
        buttonCheck->setObjectName(QStringLiteral("buttonCheck"));
        sizePolicy1.setHeightForWidth(buttonCheck->sizePolicy().hasHeightForWidth());
        buttonCheck->setSizePolicy(sizePolicy1);
        buttonCheck->setMinimumSize(QSize(182, 35));
        buttonCheck->setMaximumSize(QSize(1234, 16777215));
        buttonCheck->setFont(font1);
        buttonCheck->setDisabled(true);

        gridLayout->addWidget(buttonCheck, 4, 0, 1, 3);

        buttonLaunch = new QPushButton(NemesisMainGUIClass);
        buttonLaunch->setObjectName(QStringLiteral("buttonLaunch"));
        sizePolicy1.setHeightForWidth(buttonLaunch->sizePolicy().hasHeightForWidth());
        buttonLaunch->setSizePolicy(sizePolicy1);
        buttonLaunch->setMinimumSize(QSize(200, 45));
        QFont font2;
        font2.setFamily(QStringLiteral("Cambria"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        buttonLaunch->setFont(font2);

        gridLayout->addWidget(buttonLaunch, 3, 1, 1, 5);

        textBrowser = new QTextBrowser(NemesisMainGUIClass);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        sizePolicy.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy);
        textBrowser->setMaximumSize(QSize(16777215, 1000));
        textBrowser->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);

        gridLayout->addWidget(textBrowser, 9, 0, 1, 8);


        retranslateUi(NemesisMainGUIClass);

        QMetaObject::connectSlotsByName(NemesisMainGUIClass);
    } // setupUi

    void retranslateUi(QWidget *NemesisMainGUIClass)
    {
		NemesisMainGUIClass->setWindowTitle(QApplication::translate("NemesisMainGUIClass", "Nemesis Ultimate Behavior Engine", nullptr));

		GetSettings();

		NemesisMainGUIClass->setWindowTitle(QApplication::translate("NemesisMainGUIClass", UIMessage(1000).c_str(), nullptr));
		label->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1000).c_str(), nullptr));
		checkBox->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1004).c_str(), nullptr));
		buttonUpdate->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1003).c_str(), nullptr));
		buttonCheck->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1002).c_str(), nullptr));
		buttonLaunch->setText(QApplication::translate("NemesisMainGUIClass", UIMessage(1001).c_str(), nullptr));

		QObject::connect(buttonLaunch, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleButton1()));
		QObject::connect(buttonUpdate, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleButton2()));
		QObject::connect(buttonCheck, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleButton3()));

		modView->setColumnWidth(0, 380);
		modView->setColumnWidth(1, 75);
		modView->setColumnWidth(2, 47);
		modView->header()->setDefaultAlignment(Qt::AlignCenter);
		modView->header()->setSectionResizeMode(QHeaderView::Fixed);
	} // retranslateUi

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
					NewDebugMessage(language);
				}
				else if (language == "english")
				{
					english = i;
				}
			}

			if (curindex == -1)
			{
				NewDebugMessage("english");
				interMsg("Previous language pack is not found. Default language is being used instead");
				comboBox->setCurrentIndex(english);
			}
			else
			{
				comboBox->setCurrentIndex(curindex);
			}

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

			NewDebugMessage(language);
			comboBox->setCurrentIndex(curindex);
		}
	}
};

namespace Ui {
    class NemesisMainGUIClass: public Ui_NemesisMainGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEMESISMAINGUI_H
