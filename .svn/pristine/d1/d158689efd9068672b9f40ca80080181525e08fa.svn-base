#ifndef GUIGESTURERECOGNITIONTEST_H
#define GUIGESTURERECOGNITIONTEST_H

#include <QWidget>
#include <QVector>
#include <gesturerecognitiontest.h>


namespace Ui {
class GUIgesturerecognitiontest;
}


class GUIgesturerecognitiontest : public QWidget
{
    Q_OBJECT


public:
    explicit GUIgesturerecognitiontest(GestureRecognitionTEST *prec, QWidget *parent = 0);


private:
    Ui::GUIgesturerecognitiontest *ui;

    GestureRecognitionTEST *_pGestureRec;

    QVector<double> frames;
    QVector<double> vHandRight;
    QVector<double> vHandLeft;
    QVector<double> vElbowRight;
    QVector<double> vElbowLeft;
    QVector<double> headPitch;
    QVector<double> headYaw;
    QVector<double> headRoll;
    QVector<double> shoulderRotation;

    int numFrames = 0;


signals:


public slots:
    void UpdateInterface();
};

#endif // GUIGESTURERECOGNITIONTEST_H
