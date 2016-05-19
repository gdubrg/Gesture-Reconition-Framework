#ifndef TESTMYO_H
#define TESTMYO_H

#include "gesturerecognition.h"
#include "frameset.h"


class TestMyo : public GestureRecognition{

    Q_OBJECT

public:

    // Constructor
    TestMyo();

    void recognize(FrameSet &frame);

    // Frameset
    FrameSet _frame;

    int numFrame;

signals:
    void FrameAcquired();

};

#endif // TESTMYO_H
