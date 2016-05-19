#ifndef GUIGESTURERECOGNITIONHMM_H
#define GUIGESTURERECOGNITIONHMM_H

#include <QWidget>
#include <gesturerecognitionhmm.h>


namespace Ui {
class GUIgesturerecognitionhmm;
}


class GUIgesturerecognitionhmm : public QWidget
{
    Q_OBJECT
public:
    explicit GUIgesturerecognitionhmm(GestureRecognitionHMM *prec, QWidget *parent = 0);

private:
    Ui::GUIgesturerecognitionhmm *ui;

    GestureRecognitionHMM *_pGestureRec;

signals:

public slots:
    void UpdateInterface();
};

#endif // GUIGESTURERECOGNITIONHMM_H
