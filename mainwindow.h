#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QStringList>

#include "frameset.h"
#include "acquisitionkinect1.h"
#include "acquisitionkinect2.h"
#include "acquisitionfile.h"
#include "gesturerecognitionhmm.h"
#include "gesturerecognitiondtw.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QStringList, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void processStep();

    // batch processing
    void BatchProcess1(QStringList);
    void Run(bool brun);
    void resetGUI();
    void doAll();
    void close();

private:
    Ui::MainWindow *ui;

    //acquisition
    Acquisition *_pkinect = nullptr;

    // gesture recognition
    GestureRecognition * _precognizer = nullptr;

    //Timer
    QTimer *timer;

    //Frame set
    FrameSet frame;

};

#endif // MAINWINDOW_H
