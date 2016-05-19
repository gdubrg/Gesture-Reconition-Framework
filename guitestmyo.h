#ifndef GUITESTMYO_H
#define GUITESTMYO_H

#include <QWidget>
#include <QVector>
#include <testmyo.h>

#include "ui_GUImyo.h"

class GUItestMyo : public QWidget{

    Q_OBJECT

public:

    explicit GUItestMyo(TestMyo *prec, QWidget *parent = 0);


private:

    Ui::GUImyo *ui;

    TestMyo *_pGestureRec;

    QVector<double> frames;
    QVector<double> m1;
    QVector<double> m2;
    QVector<double> m3;
    QVector<double> m4;
    QVector<double> m5;
    QVector<double> m6;
    QVector<double> m7;
    QVector<double> m8;

    QVector<double> pitch;
    QVector<double> roll;
    QVector<double> yaw;

    int numFrames = 0;



signals:


public slots:

    void UpdateInterface();
};

#endif // GUITESTMYO_H

