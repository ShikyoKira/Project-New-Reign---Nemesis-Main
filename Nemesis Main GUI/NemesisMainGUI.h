#pragma once

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QThread>
#include <QTimer>
#include <mutex>
#include "ui_NemesisMainGUI.h"
#include "master.h"

class NemesisMainGUI : public QWidget
{
	Q_OBJECT

public:
	NemesisMainGUI(QWidget *parent = Q_NULLPTR);
	~NemesisMainGUI();
	
private:
	Ui::NemesisMainGUIClass ui;
	std::mutex mLock;

private slots:
	void handleButton1()
	{
		ui.textBrowser->clear();
		ui.textBrowser->append("Initializing behavior generation\n");

		if (!isEngineUpdated())
		{
			ui.textBrowser->append(QString::fromStdString(TextBoxMessage(1000)));
			return;
		}

		ui.progressBar->setHidden(false);
		ui.progressBar->setValue(0);
		ui.buttonLaunch->setDisabled(true);
		ui.buttonUpdate->setDisabled(true);
		ui.buttonCheck->setDisabled(true);
		ui.modView->setDisabled(true);

		vecstr behaviorPriority;
		std::unordered_map<std::string, bool> chosenBehavior;

		for (int i = 0; i < ui.modView->model()->rowCount(); ++i)
		{
			BehaviorInfo result = ui.modView->model()->data(ui.modView->model()->index(i, 0)).value<BehaviorInfo>();
			
			if (result.state == Qt::Checked)
			{
				std::string modcode = modConvert[result.modname.toStdString()];
				behaviorPriority.push_back(modcode);
				chosenBehavior[modcode] = true;
			}
		}
		
		QThread* thread = new QThread;
		BehaviorStart* worker = new BehaviorStart;
		worker->addBehaviorPick(behaviorPriority, chosenBehavior);

		connect(worker, SIGNAL(progress(int)), ui.progressBar, SLOT(setValue(int)));
		connect(thread, SIGNAL(started()), worker, SLOT(GenerateBehavior()));
		connect(worker, SIGNAL(progressUp()), worker, SLOT(milestoneUp()));
		connect(worker, SIGNAL(incomingMessage(QString)), this, SLOT(sendMessage(QString)));

		connect(worker, SIGNAL(enable(bool)), ui.buttonLaunch, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enable(bool)), ui.buttonUpdate, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enableCheck(bool)), ui.buttonCheck, SLOT(setDisabled(bool)));
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
		ui.progressBar->setHidden(false);
		ui.progressBar->setValue(0);
		ui.buttonLaunch->setDisabled(true);
		ui.buttonUpdate->setDisabled(true);
		ui.buttonCheck->setDisabled(true);
		ui.modView->setDisabled(true);
		ui.textBrowser->clear();
		ui.textBrowser->append("Initializing engine update\n");

		QThread* thread = new QThread;
		UpdateFilesStart* worker = new UpdateFilesStart;

		connect(worker, SIGNAL(progress(int)), ui.progressBar, SLOT(setValue(int)));
		connect(thread, SIGNAL(started()), worker, SLOT(UpdateFiles()));
		connect(worker, SIGNAL(progressUp()), worker, SLOT(milestoneUp()));
		connect(worker, SIGNAL(incomingMessage(QString)), this, SLOT(sendMessage(QString)));

		if (usedAnim.size() > 0)
		{
			connect(worker, SIGNAL(enableCheck(bool)), ui.buttonCheck, SLOT(setDisabled(bool)));
		}

		connect(worker, SIGNAL(enable(bool)), ui.buttonLaunch, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enable(bool)), ui.buttonUpdate, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(enable(bool)), ui.modView, SLOT(setDisabled(bool)));
		connect(worker, SIGNAL(hide(bool)), ui.progressBar, SLOT(setHidden(bool)));

		connect(worker, SIGNAL(end()), thread, SLOT(quit()));
		connect(worker, SIGNAL(end()), worker, SLOT(deleteLater()));
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		
		worker->moveToThread(thread);
		thread->start();
	}

	void handleButton3()
	{
		ui.buttonLaunch->setDisabled(true);
		ui.buttonUpdate->setDisabled(true);
		ui.buttonCheck->setDisabled(true);
		ui.modView->setDisabled(true);

		behaviorCheck();

		ui.buttonLaunch->setDisabled(false);
		ui.buttonUpdate->setDisabled(false);
		ui.buttonCheck->setDisabled(false);
		ui.modView->setDisabled(false);
	}

	void sendMessage(QString input)
	{
		ui.textBrowser->append(input);
		ui.textBrowser->verticalScrollBar()->setValue(ui.textBrowser->verticalScrollBar()->maximum());
		ui.textBrowser->ensureCursorVisible();
	}
};
