#pragma once

#include <QCloseEvent>
#include <QThread>
#include <QTimer>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>

#include "debugmsg.h"
#include "ui_NemesisEngine.h"

class NemesisEngine : public QWidget
{
    Q_OBJECT

private:
    bool terminated = false;
    QTimer* resizeTimer;
    DebugMsg* DMsg;
    DummyLog* DLog;

    void setupUi();
    void reset();
    void GetSettings();

public:
    Ui::NemesisEngine ui;
    std::atomic_flag lock = ATOMIC_FLAG_INIT;
    int progressMax;
    int progressPercentage;

    NemesisEngine(QWidget* parent = Q_NULLPTR);
    ~NemesisEngine();

    void closeEvent(QCloseEvent* curEvent);
    void resizeEvent(QResizeEvent* event);
    bool isClosing();
    bool isResizing();

private slots:
    void handleLaunch(); // LAUNCH ENGINE
    void handleUpdate(); // UPDATE ENGINE
    void handleCheck();  // WARNING CHECK
    void sendMessage(QString input);
    void languageChange(QString language);
    void setProgressBarMax(int number);
    void setProgressBarValue();
    void firstNull();
    void resizeDone();
};
