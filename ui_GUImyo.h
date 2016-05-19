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
    QLabel *label;
    QCustomPlot *chart2;
    QCustomPlot *chart3;
    QCustomPlot *chart4;
    QCustomPlot *chart5;
    QCustomPlot *chart6;
    QCustomPlot *chart2_6;
    QCustomPlot *chart7;
    QCustomPlot *chart8;
    QCustomPlot *chartOr;

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
        chart1->setGeometry(QRect(40, 30, 1221, 81));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(1180, 630, 47, 13));
        chart2 = new QCustomPlot(centralWidget);
        chart2->setObjectName(QStringLiteral("chart2"));
        chart2->setGeometry(QRect(40, 120, 1221, 81));
        chart3 = new QCustomPlot(centralWidget);
        chart3->setObjectName(QStringLiteral("chart3"));
        chart3->setGeometry(QRect(40, 210, 1221, 81));
        chart4 = new QCustomPlot(centralWidget);
        chart4->setObjectName(QStringLiteral("chart4"));
        chart4->setGeometry(QRect(40, 300, 1221, 81));
        chart5 = new QCustomPlot(centralWidget);
        chart5->setObjectName(QStringLiteral("chart5"));
        chart5->setGeometry(QRect(40, 390, 1221, 81));
        chart6 = new QCustomPlot(centralWidget);
        chart6->setObjectName(QStringLiteral("chart6"));
        chart6->setGeometry(QRect(40, 480, 1221, 81));
        chart2_6 = new QCustomPlot(centralWidget);
        chart2_6->setObjectName(QStringLiteral("chart2_6"));
        chart2_6->setGeometry(QRect(40, 570, 1221, 81));
        chart7 = new QCustomPlot(chart2_6);
        chart7->setObjectName(QStringLiteral("chart7"));
        chart7->setGeometry(QRect(0, 0, 1221, 81));
        chart8 = new QCustomPlot(centralWidget);
        chart8->setObjectName(QStringLiteral("chart8"));
        chart8->setGeometry(QRect(40, 650, 1221, 81));
        chartOr = new QCustomPlot(centralWidget);
        chartOr->setObjectName(QStringLiteral("chartOr"));
        chartOr->setGeometry(QRect(40, 750, 1221, 191));
        label_4->raise();
        chart1->raise();
        label->raise();
        chart2->raise();
        chart3->raise();
        chart4->raise();
        chart5->raise();
        chart6->raise();
        chart2_6->raise();
        chart8->raise();
        chart7->raise();
        chartOr->raise();

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
        label->setText(QApplication::translate("GUImyo", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class GUImyo: public Ui_GUImyo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIMYO_H
