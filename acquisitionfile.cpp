#include "acquisitionfile.h"

using namespace std;
using namespace cv;

//Constructor
AcquisitionFile::AcquisitionFile(){

    //Create name file to open
    string namePlayer;
    namePlayer.append(mainPath);
    namePlayer.append(mainFilename);
    playerFile.open(namePlayer, ifstream::in);

    if(!playerFile.is_open())
        cout << "Error opening player file..." << endl;

    //Load file and all informations
    loadFiles();

}

AcquisitionFile::~AcquisitionFile(){

    //Close all files
    playerFile.close();
    coordFile.close();

}

//Get next frame from file
void AcquisitionFile::getFrame(FrameSet &frame){

    //Get RGB frame
    string tmpRGB;
    tmpRGB.append(pathRGB);
    tmpRGB.append("/");
    tmpRGB.append(to_string(count));
    tmpRGB.append(".jpg");
    frame.RGB = imread(tmpRGB);

    //Get Depth frame
    string tmpDepth;
    tmpDepth.append(pathDepth);
    tmpDepth.append("/");
    tmpDepth.append(to_string(count));
    tmpDepth.append(".jpg");
    frame.depth= imread(tmpDepth);

    //Get coordinates
    string line;

    for(int i=0;i<28;++i){
        getline(coordFile, line);
        stringstream sline(line);

        //Real coordinates
        sline >> frame.bodyJoint[i].x;
        sline >> frame.bodyJoint[i].y;
        sline >> frame.bodyJoint[i].z;

        //RGB coordinates
        sline >> frame.bodyJointRGB[i].x;
        sline >> frame.bodyJointRGB[i].y;

        //Depth coordinates
        sline >> frame.bodyJointDepth[i].x;
        sline >> frame.bodyJointDepth[i].y;
    }

    //Read head rotation
    getline(coordFile, line);
    stringstream linehead(line);
    linehead >> frame.pitch;
    linehead >> frame.yaw;
    linehead >> frame.roll;

    //Read end of frame
    getline(coordFile, line);

    //Increase counter
    count++;

    if(count > (sizeCoord - 1)){
        count = 0;
        coordFile.close();
        loadFiles();
    }

}

void AcquisitionFile::loadFiles(){

    if(!playerFile.eof()){

        //Read new name of file
        playerFile >> jsonFilename;

        //Open json file
        string tmpPathJsonFile;
        tmpPathJsonFile.append(pathJsonFile);
        tmpPathJsonFile.append(jsonFilename);
        QFile jsonFile(QString::fromStdString(tmpPathJsonFile));
        jsonFile.open(QFile::ReadOnly);
        QString contentJson = jsonFile.readAll();
        jsonFile.close();
        QJsonDocument documentJson = QJsonDocument::fromJson(contentJson.toUtf8());
        QJsonObject objectJson = documentJson.object();
        QJsonValue valueJson;

        //Read size
        valueJson = objectJson.value("Video length");
        sizeCoord = valueJson.toInt();

        //Read RGB path
        valueJson = objectJson.value("RGB Path");
        pathRGB = valueJson.toString().toStdString();

        //Read depth path
        valueJson = objectJson.value("Depth Path");
        pathDepth = valueJson.toString().toStdString();

        //Read coordinates
        valueJson = objectJson.value("Coordinates Path");
        pathCoord = valueJson.toString().toStdString();

        //Open file with coordinates
        coordFile.open(pathCoord, ifstream::in);
        if(!coordFile.is_open())
            cout << "Error opening coordinates file..." << endl;
    }

    //No other namefile
    else{
        jsonFilename = "";
        pathRGB = "";
        pathDepth = "";
        pathCoord = "";
        playerFile.close();

    }

}

