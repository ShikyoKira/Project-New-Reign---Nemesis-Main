#pragma once

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QCloseEvent>
#include <QThread>
#include <QTimer>
#include "ui_NemesisMainGUI.h"
#include "master.h"

class NemesisMainGUI : public QWidget
{
	Q_OBJECT
		
private:
	bool terminated = false;

public:
	Ui::NemesisMainGUIClass ui;
	std::atomic_flag lock = ATOMIC_FLAG_INIT;
	int progressMax;
	int progressPercentage;

	NemesisMainGUI(QWidget *parent = Q_NULLPTR);
	~NemesisMainGUI();

	void closeEvent(QCloseEvent* curEvent);

private slots:
	void handleButton1()
	{
		if (terminated) return;

		ui.textBrowser->clear();
		ui.textBrowser->append(QString::fromStdString(TextBoxMessage(1002)));
		ui.progressBar->setHidden(false);
		ui.progressBar->setValue(0);
		ui.buttonLaunch->setDisabled(true);
		ui.buttonUpdate->setDisabled(true);
		ui.buttonCheck->setDisabled(true);
		ui.comboBox->setDisabled(true);
		ui.modView->setDisabled(true);
		ui.animProgressBar->setTextVisible(true);
		ui.animProgressBar->setStyleSheet("");
		ui.animProgressBar->font = "";
		ui.animProgressBar->newValue(0);
		ui.animProgressBar->setFormat("0 animation(s)");

		vecstr behaviorPriority;
		vecstr hiddenModList = getHiddenMods();
		std::unordered_map<std::string, bool> chosenBehavior;

		for (auto& mod : hiddenModList)
		{
			behaviorPriority.push_back(mod);
			chosenBehavior[mod] = true;
		}

		for (int i = 0; i < ui.modView->model()->rowCount(); ++i)
		{
			QAbstractItemModel* model = ui.modView->model();
			QVariant state = model->data(model->index(i, 0), Qt::CheckStateRole);
			
			if (state == Qt::Checked)
			{
				std::string modcode = modConvert[model->data(model->index(i, 0), Qt::DisplayRole).toString().toStdString()];
				behaviorPriority.insert(behaviorPriority.begin(), modcode);
				chosenBehavior[modcode] = true;
			}
		}
		
		QThread* thread = new QThread;
		BehaviorStart* worker = new BehaviorStart;
		worker->addBehaviorPick(worker, this, behaviorPriority, chosenBehavior);
		
		connect(worker, SIGNAL(totalAnim(int)), ui.animProgressBar, SLOT(newValue(int)));
		connect(thread, SIGNAL(started()), worker, SLOT(GenerateBehavior()));
		connect(worker, SIGNAL(progressUp()), this, SLOT(setProgressBarValue()));
		connect(worker, SIGNAL(progressMax(int)), this, SLOT(setProgressBarMax(int)));
		connect(worker, SIGNAL(incomingMessage(QString)), this, SLOT(sendMessage(QString)));

		connect(worker, SIGNAL(enable(bool)), ui.buttonLaunch, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enable(bool)), ui.buttonUpdate, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enableCheck(bool)), ui.buttonCheck, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enable(bool)), ui.comboBox, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enable(bool)), ui.modView, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(hide(bool)), ui.progressBar, SLOT(setHidden(bool)));

		connect(worker, SIGNAL(end()), thread, SLOT(quit()));
		connect(worker, SIGNAL(end()), worker, SLOT(deleteLater()));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

		worker->moveToThread(thread);
		thread->start();
	}

	void handleButton2()
	{
		if (terminated) return;

		ui.progressBar->setHidden(false);
		ui.progressBar->setValue(0);
		ui.buttonLaunch->setDisabled(true);
		ui.buttonUpdate->setDisabled(true);
		ui.buttonCheck->setDisabled(true);
		ui.comboBox->setDisabled(true);
		ui.modView->setDisabled(true);
		ui.textBrowser->clear();
		ui.textBrowser->append(QString::fromStdString(TextBoxMessage(1005)));
		ui.textBrowser->append("");

		QThread* thread = new QThread;
		UpdateFilesStart* worker = new UpdateFilesStart;

		connect(thread, SIGNAL(started()), worker, SLOT(UpdateFiles()));
		connect(worker, SIGNAL(progressUp()), this, SLOT(setProgressBarValue()));
		connect(worker, SIGNAL(progressMax(int)), this, SLOT(setProgressBarMax(int)));
		connect(worker, SIGNAL(incomingMessage(QString)), this, SLOT(sendMessage(QString)));
		connect(worker, SIGNAL(enable(bool)), ui.buttonLaunch, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enable(bool)), ui.buttonUpdate, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enable(bool)), ui.comboBox, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enable(bool)), ui.modView, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(hide(bool)), ui.progressBar, SLOT(setHidden(bool)));

		connect(worker, SIGNAL(end()), this, SLOT(firstNull()));
		connect(worker, SIGNAL(end()), thread, SLOT(quit()));
		connect(worker, SIGNAL(end()), worker, SLOT(deleteLater()));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		
		worker->moveToThread(thread);
		thread->start();
	}

	void handleButton3()
	{
		if (terminated) return;

		ui.buttonLaunch->setDisabled(true);
		ui.buttonUpdate->setDisabled(true);
		ui.buttonCheck->setDisabled(true);
		ui.comboBox->setDisabled(true);
		ui.modView->setDisabled(true);
		ui.textBrowser->clear();

		DummyLog* DLog = new DummyLog;
		connectProcess(DLog);
		QObject::connect(DLog, SIGNAL(incomingMessage(QString)), this, SLOT(sendMessage(QString)));

		warningCheck();

		disconnectProcess();
		delete DLog;

		ui.buttonLaunch->setDisabled(false);
		ui.buttonUpdate->setDisabled(false);
		ui.buttonCheck->setDisabled(false);
		ui.comboBox->setDisabled(false);
		ui.modView->setDisabled(false);
	}

	void sendMessage(QString input)
	{
		if (terminated) return;

		ui.textBrowser->append(input);
		ui.textBrowser->verticalScrollBar()->setValue(ui.textBrowser->verticalScrollBar()->maximum());
		ui.textBrowser->ensureCursorVisible();
	}

	void languageChange(QString language)
	{
		if (terminated) return;

		delete ui.DMsg;
		ui.DMsg = new DebugMsg(language.toStdString());
		createLanguageCache(language.toStdString());
		NewDebugMessage(*ui.DMsg);
		ui.reset(this);
		emit ui.modView->model()->headerDataChanged(Qt::Horizontal, 0, ui.modView->model()->columnCount());
	}

	void setProgressBarMax(int number)
	{
		progressMax = number;
		progressPercentage = 0;
	}

	void setProgressBarValue()
	{
		if (!error)
		{
			while(lock.test_and_set(std::memory_order_acquire));

			int old = progressPercentage * 100 / progressMax;
			++progressPercentage;
			int result = progressPercentage * 100 / progressMax;

			if (result > old)
			{
				if (result - old < 2)
				{
					ui.progressBar->setValue(old + 1);
					Sleep(75);
				}
				else
				{
					for (int i = old + 1; i <= result; ++i)
					{
						if (error)
						{
							break;
						}

						ui.progressBar->setValue(i);
						Sleep(75);
					}
				}
			}

			lock.clear(std::memory_order_release);
		}
	}

	void firstNull()
	{
		nemesisInfo->setFirst(false);
		nemesisInfo->iniFileUpdate();
	}
};
