/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionProject;
    QAction *actionExit;
    QAction *actionKk;
    QAction *actionAbout;
    QAction *actionExit_2;
    QAction *actionClose;
    QAction *actionRun;
    QWidget *centralWidget;
    QGroupBox *boxRGB;
    QLabel *labelRGB;
    QWidget *widgetRecognizer;
    QGroupBox *boxDepth;
    QLabel *labelDepth;
    QWidget *widgetR;
    QGroupBox *groupSource;
    QRadioButton *radioKinect1;
    QRadioButton *radioKinect2;
    QRadioButton *radioFile;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QRadioButton *radioMyo;
    QLabel *label_11;
    QLabel *label_12;
    QGroupBox *groupMethod;
    QRadioButton *radioHMM;
    QRadioButton *radioDTW;
    QRadioButton *radioTest;
    QRadioButton *radioSave;
    QRadioButton *radioTestMyo;
    QPushButton *buttonStart;
    QWidget *widgetL;
    QLabel *labelTimeAcq;
    QLabel *label;
    QLabel *labelTimeElab;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *labelTotal;
    QLabel *label_4;
    QLabel *labelFPS;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuAbout;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1920, 1080);
        MainWindow->setMouseTracking(false);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QStringLiteral(""));
        actionProject = new QAction(MainWindow);
        actionProject->setObjectName(QStringLiteral("actionProject"));
        actionProject->setCheckable(false);
        actionProject->setChecked(false);
        actionProject->setEnabled(true);
        actionProject->setIconVisibleInMenu(true);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionKk = new QAction(MainWindow);
        actionKk->setObjectName(QStringLiteral("actionKk"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionExit_2 = new QAction(MainWindow);
        actionExit_2->setObjectName(QStringLiteral("actionExit_2"));
        actionClose = new QAction(MainWindow);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QStringLiteral("actionRun"));
        actionRun->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        boxRGB = new QGroupBox(centralWidget);
        boxRGB->setObjectName(QStringLiteral("boxRGB"));
        boxRGB->setGeometry(QRect(10, 10, 621, 450));
        boxRGB->setStyleSheet(QStringLiteral("font: 10pt \"Arial Unicode MS\";"));
        labelRGB = new QLabel(boxRGB);
        labelRGB->setObjectName(QStringLiteral("labelRGB"));
        labelRGB->setGeometry(QRect(10, 30, 601, 411));
        labelRGB->setAutoFillBackground(false);
        labelRGB->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        widgetRecognizer = new QWidget(boxRGB);
        widgetRecognizer->setObjectName(QStringLiteral("widgetRecognizer"));
        widgetRecognizer->setGeometry(QRect(509, 30, 921, 411));
        boxDepth = new QGroupBox(centralWidget);
        boxDepth->setObjectName(QStringLiteral("boxDepth"));
        boxDepth->setGeometry(QRect(10, 470, 621, 451));
        boxDepth->setStyleSheet(QStringLiteral("font: 10pt \"Arial Unicode MS\";"));
        labelDepth = new QLabel(boxDepth);
        labelDepth->setObjectName(QStringLiteral("labelDepth"));
        labelDepth->setGeometry(QRect(10, 30, 601, 411));
        labelDepth->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        widgetR = new QWidget(centralWidget);
        widgetR->setObjectName(QStringLiteral("widgetR"));
        widgetR->setGeometry(QRect(640, 0, 1271, 961));
        groupSource = new QGroupBox(widgetR);
        groupSource->setObjectName(QStringLiteral("groupSource"));
        groupSource->setGeometry(QRect(90, 50, 441, 411));
        QFont font;
        font.setPointSize(11);
        groupSource->setFont(font);
        radioKinect1 = new QRadioButton(groupSource);
        radioKinect1->setObjectName(QStringLiteral("radioKinect1"));
        radioKinect1->setGeometry(QRect(30, 40, 111, 31));
        radioKinect2 = new QRadioButton(groupSource);
        radioKinect2->setObjectName(QStringLiteral("radioKinect2"));
        radioKinect2->setGeometry(QRect(30, 130, 111, 31));
        radioFile = new QRadioButton(groupSource);
        radioFile->setObjectName(QStringLiteral("radioFile"));
        radioFile->setGeometry(QRect(30, 210, 91, 31));
        label_5 = new QLabel(groupSource);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(50, 30, 111, 61));
        label_5->setPixmap(QPixmap(QString::fromUtf8("Images/kinect1.png")));
        label_5->setScaledContents(true);
        label_6 = new QLabel(groupSource);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(60, 130, 91, 31));
        label_6->setPixmap(QPixmap(QString::fromUtf8("Images/kinect2.png")));
        label_6->setScaledContents(true);
        label_7 = new QLabel(groupSource);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(180, 40, 141, 41));
        label_8 = new QLabel(groupSource);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(180, 130, 141, 41));
        label_9 = new QLabel(groupSource);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(70, 200, 64, 64));
        label_9->setPixmap(QPixmap(QString::fromUtf8("Images/folder-128.png")));
        label_9->setScaledContents(true);
        label_10 = new QLabel(groupSource);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(180, 210, 141, 41));
        radioMyo = new QRadioButton(groupSource);
        radioMyo->setObjectName(QStringLiteral("radioMyo"));
        radioMyo->setGeometry(QRect(30, 300, 82, 17));
        label_11 = new QLabel(groupSource);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(180, 290, 141, 41));
        label_12 = new QLabel(groupSource);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(45, 280, 111, 71));
        label_12->setPixmap(QPixmap(QString::fromUtf8("Images/myo.png")));
        label_12->setScaledContents(true);
        groupMethod = new QGroupBox(widgetR);
        groupMethod->setObjectName(QStringLiteral("groupMethod"));
        groupMethod->setGeometry(QRect(570, 50, 421, 411));
        groupMethod->setFont(font);
        radioHMM = new QRadioButton(groupMethod);
        radioHMM->setObjectName(QStringLiteral("radioHMM"));
        radioHMM->setGeometry(QRect(30, 40, 181, 31));
        radioDTW = new QRadioButton(groupMethod);
        radioDTW->setObjectName(QStringLiteral("radioDTW"));
        radioDTW->setGeometry(QRect(30, 125, 181, 31));
        radioTest = new QRadioButton(groupMethod);
        radioTest->setObjectName(QStringLiteral("radioTest"));
        radioTest->setGeometry(QRect(30, 205, 191, 31));
        radioSave = new QRadioButton(groupMethod);
        radioSave->setObjectName(QStringLiteral("radioSave"));
        radioSave->setGeometry(QRect(30, 285, 251, 17));
        radioTestMyo = new QRadioButton(groupMethod);
        radioTestMyo->setObjectName(QStringLiteral("radioTestMyo"));
        radioTestMyo->setGeometry(QRect(30, 360, 201, 21));
        buttonStart = new QPushButton(widgetR);
        buttonStart->setObjectName(QStringLiteral("buttonStart"));
        buttonStart->setGeometry(QRect(510, 490, 91, 41));
        buttonStart->setFont(font);
        widgetL = new QWidget(widgetR);
        widgetL->setObjectName(QStringLiteral("widgetL"));
        widgetL->setGeometry(QRect(0, 0, 1271, 961));
        widgetL->raise();
        groupSource->raise();
        groupMethod->raise();
        buttonStart->raise();
        labelTimeAcq = new QLabel(centralWidget);
        labelTimeAcq->setObjectName(QStringLiteral("labelTimeAcq"));
        labelTimeAcq->setGeometry(QRect(70, 930, 61, 31));
        labelTimeAcq->setFont(font);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 930, 31, 31));
        label->setFont(font);
        labelTimeElab = new QLabel(centralWidget);
        labelTimeElab->setObjectName(QStringLiteral("labelTimeElab"));
        labelTimeElab->setGeometry(QRect(170, 930, 71, 31));
        labelTimeElab->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(140, 930, 61, 31));
        label_2->setFont(font);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(260, 930, 81, 31));
        label_3->setFont(font);
        labelTotal = new QLabel(centralWidget);
        labelTotal->setObjectName(QStringLiteral("labelTotal"));
        labelTotal->setGeometry(QRect(340, 930, 71, 31));
        labelTotal->setFont(font);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(430, 930, 41, 31));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        label_4->setFont(font1);
        labelFPS = new QLabel(centralWidget);
        labelFPS->setObjectName(QStringLiteral("labelFPS"));
        labelFPS->setGeometry(QRect(470, 930, 71, 31));
        labelFPS->setFont(font);
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1920, 21));
        QPalette palette;
        menuBar->setPalette(palette);
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        QPalette palette1;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        menuAbout->setPalette(palette1);
        menuAbout->setStyleSheet(QLatin1String("color: rgb(0, 0, 0);\n"
"\n"
""));
        MainWindow->setMenuBar(menuBar);

        mainToolBar->addAction(actionRun);
        mainToolBar->addAction(actionClose);
        mainToolBar->addAction(actionExit_2);
        mainToolBar->addSeparator();
        menuBar->addAction(menuAbout->menuAction());
        menuAbout->addAction(actionProject);
        menuAbout->addAction(actionExit_2);

        retranslateUi(MainWindow);
        QObject::connect(actionExit_2, SIGNAL(triggered()), MainWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Gesture Recognition Framework ImageLab (GRF)", 0));
        actionProject->setText(QApplication::translate("MainWindow", "Project", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionKk->setText(QApplication::translate("MainWindow", "kk", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionExit_2->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionClose->setText(QApplication::translate("MainWindow", "Close", 0));
        actionRun->setText(QApplication::translate("MainWindow", "Run", 0));
        boxRGB->setTitle(QApplication::translate("MainWindow", "RGB Video", 0));
        labelRGB->setText(QString());
        boxDepth->setTitle(QApplication::translate("MainWindow", "Depth Video and Skeleton", 0));
        labelDepth->setText(QString());
        groupSource->setTitle(QApplication::translate("MainWindow", "Sources", 0));
        radioKinect1->setText(QString());
        radioKinect2->setText(QString());
        radioFile->setText(QString());
        label_5->setText(QString());
        label_6->setText(QString());
        label_7->setText(QApplication::translate("MainWindow", "Microsoft Kinect 1", 0));
        label_8->setText(QApplication::translate("MainWindow", "Microsoft Kinect 2", 0));
        label_9->setText(QString());
        label_10->setText(QApplication::translate("MainWindow", "Gesture File", 0));
        radioMyo->setText(QString());
        label_11->setText(QApplication::translate("MainWindow", "Myo", 0));
        label_12->setText(QString());
        groupMethod->setTitle(QApplication::translate("MainWindow", "Methods", 0));
        radioHMM->setText(QApplication::translate("MainWindow", "Hidden Markov Model", 0));
        radioDTW->setText(QApplication::translate("MainWindow", "Dinamic Time Warping", 0));
        radioTest->setText(QApplication::translate("MainWindow", "Testing", 0));
        radioSave->setText(QApplication::translate("MainWindow", "Save and Annotation Tool", 0));
        radioTestMyo->setText(QApplication::translate("MainWindow", "Testing Myo Armband", 0));
        buttonStart->setText(QApplication::translate("MainWindow", "Start", 0));
        labelTimeAcq->setText(QString());
        label->setText(QApplication::translate("MainWindow", "AT:", 0));
        labelTimeElab->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "ET:", 0));
        label_3->setText(QApplication::translate("MainWindow", "Total Time:", 0));
        labelTotal->setText(QString());
        label_4->setText(QApplication::translate("MainWindow", "FPS:", 0));
        labelFPS->setText(QString());
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
