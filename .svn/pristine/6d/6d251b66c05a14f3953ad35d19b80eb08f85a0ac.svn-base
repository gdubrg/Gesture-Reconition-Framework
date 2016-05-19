#include "msrimporter.h"

#include "trainingset.h"
#include <string>
#include <QString>
#include <QTextStream>
#include <QFile>


using namespace std;
using namespace cv;

#define JOINTSINSKELETON 20

enum jointNamesMSR{
    MSR_shoulderRight,
    MSR_shoulderLeft,
    MSR_shoulderCenter,
    MSR_spine,
    MSR_hipRight,
    MSR_hipLeft,
    MSR_hipCenter,
    MSR_elbowRight,
    MSR_elbowLeft,
    MSR_wristRight,
    MSR_wristLeft,
    MSR_handRight,
    MSR_handLeft,
    MSR_kneeRight,
    MSR_kneeLeft,
    MSR_ankleRight,
    MSR_ankleLeft,
    MSR_footRight,
    MSR_footLeft,
    MSR_head
};

MSRImporter::MSRImporter(){

}

FrameSetSequence LoadFrameSetSeqFromMSR(std::string strfilename, std::string strfilenameRGB){
    FrameSetSequence fs;

    fs.SetDescription(strfilename);

    //Open file REAL coordinates
    QFile file (strfilename.data());
    file.open(QIODevice::ReadOnly);
    QString str;

    //Read file content
    QTextStream ts (&file);

    //Open file SCREEN coordinates
    QFile fileRGB (strfilenameRGB.data());
    fileRGB.open(QIODevice::ReadOnly);
    QString strRGB;

    QTextStream tsRGB (&fileRGB);

    bool bInitOld = false;
    tripletPoint oldscheletro [JOINTSINSKELETON];
    tripletPoint scheletro [JOINTSINSKELETON];
    tripletPoint skeletonRGB [JOINTSINSKELETON];
    int idGiunto=0;


    // inizialmente la sequenza non è valida
    bool bValid=false;

    while (!ts.atEnd())
    {
        str = ts.readLine();
        strRGB = tsRGB.readLine();

        if (str.length()>0 && strRGB.length()>0){

            // ho letto una riga non nulla
            QStringList strList = str.split(" ");
            scheletro[idGiunto].x = strList[0].toDouble();
            scheletro[idGiunto].y = strList[1].toDouble();
            scheletro[idGiunto].z = strList[2].toDouble();

            QStringList strListRGB = strRGB.split(" ");
            skeletonRGB[idGiunto].x = strListRGB[2].toDouble();
            skeletonRGB[idGiunto].y = strListRGB[4].toDouble();
            skeletonRGB[idGiunto].z = strListRGB[6].toDouble();

            idGiunto++;

            // se ho completato uno scheletro
            if (idGiunto == JOINTSINSKELETON){
                // creo un frameset
                FrameSet frameset;
                frameset.kinectVersion = 1;

                //Joint in REAL coordinates conversion
                frameset.bodyJoint[FS_head] = scheletro[MSR_head];
                frameset.bodyJoint[FS_shoulderCenter] = scheletro[MSR_shoulderCenter];

                frameset.bodyJoint[FS_shoulderRight] = scheletro[MSR_shoulderRight];
                frameset.bodyJoint[FS_elbowRight] = scheletro[MSR_elbowRight];
                frameset.bodyJoint[FS_wristRight] = scheletro[MSR_wristRight];
                frameset.bodyJoint[FS_handRight] = scheletro[MSR_handRight];

                frameset.bodyJoint[FS_shoulderLeft] = scheletro[MSR_shoulderLeft];
                frameset.bodyJoint[FS_elbowLeft] = scheletro[MSR_elbowLeft];
                frameset.bodyJoint[FS_wristLeft] = scheletro[MSR_wristLeft];
                frameset.bodyJoint[FS_handLeft] = scheletro[MSR_handLeft];

                frameset.bodyJoint[FS_spine] = scheletro[MSR_spine];
                frameset.bodyJoint[FS_hipCenter] = scheletro[MSR_hipCenter];

                frameset.bodyJoint[FS_hipRight] = scheletro[MSR_hipRight];
                frameset.bodyJoint[FS_kneeRight] = scheletro[MSR_kneeRight];
                frameset.bodyJoint[FS_ankleRight] = scheletro[MSR_ankleRight];
                frameset.bodyJoint[FS_footRight] = scheletro[MSR_footRight];

                frameset.bodyJoint[FS_hipLeft] = scheletro[MSR_hipLeft];
                frameset.bodyJoint[FS_kneeLeft] = scheletro[MSR_kneeLeft];
                frameset.bodyJoint[FS_ankleLeft] = scheletro[MSR_ankleLeft];
                frameset.bodyJoint[FS_footLeft] = scheletro[MSR_footLeft];


                //Joint in SCREEN coordinates conversion
                frameset.bodyJointRGB[FS_head] = skeletonRGB[MSR_head];
                frameset.bodyJointRGB[FS_shoulderCenter] = skeletonRGB[MSR_shoulderCenter];

                frameset.bodyJointRGB[FS_shoulderRight] = skeletonRGB[MSR_shoulderRight];
                frameset.bodyJointRGB[FS_elbowRight] = skeletonRGB[MSR_elbowRight];
                frameset.bodyJointRGB[FS_wristRight] = skeletonRGB[MSR_wristRight];
                frameset.bodyJointRGB[FS_handRight] = skeletonRGB[MSR_handRight];

                frameset.bodyJointRGB[FS_shoulderLeft] = skeletonRGB[MSR_shoulderLeft];
                frameset.bodyJointRGB[FS_elbowLeft] = skeletonRGB[MSR_elbowLeft];
                frameset.bodyJointRGB[FS_wristLeft] = skeletonRGB[MSR_wristLeft];
                frameset.bodyJointRGB[FS_handLeft] = skeletonRGB[MSR_handLeft];

                frameset.bodyJointRGB[FS_spine] = skeletonRGB[MSR_spine];
                frameset.bodyJointRGB[FS_hipCenter] = skeletonRGB[MSR_hipCenter];

                frameset.bodyJointRGB[FS_hipRight] = skeletonRGB[MSR_hipRight];
                frameset.bodyJointRGB[FS_kneeRight] = skeletonRGB[MSR_kneeRight];
                frameset.bodyJointRGB[FS_ankleRight] = skeletonRGB[MSR_ankleRight];
                frameset.bodyJointRGB[FS_footRight] = skeletonRGB[MSR_footRight];

                frameset.bodyJointRGB[FS_hipLeft] = skeletonRGB[MSR_hipLeft];
                frameset.bodyJointRGB[FS_kneeLeft] = skeletonRGB[MSR_kneeLeft];
                frameset.bodyJointRGB[FS_ankleLeft] = skeletonRGB[MSR_ankleLeft];
                frameset.bodyJointRGB[FS_footLeft] = skeletonRGB[MSR_footLeft];

                //Store the loaded frameset
/*
                // check
                if (!bInitOld)
                {

                    for (int i=0; i<JOINTSINSKELETON; i++)
                        oldscheletro[i]=scheletro[i];
                    bInitOld = true;

                }
                else
                {
                    // controllo se inizia validità
                    if (!bValid){
                        // calcolo distanza totale
                        double dDist = 0;

                        for (int i=0; i<JOINTSINSKELETON; i++)
                        {
                            dDist+= fabs(scheletro[i].x-oldscheletro[i].x)+
                                    fabs(scheletro[i].y-oldscheletro[i].y)+
                                    fabs(scheletro[i].z-oldscheletro[i].z);
                        }

                        if (dDist>0.2) // todo: soglia!?!?!?
                            bValid=true;

                    }
                }
*/
                // da inserire per usare un valore frame by frame

//                  for (int i=0; i<JOINTSINSKELETON; i++)
//                    oldscheletro[i]=scheletro[i];





                // if valid then
               // if (bValid)
                fs.push_back(frameset);

                idGiunto  = 0;
            }
        }
    }
    return fs;
}

TrainingSet MSRImporter::CreateTrainingSet(string filenameTrain){

    // devo creare un training set
    TrainingSet trainingSet;

    // carico/creo la lista di video di training
    std::vector<std::string> listFileName;
    std::vector<std::string> listFilenameRGB;

    std::string strBasePath;
    strBasePath.append("E:\\Google Drive\\Progetti\\MSR Action3D\\Real\\");
    std::string strBasePathRGB;
    strBasePathRGB.append("E:\\Google Drive\\Progetti\\MSR Action3D\\Screen\\");

    std::string strFilename;
    std::string strFilenameRGB;

    std::vector<int> listAnnotations;
    int iId;

    //--------------------------------------Real coordinates------------------------------
    QFile file (filenameTrain.data());
    file.open(QIODevice::ReadOnly);
    QString str;
    //Read file content
    QTextStream ts (&file);
    //read and store all lines (filenames)
    while (!ts.atEnd())
    {
        str = ts.readLine();

        if (str.length()>0){
            //Convert from QString to string
            strFilename = str.toStdString();
            //Store path + filename
            listFileName.push_back(strBasePath + strFilename);
            //Retrieve action id
            iId = atoi(strFilename.substr(1,2).data());
            //Store action id
            listAnnotations.push_back(iId);

            //RGB coordinates
            strFilenameRGB = strFilename.substr(0, 20);
            strFilenameRGB.append(".txt");
            listFilenameRGB.push_back(strBasePathRGB + strFilenameRGB);
        }
    }

    //--------------------------------------Screen coordinates-----------------------------

    // per ogni video
    auto itId = listAnnotations.begin();
    auto it = listFileName.begin();
    auto itRGB = listFilenameRGB.begin();
    for (;it != listFileName.end();++it, ++itId, ++itRGB)
    {
        // recupero il nome del file e l'annotazione
        std::string strfilename;
        std::string strfilenameRGB;
        int idAnnotation;
        strfilename = *it;
        idAnnotation = *itId;
        strfilenameRGB = *itRGB;

        // carico il file da MSR
        FrameSetSequence fs = LoadFrameSetSeqFromMSR(strfilename, strfilenameRGB);
        fs.SetGestureId(idAnnotation);
        trainingSet.push_back(fs);

    }

    return trainingSet;

}

TestingSet MSRImporter::CreateTestingSet(string filenameTest){

    // devo creare un testing set
    TestingSet testingSet;

    testingSet.parameter = filenameTest;

    // carico/creo la lista di video di training
    std::vector<std::string> listFileName;
    std::vector<std::string> listFilenameRGB;

    std::string strBasePath;
    strBasePath.append("E:\\Google Drive\\Progetti\\MSR Action3D\\Real\\");
    std::string strBasePathRGB;
    strBasePathRGB.append("E:\\Google Drive\\Progetti\\MSR Action3D\\Screen\\");

    std::string strFilename;
    std::string strFilenameRGB;

    std::vector<int> listAnnotations;
    int iId;

    //--------------------------------------Real coordinates------------------------------
    QFile file (filenameTest.data());
    file.open(QIODevice::ReadOnly);
    QString str;
    //Read file content
    QTextStream ts (&file);
    //read and store all lines (filenames)
    while (!ts.atEnd())
    {
        str = ts.readLine();

        if (str.length()>0){
            //Convert from QString to string
            strFilename = str.toStdString();
            //Store path + filename
            listFileName.push_back(strBasePath + strFilename);
            //Retrieve action id
            iId = atoi(strFilename.substr(1,2).data());
            //Store action id
            listAnnotations.push_back(iId);

            //RGB coordinates
            strFilenameRGB = strFilename.substr(0, 20);
            strFilenameRGB.append(".txt");
            listFilenameRGB.push_back(strBasePathRGB + strFilenameRGB);
        }
    }

    //--------------------------------------Screen coordinates-----------------------------

    // per ogni video
    auto itId = listAnnotations.begin();
    auto it = listFileName.begin();
    auto itRGB = listFilenameRGB.begin();
    for (;it != listFileName.end();++it, ++itId, ++itRGB)
    {
        // recupero il nome del file e l'annotazione
        std::string strfilename;
        std::string strfilenameRGB;
        int idAnnotation;
        strfilename = *it;
        idAnnotation = *itId;
        strfilenameRGB = *itRGB;

        // carico il file da MSR
        FrameSetSequence fs = LoadFrameSetSeqFromMSR(strfilename, strfilenameRGB);
        fs.SetGestureId(idAnnotation);
        testingSet.push_back(fs);

    }

    return testingSet;
}

