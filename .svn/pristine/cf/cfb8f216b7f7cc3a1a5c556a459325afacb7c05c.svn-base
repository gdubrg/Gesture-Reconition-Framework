#ifndef GUIGESTURERECOGNITIONDTW_H
#define GUIGESTURERECOGNITIONDTW_H

#include <QWidget>
#include <QVector>
#include <gesturerecognitiondtw.h>


namespace Ui {
class GUIgesturerecognitiondtw;
}


class GUIgesturerecognitiondtw : public QWidget
{
    Q_OBJECT
public:
    explicit GUIgesturerecognitiondtw(GestureRecognitionDTW *prec, QWidget *parent = 0);

private:
    Ui::GUIgesturerecognitiondtw *ui;

    GestureRecognitionDTW *_pGestureRec;

    std::string allGesturesClassified;

    QVector<double> frames;
    int nFrames = 0;
    QVector<double> DTWValues;
    QVector<double> windowBorder;

    QVector<double> thresh1;
    QVector<double> frameThresh;
    QVector<double> thresh2;

signals:

public slots:
    void UpdateInterface();
    void updateClassLabel();
};

#endif // GUIGESTURERECOGNITIONDTW_H
