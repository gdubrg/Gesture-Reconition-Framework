#define _USE_MATH_DEFINES

#include "acquisitionmyo.h"
#include "frameset.h"

#include <cmath>
#include <iomanip>
#include <string>
#include <algorithm>
#include <stdexcept>

#include <conio.h>
#include <fstream>

using namespace std;

// Constructor
AcquisitionMyo::AcquisitionMyo()
{
    // creo l'hub
    _pHub = new myo::Hub("it.unimore.imagelab.grframework"); // identificativo univoco

    // attesa di acquisizione dell'oggetto
    // todo: occhi a tenere il blocco attivo... vediamo se fare qualcosa ad eventi
    _pmyo = _pHub->waitForMyo(10000);

    // occhio errori

    if (_pHub && _pmyo)
        _pHub->addListener(&_collector);


}


AcquisitionMyo::~AcquisitionMyo(){

    delete _pHub;
    _pHub = nullptr;


}


void AcquisitionMyo::getFrame(FrameSet &frame){

    _pHub->run(1000/20);

    // qui abbiamo i dati dentro al collector
    cout << "nuovo frame da collector";
    cout << _collector.pitch_w << endl;

    frame.myoPitch = _collector.pitch_w;
    frame.myoRoll = _collector.roll_w;
    frame.myoYaw = _collector.yaw_w;

    for(int i=0;i<8;++i)
        frame.muscles[i] = _collector.datiEmg[i];

    frame.gyr_x = _collector.gyr_x;
    frame.gyr_y = _collector.gyr_y;
    frame.gyr_z = _collector.gyr_z;



}
