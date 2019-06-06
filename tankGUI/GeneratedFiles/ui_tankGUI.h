/********************************************************************************
** Form generated from reading UI file 'tankGUI.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TANKGUI_H
#define UI_TANKGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tankGUIClass
{
public:
    QAction *actioninfo;
    QWidget *centralWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *runButton;
    QLabel *label_1;
    QPushButton *getColorButton;
    QLabel *extractedColorLabel;
    QLabel *imageLabel;
    QLabel *contoursLabel;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label;
    QLabel *tankReadyLabel;
    QMenuBar *menuBar;
    QMenu *menuinfo;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *tankGUIClass)
    {
        if (tankGUIClass->objectName().isEmpty())
            tankGUIClass->setObjectName(QString::fromUtf8("tankGUIClass"));
        tankGUIClass->resize(1111, 797);
        actioninfo = new QAction(tankGUIClass);
        actioninfo->setObjectName(QString::fromUtf8("actioninfo"));
        centralWidget = new QWidget(tankGUIClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayoutWidget = new QWidget(centralWidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(330, 30, 361, 161));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        runButton = new QPushButton(gridLayoutWidget);
        runButton->setObjectName(QString::fromUtf8("runButton"));

        gridLayout->addWidget(runButton, 0, 1, 1, 1);

        label_1 = new QLabel(gridLayoutWidget);
        label_1->setObjectName(QString::fromUtf8("label_1"));

        gridLayout->addWidget(label_1, 1, 0, 1, 1, Qt::AlignHCenter);

        getColorButton = new QPushButton(gridLayoutWidget);
        getColorButton->setObjectName(QString::fromUtf8("getColorButton"));

        gridLayout->addWidget(getColorButton, 0, 0, 1, 1);

        extractedColorLabel = new QLabel(gridLayoutWidget);
        extractedColorLabel->setObjectName(QString::fromUtf8("extractedColorLabel"));

        gridLayout->addWidget(extractedColorLabel, 1, 1, 1, 1, Qt::AlignHCenter);

        imageLabel = new QLabel(centralWidget);
        imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
        imageLabel->setGeometry(QRect(40, 220, 531, 480));
        imageLabel->setAlignment(Qt::AlignCenter);
        contoursLabel = new QLabel(centralWidget);
        contoursLabel->setObjectName(QString::fromUtf8("contoursLabel"));
        contoursLabel->setGeometry(QRect(600, 220, 501, 480));
        contoursLabel->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(220, 710, 121, 21));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(780, 710, 101, 16));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 141, 31));
        tankReadyLabel = new QLabel(centralWidget);
        tankReadyLabel->setObjectName(QString::fromUtf8("tankReadyLabel"));
        tankReadyLabel->setGeometry(QRect(180, 40, 31, 16));
        tankGUIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(tankGUIClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1111, 26));
        menuinfo = new QMenu(menuBar);
        menuinfo->setObjectName(QString::fromUtf8("menuinfo"));
        tankGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(tankGUIClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        tankGUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(tankGUIClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        tankGUIClass->setStatusBar(statusBar);

        menuBar->addAction(menuinfo->menuAction());
        menuinfo->addAction(actioninfo);

        retranslateUi(tankGUIClass);

        QMetaObject::connectSlotsByName(tankGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *tankGUIClass)
    {
        tankGUIClass->setWindowTitle(QApplication::translate("tankGUIClass", "tankGUI", nullptr));
        actioninfo->setText(QApplication::translate("tankGUIClass", "info", nullptr));
        runButton->setText(QApplication::translate("tankGUIClass", "Run", nullptr));
        label_1->setText(QApplication::translate("tankGUIClass", "Extracted color : ", nullptr));
        getColorButton->setText(QApplication::translate("tankGUIClass", "getColor", nullptr));
        extractedColorLabel->setText(QApplication::translate("tankGUIClass", "color here", nullptr));
        imageLabel->setText(QApplication::translate("tankGUIClass", "camera image", nullptr));
        contoursLabel->setText(QApplication::translate("tankGUIClass", "contours of maze", nullptr));
        label_2->setText(QApplication::translate("tankGUIClass", "Image from camera", nullptr));
        label_4->setText(QApplication::translate("tankGUIClass", "extracted lines", nullptr));
        label->setText(QApplication::translate("tankGUIClass", "Komunikacja z robotem", nullptr));
        tankReadyLabel->setText(QString());
        menuinfo->setTitle(QApplication::translate("tankGUIClass", "about", nullptr));
    } // retranslateUi

};

namespace Ui {
    class tankGUIClass: public Ui_tankGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TANKGUI_H
