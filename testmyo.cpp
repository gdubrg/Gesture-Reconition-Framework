#include "testmyo.h"
#include "frameset.h"

using namespace std;
using namespace cv;

TestMyo::TestMyo()
{

}


void TestMyo::recognize(FrameSet &frame){

    _frame = frame;

    numFrame++;

    emit FrameAcquired();
}

