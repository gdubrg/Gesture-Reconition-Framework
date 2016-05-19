/********************************************************************************
** Form generated from reading UI file 'GUIgesturerecognitiondtw.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIGESTURERECOGNITIONDTW_H
#define UI_GUIGESTURERECOGNITIONDTW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_GUIgesturerecognitiondtw
{
public:
    QAction *actionProject;
    QAction *actionExit;
    QAction *actionKk;
    QAction *actionAbout;
    QAction *actionExit_2;
    QWidget *centralWidget;
    QLabel *label_4;
    QGroupBox *boxWS;
    QLabel *labelWS;
    QGroupBox *groupBox;
    QLabel *labelGestureLabel;
    QGroupBox *groupBoxDTWvalue;
    QLabel *labelDTWvalue;
    QGroupBox *groupBoxClass;
    QLabel *labelClass;
    QGroupBox *groupBoxAllScores;
    QLabel *labelAllScores;
    QGroupBox *groupBoxClassDTW;
    QLabel *labelClassDTW;
    QCustomPlot *chart;

    void setupUi(QWidget *GUIgesturerecognitiondtw)
    {
        if (GUIgesturerecognitiondtw->objectName().isEmpty())
            GUIgesturerecognitiondtw->setObjectName(QStringLiteral("GUIgesturerecognitiondtw"));
        GUIgesturerecognitiondtw->resize(1920, 1080);
        GUIgesturerecognitiondtw->setMouseTracking(false);
        GUIgesturerecognitiondtw->setAutoFillBackground(false);
        GUIgesturerecognitiondtw->setStyleSheet(QStringLiteral(""));
        actionProject = new QAction(GUIgesturerecognitiondtw);
        actionProject->setObjectName(QStringLiteral("actionProject"));
        actionProject->setCheckable(false);
        actionProject->setChecked(false);
        actionProject->setEnabled(true);
        actionProject->setIconVisibleInMenu(true);
        actionExit = new QAction(GUIgesturerecognitiondtw);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionKk = new QAction(GUIgesturerecognitiondtw);
        actionKk->setObjectName(QStringLiteral("actionKk"));
        actionAbout = new QAction(GUIgesturerecognitiondtw);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionExit_2 = new QAction(GUIgesturerecognitiondtw);
        actionExit_2->setObjectName(QStringLiteral("actionExit_2"));
        centralWidget = new QWidget(GUIgesturerecognitiondtw);
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
        boxWS = new QGroupBox(centralWidget);
        boxWS->setObjectName(QStringLiteral("boxWS"));
        boxWS->setGeometry(QRect(30, 50, 361, 81));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(11);
        boxWS->setFont(font1);
        labelWS = new QLabel(boxWS);
        labelWS->setObjectName(QStringLiteral("labelWS"));
        labelWS->setGeometry(QRect(30, 30, 281, 31));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(450, 50, 361, 511));
        groupBox->setFont(font1);
        labelGestureLabel = new QLabel(groupBox);
        labelGestureLabel->setObjectName(QStringLiteral("labelGestureLabel"));
        labelGestureLabel->setGeometry(QRect(16, 20, 331, 431));
        groupBoxDTWvalue = new QGroupBox(centralWidget);
        groupBoxDTWvalue->setObjectName(QStringLiteral("groupBoxDTWvalue"));
        groupBoxDTWvalue->setGeometry(QRect(30, 140, 361, 81));
        groupBoxDTWvalue->setFont(font1);
        labelDTWvalue = new QLabel(groupBoxDTWvalue);
        labelDTWvalue->setObjectName(QStringLiteral("labelDTWvalue"));
        labelDTWvalue->setGeometry(QRect(16, 32, 271, 31));
        groupBoxClass = new QGroupBox(centralWidget);
        groupBoxClass->setObjectName(QStringLiteral("groupBoxClass"));
        groupBoxClass->setGeometry(QRect(850, 50, 371, 511));
        groupBoxClass->setFont(font1);
        labelClass = new QLabel(groupBoxClass);
        labelClass->setObjectName(QStringLiteral("labelClass"));
        labelClass->setGeometry(QRect(16, 23, 341, 421));
        groupBoxAllScores = new QGroupBox(centralWidget);
        groupBoxAllScores->setObjectName(QStringLiteral("groupBoxAllScores"));
        groupBoxAllScores->setGeometry(QRect(30, 320, 361, 241));
        groupBoxAllScores->setFont(font1);
        labelAllScores = new QLabel(groupBoxAllScores);
        labelAllScores->setObjectName(QStringLiteral("labelAllScores"));
        labelAllScores->setGeometry(QRect(10, 20, 341, 161));
        groupBoxClassDTW = new QGroupBox(centralWidget);
        groupBoxClassDTW->setObjectName(QStringLiteral("groupBoxClassDTW"));
        groupBoxClassDTW->setGeometry(QRect(30, 230, 361, 80));
        groupBoxClassDTW->setFont(font1);
        labelClassDTW = new QLabel(groupBoxClassDTW);
        labelClassDTW->setObjectName(QStringLiteral("labelClassDTW"));
        labelClassDTW->setGeometry(QRect(30, 30, 201, 31));
        chart = new QCustomPlot(centralWidget);
        chart->setObjectName(QStringLiteral("chart"));
        chart->setGeometry(QRect(10, 570, 1221, 261));

        retranslateUi(GUIgesturerecognitiondtw);

        QMetaObject::connectSlotsByName(GUIgesturerecognitiondtw);
    } // setupUi

    void retranslateUi(QWidget *GUIgesturerecognitiondtw)
    {
        GUIgesturerecognitiondtw->setWindowTitle(QApplication::translate("GUIgesturerecognitiondtw", "Gesture Recognition with HMM", 0));
        actionProject->setText(QApplication::translate("GUIgesturerecognitiondtw", "Project", 0));
        actionExit->setText(QApplication::translate("GUIgesturerecognitiondtw", "Exit", 0));
        actionKk->setText(QApplication::translate("GUIgesturerecognitiondtw", "kk", 0));
        actionAbout->setText(QApplication::translate("GUIgesturerecognitiondtw", "About", 0));
        actionExit_2->setText(QApplication::translate("GUIgesturerecognitiondtw", "Exit", 0));
        label_4->setText(QApplication::translate("GUIgesturerecognitiondtw", "Dynamic  Time Warping GUI Unit", 0));
        boxWS->setTitle(QApplication::translate("GUIgesturerecognitiondtw", "Window Size", 0));
        labelWS->setText(QString());
        groupBox->setTitle(QApplication::translate("GUIgesturerecognitiondtw", "Gesture Labels", 0));
        labelGestureLabel->setText(QString());
        groupBoxDTWvalue->setTitle(QApplication::translate("GUIgesturerecognitiondtw", "DTW value", 0));
        labelDTWvalue->setText(QString());
        groupBoxClass->setTitle(QApplication::translate("GUIgesturerecognitiondtw", "Gestures Classified", 0));
        labelClass->setText(QString());
        groupBoxAllScores->setTitle(QApplication::translate("GUIgesturerecognitiondtw", "All DTW values", 0));
        labelAllScores->setText(QString());
        groupBoxClassDTW->setTitle(QApplication::translate("GUIgesturerecognitiondtw", "Classification", 0));
        labelClassDTW->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GUIgesturerecognitiondtw: public Ui_GUIgesturerecognitiondtw {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIGESTURERECOGNITIONDTW_H
