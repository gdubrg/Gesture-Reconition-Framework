#ifndef ACQUISITION_H
#define ACQUISITION_H

#include "frameset.h"

#define MODE_CLOSEST 1
#define MODE_ID 2
#define RESOLUTION_VGA 1
#define RESOLUTION_FULLHD 2

class Acquisition{

public:

    //Constructor
    Acquisition();

    //Get the next frame from generic Kinect
    virtual void getFrame(FrameSet &frame) = 0;

    //Set wich skeleton
    virtual void setSkeleton(int mode);

    //Set resolution
    virtual void setResolution(int mode);

    int modeSkeleton;
    int resolution;

};

#endif // ACQUISITION_H
