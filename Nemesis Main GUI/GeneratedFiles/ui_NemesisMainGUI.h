/********************************************************************************
** Form generated from reading UI file 'NemesisMainGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEMESISMAINGUI_H
#define UI_NEMESISMAINGUI_H

#include <vector>
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
#include <QtGui/QFontDatabase.h>

#include "BehaviorListView.h"
#include "ErrorMsgBox.h"
#include "SettingsSave.h"
#include "AnimProgressBar.h"

#include "functions\wstrconvert.h"

QT_BEGIN_NAMESPACE

class Ui_NemesisMainGUIClass
{
public:
	QGridLayout* gridLayout;
	BehaviorListView* modView;
	QSpacerItem* horizontalSpacer;
	AnimProgressBar* animProgressBar;
	QTextBrowser* textBrowser;
	QProgressBar* progressBar;
	QLabel* label;
	QPushButton* buttonLaunch;
	QComboBox* comboBox;
	QPushButton* buttonCheck;
	QPushButton* buttonUpdate;
	DebugMsg* DMsg;
	DummyLog* DLog;
	std::wstring subkeys;
	QSpacerItem* horizontalSpacer_2;
	QSpacerItem* horizontalSpacer_3;
	QSpacerItem* horizontalSpacer_4;
	QSpacerItem* horizontalSpacer_5;
	QSpacerItem* horizontalSpacer_6;
	QSpacerItem* horizontalSpacer_7;
	QSpacerItem* horizontalSpacer_8;

	void setupUi(QWidget* NemesisMainGUIClass);
	void retranslateUi(QWidget* NemesisMainGUIClass);
	void reset(QWidget* NemesisMainGUIClass);
	void GetSettings();
	void backUp(QWidget* NemesisMainGUIClass);
};

namespace Ui {
	class NemesisMainGUIClass : public Ui_NemesisMainGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEMESISMAINGUI_H
