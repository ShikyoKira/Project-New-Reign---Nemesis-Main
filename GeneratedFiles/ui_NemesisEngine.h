/********************************************************************************
** Form generated from reading UI file 'NemesisEngine.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEMESISENGINE_H
#define UI_NEMESISENGINE_H

#include <QtCore/QLocale>
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
#include "header/ui/AnimProgressBar.h"
#include "header/ui/BehaviorListView.h"

QT_BEGIN_NAMESPACE

class Ui_NemesisEngine
{
public:
    QGridLayout *gridLayout;
    AnimProgressBar *animProgressBar;
    QSpacerItem *horizontalSpacer;
    QTextBrowser *textBrowser;
    QPushButton *buttonCheck;
    QProgressBar *progressBar;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *buttonUpdate;
    BehaviorListView *modView;
    QPushButton *buttonLaunch;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_4;
    QComboBox *comboBox;
    QLabel *label;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_8;

    void setupUi(QWidget *NemesisEngine)
    {
        if (NemesisEngine->objectName().isEmpty())
            NemesisEngine->setObjectName(QString::fromUtf8("NemesisEngine"));
        NemesisEngine->resize(528, 715);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(NemesisEngine->sizePolicy().hasHeightForWidth());
        NemesisEngine->setSizePolicy(sizePolicy);
        NemesisEngine->setMinimumSize(QSize(528, 559));
        NemesisEngine->setMaximumSize(QSize(16777215, 16777215));
        NemesisEngine->setStyleSheet(QString::fromUtf8("background-image: url(:/background/Resources/background.png);"));
        gridLayout = new QGridLayout(NemesisEngine);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        animProgressBar = new AnimProgressBar(NemesisEngine);
        animProgressBar->setObjectName(QString::fromUtf8("animProgressBar"));
        animProgressBar->setMaximum(10000);
        animProgressBar->setValue(0);
        animProgressBar->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(animProgressBar, 6, 0, 1, 10);

        horizontalSpacer = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 7, 0, 1, 1);

        textBrowser = new QTextBrowser(NemesisEngine);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy1);
        textBrowser->setMaximumSize(QSize(16777215, 1000));
        textBrowser->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

        gridLayout->addWidget(textBrowser, 15, 0, 1, 10);

        buttonCheck = new QPushButton(NemesisEngine);
        buttonCheck->setObjectName(QString::fromUtf8("buttonCheck"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonCheck->sizePolicy().hasHeightForWidth());
        buttonCheck->setSizePolicy(sizePolicy2);
        buttonCheck->setMinimumSize(QSize(250, 35));
        buttonCheck->setMaximumSize(QSize(16777215, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("Cambria"));
        font.setPointSize(12);
        buttonCheck->setFont(font);
        buttonCheck->setCursor(QCursor(Qt::PointingHandCursor));
        buttonCheck->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(buttonCheck, 10, 0, 1, 5);

        progressBar = new QProgressBar(NemesisEngine);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);
        progressBar->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(progressBar, 16, 0, 1, 10);

        horizontalSpacer_2 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 0, 0, 2, 1);

        horizontalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 0, 1, 2, 1);

        horizontalSpacer_3 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 7, 9, 1, 1);

        buttonUpdate = new QPushButton(NemesisEngine);
        buttonUpdate->setObjectName(QString::fromUtf8("buttonUpdate"));
        sizePolicy2.setHeightForWidth(buttonUpdate->sizePolicy().hasHeightForWidth());
        buttonUpdate->setSizePolicy(sizePolicy2);
        buttonUpdate->setMinimumSize(QSize(250, 35));
        buttonUpdate->setMaximumSize(QSize(16777215, 16777215));
        buttonUpdate->setFont(font);
        buttonUpdate->setCursor(QCursor(Qt::PointingHandCursor));
        buttonUpdate->setStyleSheet(QString::fromUtf8(""));

        gridLayout->addWidget(buttonUpdate, 10, 5, 1, 5);

        modView = new BehaviorListView(NemesisEngine);
        modView->setObjectName(QString::fromUtf8("modView"));
        sizePolicy1.setHeightForWidth(modView->sizePolicy().hasHeightForWidth());
        modView->setSizePolicy(sizePolicy1);
        modView->setMinimumSize(QSize(0, 0));
        modView->setMaximumSize(QSize(16777215, 500));
        modView->setStyleSheet(QString::fromUtf8(""));
        modView->setFrameShape(QFrame::Box);
        modView->setDragEnabled(true);
        modView->setDragDropMode(QAbstractItemView::DragDrop);
        modView->setDefaultDropAction(Qt::MoveAction);
        modView->setAlternatingRowColors(true);
        modView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        modView->setSortingEnabled(true);
        modView->setExpandsOnDoubleClick(false);
        modView->header()->setDefaultSectionSize(35);
        modView->header()->setStretchLastSection(false);

        gridLayout->addWidget(modView, 5, 0, 1, 10);

        buttonLaunch = new QPushButton(NemesisEngine);
        buttonLaunch->setObjectName(QString::fromUtf8("buttonLaunch"));
        sizePolicy2.setHeightForWidth(buttonLaunch->sizePolicy().hasHeightForWidth());
        buttonLaunch->setSizePolicy(sizePolicy2);
        buttonLaunch->setMinimumSize(QSize(200, 45));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Cambria"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        buttonLaunch->setFont(font1);
        buttonLaunch->setCursor(QCursor(Qt::PointingHandCursor));
        buttonLaunch->setStyleSheet(QString::fromUtf8(""));
        buttonLaunch->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));

        gridLayout->addWidget(buttonLaunch, 7, 1, 1, 8);

        horizontalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 0, 8, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(80, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 9, 1, 1);

        comboBox = new QComboBox(NemesisEngine);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy3);
        comboBox->setMinimumSize(QSize(0, 0));
        comboBox->setMaximumSize(QSize(500, 16777215));
        comboBox->setContextMenuPolicy(Qt::ActionsContextMenu);
        comboBox->setLayoutDirection(Qt::LeftToRight);

        gridLayout->addWidget(comboBox, 1, 8, 1, 2);

        label = new QLabel(NemesisEngine);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy4);
        label->setMinimumSize(QSize(0, 40));
        label->setMaximumSize(QSize(240, 55));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Cambria"));
        font2.setPointSize(16);
        font2.setBold(true);
        font2.setWeight(75);
        label->setFont(font2);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"background-image: url(:/background/Resources/title.png);"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/background/Resources/title header 2.jpg")));
        label->setScaledContents(true);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 3, 2, 4);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_7, 0, 2, 2, 1);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_8, 0, 7, 2, 1);


        retranslateUi(NemesisEngine);

        QMetaObject::connectSlotsByName(NemesisEngine);
    } // setupUi

    void retranslateUi(QWidget *NemesisEngine)
    {
        NemesisEngine->setWindowTitle(QCoreApplication::translate("NemesisEngine", "NemesisEngine", nullptr));
#if QT_CONFIG(tooltip)
        animProgressBar->setToolTip(QCoreApplication::translate("NemesisEngine", "Green - Good\n"
"Yellow - OK\n"
"Red - Danger", nullptr));
#endif // QT_CONFIG(tooltip)
        textBrowser->setHtml(QCoreApplication::translate("NemesisEngine", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        buttonCheck->setText(QCoreApplication::translate("NemesisEngine", "Conflict Test", nullptr));
        buttonUpdate->setText(QCoreApplication::translate("NemesisEngine", "Update Engine", nullptr));
        buttonLaunch->setText(QCoreApplication::translate("NemesisEngine", "Launch Nemesis Behavior Engine", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("NemesisEngine", "english", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("NemesisEngine", "dutch", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("NemesisEngine", "chinese", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("NemesisEngine", "japanese", nullptr));

    } // retranslateUi

};

namespace Ui {
    class NemesisEngine: public Ui_NemesisEngine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEMESISENGINE_H
