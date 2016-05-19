#ifndef ACQUISITIONFILE_H
#define ACQUISITIONFILE_H

#include <opencv2/opencv.hpp>

#include "acquisition.h"
#include "frameset.h"
#include <fstream>
#include <string>
#include <sstream>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class AcquisitionFile: public Acquisition {

public:

    //Constructor
    AcquisitionFile();

    //Destructor
    ~AcquisitionFile();

    //Get 1 frame from file
    void getFrame(FrameSet &frame);

    //Var
    std::string mainPath = "./gestures/";
    std::string mainFilename = "player.txt";
    std::string jsonFilename;
    int sizeCoord;
    int count = 0;

    //Paths
    std::string pathRGB;
    std::string pathDepth;
    std::string pathCoord;
    std::string pathJsonFile = "./gestures/";

    //Filenames
    std::ifstream playerFile; //file with json names
    std::ifstream coordFile; //file with coordinates

    void loadFiles();


};

#endif // ACQUISITIONFILE_H
