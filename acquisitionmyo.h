#ifndef ACQUISITIONMYO_H
#define ACQUISITIONMYO_H

#include "frameset.h"
#include "acquisition.h"

#include "myocollector.h"


class AcquisitionMyo: public Acquisition {

public:

    // Constructor
    AcquisitionMyo();

    // Destructor
    virtual ~AcquisitionMyo();

    // Get 1 frame
    virtual void getFrame(FrameSet &frame);

protected:
    // hub pointer
    myo::Hub* _pHub;

    myo::Myo* _pmyo;

    MyoCollector _collector;


};

#endif // ACQUISITIONMYO_H

