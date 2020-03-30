#include "Global.h"

#include <boost/atomic.hpp>

#include "connector.h"

#include "ui/ErrorMsgBox.h"
#include "ui/MessageHandler.h"
#include "ui/NemesisEngine.h"
#include "ui/SettingsSave.h"
#include "ui/Terminator.h"

#include "update/updateprocess.h"

#include "generate/behaviorcheck.h"
#include "generate/behaviorprocess.h"

std::atomic<uint> resizeCount = 0;
boost::atomic_flag atm_resize         = BOOST_ATOMIC_FLAG_INIT;

NemesisEngine::NemesisEngine(QWidget* parent)
    : QWidget(parent)
{
    resizeTimer = new QTimer;
    resizeTimer->setSingleShot(true);
    connect(resizeTimer, SIGNAL(timeout()), this, SLOT(resizeDone()));
    setupUi();
}

NemesisEngine::~NemesisEngine()
{}

void NemesisEngine::setupUi()
{
    if (this->objectName().isEmpty()) this->setObjectName(QStringLiteral("NemesisMainGUIClass"));
    this->resize(528, 782);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->setMinimumSize(QSize(528, 559));
    this->setMaximumSize(QSize(700, 950));

    ui.gridLayout = new QGridLayout(this);
    ui.gridLayout->setSpacing(6);
    ui.gridLayout->setContentsMargins(11, 11, 11, 11);
    ui.gridLayout->setObjectName(QStringLiteral("gridLayout"));
    ui.gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);

    ui.buttonCheck = new QPushButton(this);
    ui.buttonCheck->setObjectName(QStringLiteral("buttonCheck"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(ui.buttonCheck->sizePolicy().hasHeightForWidth());
    ui.buttonCheck->setSizePolicy(sizePolicy1);
    ui.buttonCheck->setDisabled(true);
    ui.buttonCheck->setMinimumSize(QSize(250, 35));
    ui.buttonCheck->setMaximumSize(QSize(500, 16777215));
    QFont font;
    font.setFamily(QStringLiteral("Cambria"));
    font.setPointSize(12);
    ui.buttonCheck->setFont(font);
    ui.buttonCheck->setCursor(QCursor(Qt::PointingHandCursor));

    ui.gridLayout->addWidget(ui.buttonCheck, 10, 0, 1, 5);

    ui.textBrowser = new QTextBrowser(this);
    ui.textBrowser->setObjectName(QStringLiteral("textBrowser"));
    QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(ui.textBrowser->sizePolicy().hasHeightForWidth());
    ui.textBrowser->setSizePolicy(sizePolicy2);
    ui.textBrowser->setMaximumSize(QSize(16777215, 1000));
    ui.textBrowser->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    ui.gridLayout->addWidget(ui.textBrowser, 15, 0, 1, 10);

    ui.animProgressBar = new AnimProgressBar(this);
    ui.animProgressBar->setObjectName(QStringLiteral("animProgressBar"));
    ui.animProgressBar->setMaximum(10000);
    ui.animProgressBar->setValue(0);
    ui.animProgressBar->setAlignment(Qt::AlignCenter);

    ui.gridLayout->addWidget(ui.animProgressBar, 6, 0, 1, 10);

    ui.progressBar = new QProgressBar(this);
    ui.progressBar->setObjectName(QStringLiteral("progressBar"));
    ui.progressBar->setValue(0);
    ui.progressBar->setAlignment(Qt::AlignCenter);

    ui.gridLayout->addWidget(ui.progressBar, 16, 0, 1, 10);

    ui.buttonUpdate = new QPushButton(this);
    ui.buttonUpdate->setObjectName(QStringLiteral("buttonUpdate"));
    sizePolicy1.setHeightForWidth(ui.buttonUpdate->sizePolicy().hasHeightForWidth());
    ui.buttonUpdate->setSizePolicy(sizePolicy1);
    ui.buttonUpdate->setMinimumSize(QSize(250, 35));
    ui.buttonUpdate->setMaximumSize(QSize(500, 16777215));
    ui.buttonUpdate->setFont(font);
    ui.buttonUpdate->setCursor(QCursor(Qt::PointingHandCursor));

    ui.gridLayout->addWidget(ui.buttonUpdate, 10, 5, 1, 5);

    ui.buttonLaunch = new QPushButton(this);
    ui.buttonLaunch->setObjectName(QStringLiteral("buttonLaunch"));
    sizePolicy1.setHeightForWidth(ui.buttonLaunch->sizePolicy().hasHeightForWidth());
    ui.buttonLaunch->setSizePolicy(sizePolicy1);
    ui.buttonLaunch->setMinimumSize(QSize(200, 45));
    QFont font1;
    font1.setFamily(QStringLiteral("Cambria"));
    font1.setPointSize(12);
    font1.setBold(true);
    font1.setWeight(75);
    ui.buttonLaunch->setFont(font1);
    ui.buttonLaunch->setCursor(QCursor(Qt::PointingHandCursor));

    ui.gridLayout->addWidget(ui.buttonLaunch, 7, 1, 1, 8);

    ui.horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    ui.gridLayout->addItem(ui.horizontalSpacer, 7, 0, 1, 1);

    ui.horizontalSpacer_2 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    ui.gridLayout->addItem(ui.horizontalSpacer_2, 0, 0, 2, 1);

    ui.horizontalSpacer_3 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    ui.gridLayout->addItem(ui.horizontalSpacer_3, 7, 9, 1, 1);

    ui.horizontalSpacer_4 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    ui.gridLayout->addItem(ui.horizontalSpacer_4, 0, 9, 1, 1);

    ui.horizontalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    ui.gridLayout->addItem(ui.horizontalSpacer_5, 0, 8, 1, 1);

    ui.horizontalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    ui.gridLayout->addItem(ui.horizontalSpacer_6, 0, 1, 2, 1);

    ui.horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    ui.gridLayout->addItem(ui.horizontalSpacer_7, 0, 2, 2, 1);

    ui.horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    ui.gridLayout->addItem(ui.horizontalSpacer_8, 0, 7, 2, 1);

    ui.comboBox = new QComboBox(this);
    ui.comboBox->setObjectName(QStringLiteral("comboBox"));
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(ui.comboBox->sizePolicy().hasHeightForWidth());
    ui.comboBox->setSizePolicy(sizePolicy3);
    ui.comboBox->setMinimumSize(QSize(0, 0));
    ui.comboBox->setMaximumSize(QSize(500, 16777215));
    ui.comboBox->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui.comboBox->setLayoutDirection(Qt::LeftToRight);

    ui.gridLayout->addWidget(ui.comboBox, 1, 8, 1, 2);

    ui.label = new QLabel(this);
    ui.label->setObjectName(QStringLiteral("label"));
    QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy4.setHorizontalStretch(0);
    sizePolicy4.setVerticalStretch(0);
    sizePolicy4.setHeightForWidth(ui.label->sizePolicy().hasHeightForWidth());
    ui.label->setSizePolicy(sizePolicy4);
    ui.label->setMinimumSize(QSize(0, 40));
    ui.label->setMaximumSize(QSize(206, 55));
    ui.label->setAlignment(Qt::AlignCenter);
    ui.label->setScaledContents(true);

    ui.gridLayout->addWidget(ui.label, 0, 3, 2, 4);

    ui.modView = new BehaviorListView(this);

    if (error) return;

    ui.modView->setObjectName(QStringLiteral("modView"));
    QSizePolicy sizePolicy5(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy5.setHorizontalStretch(0);
    sizePolicy5.setVerticalStretch(0);
    sizePolicy5.setHeightForWidth(ui.modView->sizePolicy().hasHeightForWidth());
    ui.modView->setSizePolicy(sizePolicy5);
    ui.modView->setMinimumSize(QSize(0, 265));
    ui.modView->setMaximumSize(QSize(16777215, 300));
    ui.modView->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked
                                | QAbstractItemView::DoubleClicked);
    ui.modView->setDragEnabled(true);
    ui.modView->setAcceptDrops(true);
    ui.modView->setDragDropMode(QAbstractItemView::InternalMove);
    ui.modView->setDropIndicatorShown(true);
    ui.modView->setDefaultDropAction(Qt::MoveAction);
    ui.modView->setAlternatingRowColors(true);
    ui.modView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui.modView->setSortingEnabled(true);
    ui.modView->setExpandsOnDoubleClick(false);

    DLog = new DummyLog;
    connectProcess(DLog);
    QObject::connect(DLog, SIGNAL(incomingMessage(QString)), this, SLOT(sendMessage(QString)));

    GetSettings();

    this->setWindowTitle(QString::fromStdString(UIMessage(1000)));
    ui.label->setPixmap(QPixmap(":/background/Resources/title header.png"));
    ui.animProgressBar->setToolTip(QString::fromStdString(UIMessage(1004)));
    ui.buttonUpdate->setText(QString::fromStdString(UIMessage(1003)));
    ui.buttonCheck->setText(QString::fromStdString(UIMessage(1002)));
    ui.buttonLaunch->setText(QString::fromStdString(UIMessage(1001)));

    bool exception = false;
    nemesisInfo    = new NemesisInfo(exception);

    if (exception)
    {
        ui.buttonCheck->setDisabled(true);
        ui.buttonLaunch->setDisabled(true);
        ui.buttonUpdate->setDisabled(true);
        ui.comboBox->setDisabled(true);
        ui.modView->setDisabled(true);
        error = false;
    }

    this->resize(nemesisInfo->GetWidth(), nemesisInfo->GetHeight());

    ui.modView->header()->setStyleSheet("background: transparent");
    ui.modView->header()->setDefaultAlignment(Qt::AlignCenter);
    ui.modView->header()->setSectionResizeMode(QHeaderView::ResizeMode::Interactive);

    ui.modView->setColumnWidth(0, nemesisInfo->GetModNameWidth());
    ui.modView->setColumnWidth(1, nemesisInfo->GetAuthorWidth());
    ui.modView->setColumnWidth(2, nemesisInfo->GetPriorityWidth());

    QObject::connect(
        ui.modView, SIGNAL(dropModelUpdate(bool)), ui.modView->model(), SLOT(dropModelUpdate(bool)));

    ui.gridLayout->addWidget(ui.modView, 5, 0, 1, 10);

    QObject::connect(ui.buttonLaunch, SIGNAL(released()), this, SLOT(handleLaunch()));
    QObject::connect(ui.buttonUpdate, SIGNAL(released()), this, SLOT(handleUpdate()));
    QObject::connect(ui.buttonCheck, SIGNAL(released()), this, SLOT(handleCheck()));
    QObject::connect(ui.comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(languageChange(QString)));

    QModelIndexList* indexlist = new QModelIndexList();

    for (int j = 0; j < ui.modView->model()->columnCount(); ++j)
    {
        for (int i = 0; i < ui.modView->model()->rowCount(); ++i)
        {
            indexlist->push_back(ui.modView->model()->index(i, j));
        }
    }

    ui.buttonLaunch->setDisabled(nemesisInfo->IsFirst());

    ui.animProgressBar->setMaximum(MAX_ANIM);
    ui.animProgressBar->setValue(0);

    QMetaObject::connectSlotsByName(this);
}

void NemesisEngine::reset()
{
    this->setWindowTitle(QString::fromStdString(UIMessage(1000)));
    ui.label->setPixmap(QPixmap(":/background/Resources/title header.png"));
    ui.animProgressBar->setToolTip(QString::fromStdString(UIMessage(1004)));
    ui.buttonUpdate->setText(QString::fromStdString(UIMessage(1003)));
    ui.buttonCheck->setText(QString::fromStdString(UIMessage(1002)));
    ui.buttonLaunch->setText(QString::fromStdString(UIMessage(1001)));
}

void NemesisEngine::GetSettings()
{
    std::wstring language;
    std::unordered_map<std::string, bool> chosenBehavior;

    if (!isFileExist(L"languages"))
    {
        CEMsgBox* msg = new CEMsgBox;
        msg->setWindowTitle("ERROR");
        msg->setText("Error: \"languages\" folder not found. Please reinstall Nemesis");
        msg->show();
    }
    else if (!isFileExist(L"languages\\english.txt"))
    {
        CEMsgBox* msg = new CEMsgBox;
        msg->setWindowTitle("ERROR");
        msg->setText("Error: \"english.txt\" file not found in language folder. Please reinstall Nemesis");
        msg->show();
    }

    std::vector<std::wstring> languagelist;
    int curindex = -1;
    bool cacheResult;

    read_directory(L"languages", languagelist);

    for (uint i = 0; i < languagelist.size(); ++i)
    {
        ui.comboBox->addItem(QString());
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

        for (uint i = 0; i < languagelist.size(); ++i)
        {
            std::wstring curLang = GetFileName(languagelist[i]);
            ui.comboBox->setItemText(i, QString::fromStdWString(curLang));

            if (curLang == language)
            {
                curindex = i;
                DMsg     = new DebugMsg(language);
            }
            else if (language == L"english")
            {
                english = i;
            }
        }

        if (curindex < 0)
        {
            DMsg = new DebugMsg("english");
            interMsg("Previous language pack is not found. Default language is being used instead");
            ui.comboBox->setCurrentIndex(english);
        }
        else
        {
            ui.comboBox->setCurrentIndex(curindex);
        }

        // check the check state of mods in previous runtime
        for (int i = 0; i < ui.modView->model()->rowCount(); ++i)
        {
            QAbstractItemModel* model = ui.modView->model();
            std::string mod = (model->data(model->index(i, 0), Qt::DisplayRole)).toString().toStdString();

            if (chosenBehavior.find(mod) != chosenBehavior.end())
            {
                if (chosenBehavior[mod])
                { model->setData(model->index(i, 0), Qt::Checked, Qt::CheckStateRole); }
            }
        }
    }
    else
    {
        for (uint i = 0; i < languagelist.size(); ++i)
        {
            std::wstring curLang = GetFileName(languagelist[i]);
            ui.comboBox->setItemText(i, QString::fromStdWString(curLang));

            if (curLang == language) { curindex = i; }
        }

        DMsg = new DebugMsg(language);
        ui.comboBox->setCurrentIndex(curindex);
    }

    NewDebugMessage(*DMsg);
}

void NemesisEngine::closeEvent(QCloseEvent* curEvent)
{
    error      = true;
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

void NemesisEngine::resizeEvent(QResizeEvent* event)
{
    ++resizeCount;
    resizeTimer->start(500);
    int oldSize = ui.modView->columnWidth(0) + ui.modView->columnWidth(1) + ui.modView->columnWidth(2);
    QWidget::resizeEvent(event);

    if (event->oldSize() == QSize(-1, -1)) return;

    int newSize = ui.modView->columnWidth(0) + ui.modView->columnWidth(1) + ui.modView->columnWidth(2);
    int incre   = newSize - oldSize;

    int total = 0;
    std::vector<uint> columnSizes
        = {nemesisInfo->GetModNameWidth(), nemesisInfo->GetAuthorWidth(), nemesisInfo->GetPriorityWidth()};

    for (auto& size : columnSizes)
    {
        total += size;
    }

    double increPerc = double(newSize) / double(oldSize);

    for (uint i = 0; i < columnSizes.size(); ++i)
    {
        double newCSize = increPerc * double(columnSizes[i]);
        ui.modView->setColumnWidth(i, newCSize);
    }
}

bool NemesisEngine::isClosing()
{
    return terminated;
}

bool NemesisEngine::isResizing()
{
    return resizeCount != 0;
}

void NemesisEngine::handleLaunch()
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

    VecStr behaviorPriority;
    VecStr hiddenModList = getHiddenMods();
    std::unordered_map<std::string, bool> chosenBehavior;

    for (auto& mod : hiddenModList)
    {
        behaviorPriority.push_back(mod);
        chosenBehavior[mod] = true;
    }

    for (int i = 0; i < ui.modView->model()->rowCount(); ++i)
    {
        QAbstractItemModel* model = ui.modView->model();
        QVariant state            = model->data(model->index(i, 0), Qt::CheckStateRole);

        if (state == Qt::Checked)
        {
            std::string modcode
                = modConvert[model->data(model->index(i, 0), Qt::DisplayRole).toString().toStdString()];
            behaviorPriority.insert(behaviorPriority.begin(), modcode);
            chosenBehavior[modcode] = true;
        }
    }

    QThread* thread       = new QThread;
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

void NemesisEngine::handleUpdate()
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

    QThread* thread          = new QThread;
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

void NemesisEngine::handleCheck()
{
    if (terminated) return;

    ui.buttonLaunch->setDisabled(true);
    ui.buttonUpdate->setDisabled(true);
    ui.buttonCheck->setDisabled(true);
    ui.comboBox->setDisabled(true);
    ui.modView->setDisabled(true);

    QMessageBox::StandardButton reply;

    reply = QMessageBox::information(this,
                                     QString::fromStdString(TextBoxMessage(1019)),
                                     QString::fromStdString(TextBoxMessage(1018)),
                                     QMessageBox::Ok,
                                     QMessageBox::Abort);

    if (reply == QMessageBox::Ok)
    {
        ui.textBrowser->clear();

        DummyLog* DLog = new DummyLog;
        connectProcess(DLog);
        QObject::connect(DLog, SIGNAL(incomingMessage(QString)), this, SLOT(sendMessage(QString)));

        warningCheck();

        disconnectProcess();
        delete DLog;
    }

    ui.buttonLaunch->setDisabled(false);
    ui.buttonUpdate->setDisabled(false);
    ui.buttonCheck->setDisabled(false);
    ui.comboBox->setDisabled(false);
    ui.modView->setDisabled(false);
}

void NemesisEngine::sendMessage(QString input)
{
    if (terminated) return;

    ui.textBrowser->append(input);
    ui.textBrowser->verticalScrollBar()->setValue(ui.textBrowser->verticalScrollBar()->maximum());
    ui.textBrowser->ensureCursorVisible();
}

void NemesisEngine::languageChange(QString language)
{
    if (terminated) return;

    delete DMsg;
    DMsg = new DebugMsg(language.toStdString());
    createLanguageCache(language.toStdString());
    NewDebugMessage(*DMsg);
    reset();
    emit ui.modView->model()->headerDataChanged(Qt::Horizontal, 0, ui.modView->model()->columnCount());
}

void NemesisEngine::setProgressBarMax(int number)
{
    progressMax        = number;
    progressPercentage = 0;
}

void NemesisEngine::setProgressBarValue()
{
    if (!error)
    {
        Lockless_s plock(lock);
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
                    if (error) { break; }

                    ui.progressBar->setValue(i);
                    std::this_thread::sleep_for(std::chrono::milliseconds(75));
                }
            }
        }
    }
}

void NemesisEngine::firstNull()
{
    nemesisInfo->setFirst(false);
    nemesisInfo->iniFileUpdate();
}

void NemesisEngine::resizeDone()
{
    while (atm_resize.test_and_set(boost::memory_order_acquire))
        ;
    --resizeCount;

    if (resizeCount == 0)
    {
        nemesisInfo->setWidth(width());
        nemesisInfo->setHeight(height());
        nemesisInfo->iniFileUpdate();
    }

    atm_resize.clear(boost::memory_order_release);
}
