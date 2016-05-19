#ifndef GUIGESTURERECOGNITIONSAVE_H
#define GUIGESTURERECOGNITIONSAVE_H

#include <QWidget>
#include <QVector>
#include <gesturerecognitionsave.h>


namespace Ui {
class GUIgesturerecognitionsave;
}


class GUIgesturerecognitionsave : public QWidget
{
    Q_OBJECT


public:
    explicit GUIgesturerecognitionsave(GestureRecognitionSave *prec, QWidget *parent = 0);

    std::stringstream bufferSubmissions;

private:
    Ui::GUIgesturerecognitionsave *ui;

    GestureRecognitionSave *_pGestureRec;


signals:


public slots:
    void start();
    void stop();
    void updateInterface();
    void updateSaving();
    void updateOpening();
    void annotate();
    void submit();
    void save();
    void syncSlider();
    void reset();
};

#endif // GUIGESTURERECOGNITIONSAVE_H
