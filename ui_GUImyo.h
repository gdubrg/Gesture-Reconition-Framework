/********************************************************************************
** Form generated from reading UI file 'GUImyo.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIMYO_H
#define UI_GUIMYO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_GUImyo
{
public:
    QAction *actionProject;
    QAction *actionExit;
    QAction *actionKk;
    QAction *actionAbout;
    QAction *actionExit_2;
    QWidget *centralWidget;
    QLabel *label_4;
    QCustomPlot *chart1;
    QCustomPlot *chart2;
    QCustomPlot *chart3;
    QCustomPlot *chart4;
    QCustomPlot *chart5;
    QCustomPlot *chart6;
    QCustomPlot *chart8;
    QCustomPlot *chartOr;
    QCustomPlot *chartGir;
    QCustomPlot *chart7;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *labelPose;

    void setupUi(QWidget *GUImyo)
    {
        if (GUImyo->objectName().isEmpty())
            GUImyo->setObjectName(QStringLiteral("GUImyo"));
        GUImyo->resize(1920, 1080);
        GUImyo->setMouseTracking(false);
        GUImyo->setAutoFillBackground(false);
        GUImyo->setStyleSheet(QStringLiteral(""));
        actionProject = new QAction(GUImyo);
        actionProject->setObjectName(QStringLiteral("actionProject"));
        actionProject->setCheckable(false);
        actionProject->setChecked(false);
        actionProject->setEnabled(true);
        actionProject->setIconVisibleInMenu(true);
        actionExit = new QAction(GUImyo);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionKk = new QAction(GUImyo);
        actionKk->setObjectName(QStringLiteral("actionKk"));
        actionAbout = new QAction(GUImyo);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionExit_2 = new QAction(GUImyo);
        actionExit_2->setObjectName(QStringLiteral("actionExit_2"));
        centralWidget = new QWidget(GUImyo);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setGeometry(QRect(0, 0, 3290, 961));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 0, 321, 21));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_4->setFont(font);
        chart1 = new QCustomPlot(centralWidget);
        chart1->setObjectName(QStringLiteral("chart1"));
        chart1->setGeometry(QRect(40, 30, 1221, 71));
        chart2 = new QCustomPlot(centralWidget);
        chart2->setObjectName(QStringLiteral("chart2"));
        chart2->setGeometry(QRect(40, 110, 1221, 71));
        chart3 = new QCustomPlot(centralWidget);
        chart3->setObjectName(QStringLiteral("chart3"));
        chart3->setGeometry(QRect(40, 190, 1221, 71));
        chart4 = new QCustomPlot(centralWidget);
        chart4->setObjectName(QStringLiteral("chart4"));
        chart4->setGeometry(QRect(40, 270, 1221, 71));
        chart5 = new QCustomPlot(centralWidget);
        chart5->setObjectName(QStringLiteral("chart5"));
        chart5->setGeometry(QRect(40, 350, 1221, 71));
        chart6 = new QCustomPlot(centralWidget);
        chart6->setObjectName(QStringLiteral("chart6"));
        chart6->setGeometry(QRect(40, 430, 1221, 71));
        chart8 = new QCustomPlot(centralWidget);
        chart8->setObjectName(QStringLiteral("chart8"));
        chart8->setGeometry(QRect(40, 590, 1221, 71));
        chartOr = new QCustomPlot(centralWidget);
        chartOr->setObjectName(QStringLiteral("chartOr"));
        chartOr->setGeometry(QRect(40, 670, 1221, 111));
        chartGir = new QCustomPlot(centralWidget);
        chartGir->setObjectName(QStringLiteral("chartGir"));
        chartGir->setGeometry(QRect(40, 790, 1221, 111));
        chart7 = new QCustomPlot(centralWidget);
        chart7->setObjectName(QStringLiteral("chart7"));
        chart7->setGeometry(QRect(40, 510, 1221, 71));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 60, 21, 21));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 140, 47, 13));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 220, 47, 13));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 300, 47, 13));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 380, 47, 13));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 460, 47, 13));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 540, 47, 13));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 620, 47, 13));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(5, 720, 47, 13));
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 840, 47, 13));
        labelPose = new QLabel(centralWidget);
        labelPose->setObjectName(QStringLiteral("labelPose"));
        labelPose->setGeometry(QRect(440, 920, 351, 31));
        QFont font1;
        font1.setPointSize(12);
        labelPose->setFont(font1);
        label_4->raise();
        chart1->raise();
        chart2->raise();
        chart3->raise();
        chart4->raise();
        chart5->raise();
        chart6->raise();
        chart8->raise();
        chart7->raise();
        chartOr->raise();
        chartGir->raise();
        chart7->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_5->raise();
        label_6->raise();
        label_7->raise();
        label_8->raise();
        label_9->raise();
        label_10->raise();
        label_11->raise();
        labelPose->raise();

        retranslateUi(GUImyo);

        QMetaObject::connectSlotsByName(GUImyo);
    } // setupUi

    void retranslateUi(QWidget *GUImyo)
    {
        GUImyo->setWindowTitle(QApplication::translate("GUImyo", "Gesture Recognition with HMM", 0));
        actionProject->setText(QApplication::translate("GUImyo", "Project", 0));
        actionExit->setText(QApplication::translate("GUImyo", "Exit", 0));
        actionKk->setText(QApplication::translate("GUImyo", "kk", 0));
        actionAbout->setText(QApplication::translate("GUImyo", "About", 0));
        actionExit_2->setText(QApplication::translate("GUImyo", "Exit", 0));
        label_4->setText(QApplication::translate("GUImyo", "Myo Armbrand", 0));
        label->setText(QApplication::translate("GUImyo", "m1", 0));
        label_2->setText(QApplication::translate("GUImyo", "m2", 0));
        label_3->setText(QApplication::translate("GUImyo", "m3", 0));
        label_5->setText(QApplication::translate("GUImyo", "m4", 0));
        label_6->setText(QApplication::translate("GUImyo", "m5", 0));
        label_7->setText(QApplication::translate("GUImyo", "m6", 0));
        label_8->setText(QApplication::translate("GUImyo", "m7", 0));
        label_9->setText(QApplication::translate("GUImyo", "m8", 0));
        label_10->setText(QApplication::translate("GUImyo", "Orient.", 0));
        label_11->setText(QApplication::translate("GUImyo", "Gyro", 0));
        labelPose->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GUImyo: public Ui_GUImyo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIMYO_H
