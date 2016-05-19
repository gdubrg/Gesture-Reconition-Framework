#include "gesturerecognitionsave.h"
#include "frameset.h"

#include <iostream>
#include <iomanip>

#include <fstream>
#include <QDir>
#include <QFileDialog>
#include <QEvent>

using namespace std;
using namespace cv;

static const int maxWindowSize = 2000;

GestureRecognitionSave::GestureRecognitionSave(){
}

void GestureRecognitionSave::recognize(FrameSet &frame){

    if(acquisition){

        if(vFrames.size() < maxWindowSize){

            vFrames.push_back(frame);
            emit frameArrived();
        }
        else{
            acquisition = false;
            saveToFile();
            vFrames.clear();
            emit acquisitionCompleted();
        }

    }


}

void GestureRecognitionSave::startRec(){
   acquisition = true;
}

void GestureRecognitionSave::stopRec(){
    acquisition = false;
    saveToFile();
    vFrames.clear();
    emit acquisitionCompleted();
}

void GestureRecognitionSave::saveToFile(){

    string relPath = "./gestures/data/";
    QString relQPath = QString::fromStdString(relPath);

    //Create a directory for RGB Images
    string nameRGB = filename;
    nameRGB.append("_RGB");
    QDir(relQPath).mkdir(QString::fromStdString(nameRGB));
    string dirRGB;
    dirRGB.append(relPath);
    dirRGB.append(nameRGB);

    //Create a directory for depth images
    string nameDepth = filename;
    nameDepth.append("_depth");
    QDir(relQPath).mkdir(QString::fromStdString(nameDepth));
    string dirDepth;
    dirDepth.append(relPath);
    dirDepth.append(nameDepth);

    //Create a directory for player silhouette images
    string namePlay = filename;
    namePlay.append("_player");
    QDir(relQPath).mkdir(QString::fromStdString(namePlay));
    string dirPlayer;
    dirPlayer.append(relPath);
    dirPlayer.append(namePlay);

    //Create path and file name
    string pathFile = relPath;
    string filenameTXT = filename;
    filenameTXT.append((".txt"));
    pathFile.append(filenameTXT);
    ofstream outfile(pathFile);

    //Save data for each frame
    for(int i=0;i<vFrames.size();++i){

        savingState = i;

        //Save all joints (FLT_MAX if not valid)
        for(int k=0;k<28;++k){
        outfile << vFrames[i].bodyJoint[k].x << "\t";
        outfile << vFrames[i].bodyJoint[k].y << "\t";
        outfile << vFrames[i].bodyJoint[k].z << "\t";

        outfile << vFrames[i].bodyJointRGB[k].x << "\t";
        outfile << vFrames[i].bodyJointRGB[k].y << "\t";

        outfile << vFrames[i].bodyJointDepth[k].x << "\t";
        outfile << vFrames[i].bodyJointDepth[k].y << "\n";

        }

        outfile << vFrames[i].pitch << "\t";
        outfile << vFrames[i].yaw << "\t";
        outfile << vFrames[i].roll << "\n";

        //Next frame
        outfile << "----------\n";

        stringstream incNum;
        incNum << setfill('0') << setw(6) << to_string(i);

        //Save RGB Images
        string pathRGB;
        pathRGB.append(relPath);
        pathRGB.append("/");
        pathRGB.append(nameRGB);
        pathRGB.append("/");
        pathRGB.append(incNum.str());
        pathRGB.append(".png");
        imwrite(pathRGB, vFrames[i].RGB);

        //Save depth Images
        string pathDepth;
        pathDepth.append(relPath);
        pathDepth.append("/");
        pathDepth.append(nameDepth);
        pathDepth.append("/");
        pathDepth.append(incNum.str());
        pathDepth.append(".png");
        imwrite(pathDepth, vFrames[i].depthWithoutSkeleton);

        //Save player Images
        string pathPlayer;
        pathPlayer.append(relPath);
        pathPlayer.append("/");
        pathPlayer.append(namePlay);
        pathPlayer.append("/");
        pathPlayer.append(incNum.str());
        pathPlayer.append(".png");
        imwrite(pathPlayer, vFrames[i].playerIdx);

        //1 signal for each iteration (progress bar)
        emit saving();
    }

    //Save Json file---------------------------
    QJsonDocument documentJson;
    QJsonObject objectJson;
    QString pathJson = "gestures/";

    //QT File
    pathJson.append(QString::fromStdString(filename));
    pathJson.append(".json");
    QFile jsonFile(pathJson);
    jsonFile.open(QFile::WriteOnly);

    //Create content
    objectJson["Name file"] = filename.c_str();
    objectJson["Kinect Version"] = vFrames[0].kinectVersion;
    objectJson["Coordinates Path"] = pathFile.c_str();
    objectJson["RGB Path"] = dirRGB.c_str();
    objectJson["Depth Path"] = dirDepth.c_str();
    objectJson["Player Path"] = dirPlayer.c_str();
    objectJson["Video length"] = (int)vFrames.size();
    objectJson["Annotation Path"] = relPath.c_str();

    //Set object json in document
    documentJson.setObject(objectJson);
    jsonFile.write(documentJson.toJson());
    jsonFile.close();


    //-------------------------------------------
    outfile.close();

}

void GestureRecognitionSave::setFilename(string name){
    filename = name;
}

void GestureRecognitionSave::chooseFile(){

    QString filter = "*.json";
    QString filePath;
    filePath = QFileDialog::getOpenFileName(nullptr, "Select a Json file...", QString("./gestures"), filter);

    //Qt File to open json file
    QFile jsonFile(filePath);
    jsonFile.open(QFile::ReadOnly);
    QString contentJson = jsonFile.readAll();
    jsonFile.close();

    //Read a JsonDocument
    QJsonDocument documentJson = QJsonDocument::fromJson(contentJson.toUtf8());

    //Read a JsonObject
    QJsonObject objectJson = documentJson.object();

    //Read Values
    //QJsonValue valueJson = objectJson.value(QString("Name file"));
    //cout << valueJson.toString().toStdString() << endl;

    //QJsonValue valueJson = objectJson.value(QString("Video length"));
    //string ciao = valueJson.toString().toStdString();

    openData(objectJson);
}

void GestureRecognitionSave::openData(QJsonObject objectJson){

    vRGBFrames.clear();
    idxFrame = 0;

    //Read size of frames
    QJsonValue valueJson = objectJson.value(QString("Video length"));
    //string ciao = valueJson.toString().toStdString();
    int size = (valueJson.toInt());
    openingSize = size;

    //Read RGB image path
    valueJson = objectJson.value(QString("RGB Path"));
    string path = valueJson.toString().toStdString();

    //Read filename
    valueJson = objectJson.value(QString("Name file"));
    currentFilename = valueJson.toString().toStdString();

    //Read all RGB images
    for(int i=0; i<size;++i){

        //Create image name
        stringstream name;
        name << path << "/" << setfill('0') <<setw(6) << i << ".png";
        //cout << name.str() << endl;

        //Open RGB frames
        Mat3b frame = imread(name.str(), 1);
        vRGBFrames.push_back(frame);

        //Clear filename
        name.clear();

        //For the progress bar
        openingState = i;

        //Update interface
        emit opening();
    }

    emit handleFrame();
}

void GestureRecognitionSave::appendAnnotation(){

    //Only if there are user inputs
    if(begin > 0 && end > 0){
        QJsonObject objectTmpJson;
        objectTmpJson["Type Action"] = currentType.c_str();
        objectTmpJson["Begin"] = begin;
        objectTmpJson["End"] = end;
        arrayAnnotationJson.append(objectTmpJson);
    }

}

void GestureRecognitionSave::createAnnotation(){

    QString pathJson = "gestures/annotations/";

    //Only if there is something in json object
    if(arrayAnnotationJson.size() > 0){

        //QT File
        pathJson.append(QString::fromStdString(currentFilename));
        pathJson.append(".json");
        QFile jsonFile(pathJson);
        jsonFile.open(QFile::WriteOnly);

        //Append size of annotations
        QJsonObject objectSize;
        objectSize["Length"] = openingSize;
        //Include a single object
        objectAnnotationJson.insert("Length", openingSize);
        //Include an array object (with all annotations)
        objectAnnotationJson.insert("Annotations", arrayAnnotationJson);

        //Final json document
        documentAnnotationJson.setObject(objectAnnotationJson);
        jsonFile.write(documentAnnotationJson.toJson());
        jsonFile.close();

        //Clear json array
        QJsonArray tmp;
        arrayAnnotationJson = tmp;

    }
}

void GestureRecognitionSave::fwd(){
    if((idxFrame < openingSize-1) && vRGBFrames.size()>0){
        idxFrame++;
        emit handleFrame();
    }
}

void GestureRecognitionSave::fwd(int n){

    if((n < openingSize-1) && n>0){
        idxFrame = n;
        emit handleFrame();
    }
}

void GestureRecognitionSave::bck(){
    if((idxFrame > 0) && vRGBFrames.size()>0){
        idxFrame--;
        emit handleFrame();
    }
}

void GestureRecognitionSave::setDataSubmission(int lineBegin, int lineEnd, string current){

    begin = lineBegin;
    end = lineEnd;
    currentType = current;

}


