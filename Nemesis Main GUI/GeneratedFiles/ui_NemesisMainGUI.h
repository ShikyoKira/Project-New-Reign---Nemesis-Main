/********************************************************************************
** Form generated from reading UI file 'NemesisMainGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.11.0
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
#include "src\utilities\wstrconvert.h"

QT_BEGIN_NAMESPACE

class Ui_NemesisMainGUIClass
{
private:
	bool deleteKey(LONG nError, HKEY hKey, LPCWSTR achKey, LPCWSTR& keyName)
	{
		if (nError == ERROR_SUCCESS)
		{
			HKEY innerKey;
			nError = RegOpenKeyEx(hKey, achKey, NULL, KEY_ALL_ACCESS, &innerKey);

			if (nError == ERROR_SUCCESS)
			{
				TCHAR buffer[16383];
				DWORD size = sizeof(buffer), type = REG_SZ;
				int counter = 0;
				std::wstring filename;
				nError = RegQueryValueEx(innerKey, TEXT("directory"), NULL, &type, LPBYTE(buffer), &size);

				if (nError == ERROR_SUCCESS)
				{
					filename = buffer;

					if (!boost::filesystem::exists(filename + L"\\mod"))
					{
						return true;
					}
					else
					{
						LPWSTR curpath = new WCHAR[WCHAR_MAX];
						GetModuleFileNameW(NULL, curpath, WCHAR_MAX);
						std::wstring wstrdata = boost::filesystem::path(curpath).parent_path().wstring();
						curpath = const_cast<LPWSTR>(wstrdata.c_str());

						if (filename == curpath)
						{
							keyName = achKey;
							subkeys = std::to_wstring(CRC32Convert(wstrConv.to_bytes(curpath).c_str()) % 99999999);
						}
					}
				}
				else return true;
			}
			else return true;
		}
		else return true;

		return false;
	}

	void checkKey(HKEY hKey, LPCWSTR& keyName)
	{
		LONG nError;
		DWORD    cbName;                   // size of name string 
		TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
		DWORD    cchClassName = MAX_PATH;  // size of class string 
		DWORD    cSubKeys = 0;               // number of subkeys 
		DWORD    cbMaxSubKey;              // longest subkey size 
		DWORD    cchMaxClass;              // longest class string 
		DWORD    cValues;              // number of values for key 
		DWORD    cchMaxValue;          // longest value name 
		DWORD    cbMaxValueData;       // longest value data 
		DWORD    cbSecurityDescriptor; // size of security descriptor 
		FILETIME ftLastWriteTime;      // last write time 

		DWORD i;
		std::vector<std::wstring> deletingValues;

		// Get the class name and the value count. 
		nError = RegQueryInfoKey(
			hKey,                    // key handle 
			achClass,                // buffer for class name 
			&cchClassName,           // size of class string 
			NULL,                    // reserved 
			&cSubKeys,               // number of subkeys 
			&cbMaxSubKey,            // longest subkey size 
			&cchMaxClass,            // longest class string 
			&cValues,                // number of values for this key 
			&cchMaxValue,            // longest value name 
			&cbMaxValueData,         // longest value data 
			&cbSecurityDescriptor,   // security descriptor 
			&ftLastWriteTime);       // last write time 

									 // Enumerate the subkeys, until RegEnumKeyEx fails.

									 // Enumerate the key values. 
		if (cSubKeys)
		{
			for (i = 0; i < cSubKeys; i++)
			{
				TCHAR    achKey[255];   // buffer for subkey name
				cbName = 255;
				nError = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);

				if (deleteKey(nError, hKey, achKey, keyName)) deletingValues.push_back(achKey);
			}
		}

		for (auto& each : deletingValues)
		{
			nError = RegDeleteKeyEx(hKey, each.c_str(), KEY_ALL_ACCESS, 0);

			if (nError != ERROR_SUCCESS) DebugLogging("Failed to delete registry key");
		}

		if (keyName == NULL)
		{
			LPSTR curpath = new CHAR[CHAR_MAX];
			GetModuleFileNameA(NULL, curpath, CHAR_MAX);
			subkeys = std::to_wstring(CRC32Convert(curpath) % 99999999);
		}
	}

public:
	QGridLayout *gridLayout;
	BehaviorListView *modView;
	QSpacerItem *horizontalSpacer;
	AnimProgressBar *animProgressBar;
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
	std::wstring subkeys;

	void setupUi(QWidget *NemesisMainGUIClass)
	{
		if (NemesisMainGUIClass->objectName().isEmpty())
			NemesisMainGUIClass->setObjectName(QStringLiteral("NemesisMainGUIClass"));
		NemesisMainGUIClass->resize(526, 750);
		QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);
		sizePolicy.setHeightForWidth(NemesisMainGUIClass->sizePolicy().hasHeightForWidth());
		NemesisMainGUIClass->setSizePolicy(sizePolicy);
		NemesisMainGUIClass->setMinimumSize(QSize(526, 559));
		NemesisMainGUIClass->setMaximumSize(QSize(526, 950));
		gridLayout = new QGridLayout(NemesisMainGUIClass);
		gridLayout->setSpacing(6);
		gridLayout->setContentsMargins(11, 11, 11, 11);
		gridLayout->setObjectName(QStringLiteral("gridLayout"));
		gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

		modView = new BehaviorListView(NemesisMainGUIClass);

		if (error)
		{
			return;
		}

		modView->setObjectName(QStringLiteral("modView"));
		QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
		sizePolicy1.setHorizontalStretch(0);
		sizePolicy1.setVerticalStretch(0);
		sizePolicy1.setHeightForWidth(modView->sizePolicy().hasHeightForWidth());
		modView->setSizePolicy(sizePolicy1);
		modView->setMinimumSize(QSize(0, 265));
		modView->setMaximumSize(QSize(16777215, 300));
		modView->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
		modView->setDragEnabled(true);
		modView->setAcceptDrops(true);
		modView->setDropIndicatorShown(false);
		modView->setDefaultDropAction(Qt::MoveAction);
		modView->setAlternatingRowColors(true);
		modView->setSelectionMode(QAbstractItemView::ExtendedSelection);
		modView->setSortingEnabled(true);
		modView->setExpandsOnDoubleClick(false);
		modView->header()->setDefaultSectionSize(20);
		modView->header()->setStretchLastSection(false);

		QObject::connect(modView, SIGNAL(dropModelUpdate(bool)), modView->model(), SLOT(dropModelUpdate(bool)));

		gridLayout->addWidget(modView, 2, 0, 1, 9);

		horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

		gridLayout->addItem(horizontalSpacer, 4, 0, 1, 1);

		animProgressBar = new AnimProgressBar(NemesisMainGUIClass);
		animProgressBar->setObjectName(QStringLiteral("animProgressBar"));
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
		label->setMinimumSize(QSize(235, 55));
		label->setMaximumSize(QSize(235, 55));
		label->setScaledContents(true);

		gridLayout->addWidget(label, 0, 2, 2, 4);

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

		{
			HKEY hKey;
			DWORD dwDisposition;
			LONG nError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Nemesis"), NULL, KEY_ALL_ACCESS, &hKey);

			if (nError == ERROR_SUCCESS)
			{
				LPCWSTR keyName = NULL;
				checkKey(hKey, keyName);
				LPCWSTR s_subkey = subkeys.c_str();

				if (keyName != NULL)
				{
					HKEY innerKey;
					nError = RegOpenKeyEx(hKey, s_subkey, NULL, KEY_ALL_ACCESS, &innerKey);

					if (nError == ERROR_SUCCESS)
					{
						DWORD data, size = sizeof(data), type = REG_DWORD;
						nError = RegQueryValueEx(innerKey, TEXT("first"), NULL, &type, (LPBYTE)&data, &size);

						if (nError == ERROR_SUCCESS)
						{
							if (data == 1)
							{
								buttonLaunch->setDisabled(true);
							}
						}
						else  DebugLogging("Failed to update registry key value");
					}
					else  DebugLogging("Failed to open registry key");

					RegCloseKey(innerKey);
				}
				else
				{
					buttonLaunch->setDisabled(true);
					HKEY innerKey;
					DWORD data = 1;
					nError = RegSetKeyValue(hKey, s_subkey, TEXT("first"), REG_DWORD, &data, sizeof(data));

					if (nError != ERROR_SUCCESS) DebugLogging("Failed to create registry key value");

					nError = RegOpenKeyEx(hKey, s_subkey, NULL, KEY_ALL_ACCESS, &innerKey);

					if (nError == ERROR_SUCCESS)
					{
						LPWSTR strdata = new WCHAR[WCHAR_MAX];
						GetModuleFileNameW(NULL, strdata, WCHAR_MAX);
						std::wstring wstrdata = boost::filesystem::path(strdata).parent_path().wstring();
						strdata = const_cast<LPWSTR>(wstrdata.c_str());
						nError = RegSetValueEx(innerKey, TEXT("directory"), 0, REG_SZ, LPCBYTE(strdata), wcslen(strdata) * sizeof(TCHAR));

						if (nError != ERROR_SUCCESS) DebugLogging("Failed to create registry key value");
					}
					else  DebugLogging("Failed to open registry key");

					RegCloseKey(innerKey);
				}
			}
			else
			{
				buttonLaunch->setDisabled(true);
				LPSTR curpath = new CHAR[CHAR_MAX];
				GetModuleFileNameA(NULL, curpath, CHAR_MAX);
				std::wstring tempkey = std::to_wstring(CRC32Convert(curpath) % 99999999);
				nError = RegCreateKeyEx(HKEY_LOCAL_MACHINE, (L"SOFTWARE\\Nemesis\\" + tempkey).c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
					&hKey, NULL);

				if (nError == ERROR_SUCCESS)
				{
					DWORD data = 1, size = sizeof(data), type = REG_DWORD;
					nError = RegSetKeyValue(hKey, L"", TEXT("first"), type, &data, size);

					if (nError != ERROR_SUCCESS) DebugLogging("Failed to create registry key value");

					LPWSTR strdata = new WCHAR[WCHAR_MAX];
					GetModuleFileNameW(NULL, strdata, WCHAR_MAX);
					std::wstring wstrdata = boost::filesystem::path(strdata).parent_path().wstring();
					strdata = const_cast<LPWSTR>(wstrdata.c_str());
					nError = RegSetValueEx(hKey, TEXT("directory"), 0, REG_SZ, LPCBYTE(strdata), wcslen(strdata) * sizeof(TCHAR));

					if (nError == ERROR_SUCCESS) subkeys = tempkey;
					else DebugLogging("Failed to create registry key value");
				}
				else DebugLogging("Failed to create registry key");
			}

			RegCloseKey(hKey);
		}

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
		backUp(NemesisMainGUIClass);
		GetSettings();
		reset(NemesisMainGUIClass);

		try
		{
			nemesisInfo = new NemesisInfo;
		}
		catch (nemesis::exception)
		{
			buttonCheck->setDisabled(true);
			buttonLaunch->setDisabled(true);
			buttonUpdate->setDisabled(true);
			modView->setDisabled(true);
			comboBox->setDisabled(true);
			error = false;
		}

		QObject::connect(buttonLaunch, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleButton1()));
		QObject::connect(buttonUpdate, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleButton2()));
		QObject::connect(buttonCheck, SIGNAL(released()), NemesisMainGUIClass, SLOT(handleButton3()));
		QObject::connect(comboBox, SIGNAL(currentIndexChanged(QString)), NemesisMainGUIClass, SLOT(languageChange(QString)));

		QModelIndexList* indexlist = new QModelIndexList();

		for (int j = 0; j < modView->model()->columnCount(); ++j)
		{
			for (int i = 0; i < modView->model()->rowCount(); ++i)
			{
				indexlist->push_back(modView->model()->index(i, j));
			}
		}

		animProgressBar->setMaximum(MAX_ANIM);
		animProgressBar->setValue(0);

		modView->setColumnWidth(0, 380);
		modView->setColumnWidth(1, 75);
		modView->setColumnWidth(2, 47);
		modView->header()->setStyleSheet("background: transparent");
		modView->header()->setDefaultAlignment(Qt::AlignCenter);
		modView->header()->setSectionResizeMode(QHeaderView::Fixed);
	} // retranslateUi

	void reset(QWidget* NemesisMainGUIClass)
	{
		NemesisMainGUIClass->setWindowTitle(QApplication::translate("NemesisMainGUIClass", UIMessage(1000).c_str(), nullptr));
		label->setPixmap(QPixmap(":/background/Resources/title header 2.jpg"));
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
	class NemesisMainGUIClass : public Ui_NemesisMainGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEMESISMAINGUI_H
