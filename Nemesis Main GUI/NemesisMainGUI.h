#pragma once

#include <QTimer>
#include <QThread>
#include <QCloseEvent>

#include <QtWidgets/QWidget>
#include <QtWidgets/QMessageBox>

#include "ui_NemesisMainGUI.h"

class NemesisMainGUI : public QWidget
{
	Q_OBJECT
		
private:
	bool terminated = false;
	QTimer* resizeTimer;

public:
	Ui::NemesisMainGUIClass ui;
	std::atomic_flag lock = ATOMIC_FLAG_INIT;
	int progressMax;
	int progressPercentage;

	NemesisMainGUI(QWidget *parent = Q_NULLPTR);
	~NemesisMainGUI();

	void closeEvent(QCloseEvent* curEvent);
	void resizeEvent(QResizeEvent* event);
	bool isClosing();
	bool isResizing();

private slots:
	void handleLaunch();			// LAUNCH ENGINE
	void handleUpdate();			// UPDATE ENGINE
	void handleCheck();				// WARNING CHECK
	void sendMessage(QString input);
	void languageChange(QString language);
	void setProgressBarMax(int number);
	void setProgressBarValue();
	void firstNull();
	void resizeDone();
};
