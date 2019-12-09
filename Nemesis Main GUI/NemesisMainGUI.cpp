#include <boost\atomic.hpp>
#include <boost\thread.hpp>

#include "master.h"
#include "behaviorcheck.h"
#include "NemesisMainGUI.h"

#include "utilities\Terminator.h"

std::atomic<unsigned int> resizeCount = 0;
boost::atomic_flag atm_resize = BOOST_ATOMIC_FLAG_INIT;

NemesisMainGUI::NemesisMainGUI(QWidget *parent)
	: QWidget(parent)
{
	resizeTimer = new QTimer;
	resizeTimer->setSingleShot(true);
	connect(resizeTimer, SIGNAL(timeout()), this, SLOT(resizeDone()));
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
		nemesisInfo->iniFileUpdate();
	}
}

void NemesisMainGUI::resizeEvent(QResizeEvent* event)
{
	++resizeCount;
	resizeTimer->start(500);
	int oldSize = ui.modView->columnWidth(0) + ui.modView->columnWidth(1) + ui.modView->columnWidth(2);
	QWidget::resizeEvent(event);

	if (event->oldSize() == QSize(-1, -1)) return;

	int newSize = ui.modView->columnWidth(0) + ui.modView->columnWidth(1) + ui.modView->columnWidth(2);
	int incre = newSize - oldSize;

	int total = 0;
	std::vector<unsigned int> columnSizes = { nemesisInfo->GetModNameWidth(), nemesisInfo->GetAuthorWidth(), nemesisInfo->GetPriorityWidth() };

	for (auto& size : columnSizes)
	{
		total += size;
	}

	double increPerc = double(newSize) / double(oldSize);

	for (unsigned int i = 0; i < columnSizes.size(); ++i)
	{
		double newCSize = increPerc * double(columnSizes[i]);
		ui.modView->setColumnWidth(i, newCSize);
	}
}

bool NemesisMainGUI::isClosing()
{
	return terminated;
}

bool NemesisMainGUI::isResizing()
{
	return resizeCount != 0;
}

void NemesisMainGUI::handleLaunch()
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
	worker->addBehaviorPick(this, behaviorPriority, chosenBehavior);

	connect(worker, SIGNAL(totalAnim(int)), ui.animProgressBar, SLOT(newValue(int)));
	connect(thread, SIGNAL(started()), worker, SLOT(InitializeGeneration()));
	connect(worker, SIGNAL(progressUp()), this, SLOT(setProgressBarValue()));
	connect(worker, SIGNAL(progressMax(int)), this, SLOT(setProgressBarMax(int)));
	connect(worker, SIGNAL(incomingMessage(QString)), this, SLOT(sendMessage(QString)));

	connect(worker, SIGNAL(disableLaunch(bool)), ui.buttonLaunch, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(disable(bool)), ui.buttonLaunch, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(disable(bool)), ui.buttonUpdate, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(disableCheck(bool)), ui.buttonCheck, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(disable(bool)), ui.comboBox, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(disable(bool)), ui.modView, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(hide(bool)), ui.progressBar, SLOT(setHidden(bool)));

	connect(worker, SIGNAL(end()), thread, SLOT(quit()));
	connect(worker, SIGNAL(end()), worker, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	worker->moveToThread(thread);
	thread->start();
}

void NemesisMainGUI::handleUpdate()
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

	connect(worker, SIGNAL(disableLaunch(bool)), ui.buttonLaunch, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(disable(bool)), ui.buttonUpdate, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(disable(bool)), ui.comboBox, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(disable(bool)), ui.modView, SLOT(setDisabled(bool)));
	connect(worker, SIGNAL(hide(bool)), ui.progressBar, SLOT(setHidden(bool)));

	connect(worker, SIGNAL(end()), this, SLOT(firstNull()));
	connect(worker, SIGNAL(end()), thread, SLOT(quit()));
	connect(worker, SIGNAL(end()), worker, SLOT(deleteLater()));
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	worker->moveToThread(thread);
	thread->start();
}

void NemesisMainGUI::handleCheck()
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

void NemesisMainGUI::sendMessage(QString input)
{
	if (terminated) return;

	ui.textBrowser->append(input);
	ui.textBrowser->verticalScrollBar()->setValue(ui.textBrowser->verticalScrollBar()->maximum());
	ui.textBrowser->ensureCursorVisible();
}

void NemesisMainGUI::languageChange(QString language)
{
	if (terminated) return;

	delete ui.DMsg;
	ui.DMsg = new DebugMsg(language.toStdString());
	createLanguageCache(language.toStdString());
	NewDebugMessage(*ui.DMsg);
	ui.reset(this);
	emit ui.modView->model()->headerDataChanged(Qt::Horizontal, 0, ui.modView->model()->columnCount());
}

void NemesisMainGUI::setProgressBarMax(int number)
{
	progressMax = number;
	progressPercentage = 0;
}

void NemesisMainGUI::setProgressBarValue()
{
	if (!error)
	{
		while (lock.test_and_set(std::memory_order_acquire));

		try
		{
			int old = progressPercentage * 100 / progressMax;
			++progressPercentage;
			int result = progressPercentage * 100 / progressMax;

			if (result > old)
			{
				if (result - old < 2)
				{
					ui.progressBar->setValue(old + 1);
					std::this_thread::sleep_for(std::chrono::milliseconds(75));
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
						std::this_thread::sleep_for(std::chrono::milliseconds(75));
					}
				}
			}
		}
		catch (std::exception& ex)
		{
			lock.clear(std::memory_order_release);
			throw ex;
		}

		lock.clear(std::memory_order_release);
	}
}

void NemesisMainGUI::firstNull()
{
	nemesisInfo->setFirst(false);
	nemesisInfo->iniFileUpdate();
}

void NemesisMainGUI::resizeDone()
{
	while (atm_resize.test_and_set(boost::memory_order_acquire));
	--resizeCount;

	if (resizeCount == 0)
	{
		nemesisInfo->setWidth(width());
		nemesisInfo->setHeight(height());
		nemesisInfo->iniFileUpdate();
	}

	atm_resize.clear(boost::memory_order_release);
}
