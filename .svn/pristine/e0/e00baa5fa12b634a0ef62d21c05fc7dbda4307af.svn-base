#include "mainwindow.h"
// file che contiene tutti i batch process

#include "coordinatesutility.h"
#include "msrimporter.h"
#include "gesturerecognitionhmmoffline.h"
#include "gesturerecognitiondtw.h"
#include "gesturerecognitiondhmm.h"


#include "msdhmm_hmm.h"


#include <fstream>
#include <sstream>

/* MEMO
 * Caricare TrainingSet
 * typeTrain serve per sapere in quale cartella andare a salvare i classificatori (per HMM)
 *   -> attenzione al nome file di train, che di base deve essere DTWMSR/trainAS*.txt
 * Caricare TestingSet
 * typeTest serve per sapere in quale cartella andare a prendere i classificatori (per HMM)
 *   -> attenzione al nome file di test, che di base deve essere DTWMSR/testAS*.txt
 * E' possibile aggiungere delle annotazioni in test, verranno visualizzare tutte nel file di output finale
 *
 * LANCIO DA PROMPT - parametri
 * <pathfiletrain.txt> <pathfiletest.txt> [hcc] [dist] [coord] [dtw] [hmm] [train] [test]
 *  - hcc: human centered coordinates
 *  - scs: spherical coordinate system
 *  - dtw: Dinamic Time Warping
 *  - hmm: Hidden Markov Model
 *  - train: esegue la parte di train
 *  - test: esegue la parte di test (con l'ultimo train eseguito)
 *
 * DTW Features:
 *  - dist: distanza giunti
 *  - coord: semplici coordinate dei giunti
*/


void MainWindow::BatchProcess1(QStringList arguments){

    // creo un oggetto per il training da MSR
    MSRImporter importer;

    //-----------------------------TRAINING SET---------------------------------
    //string trainFilename = "DTWMSR/train_sample.txt";
    string trainFilename = arguments[1].toStdString();
    TrainingSet train = importer.CreateTrainingSet(trainFilename);
    cout << "LIST NAMES: Loaded " << train.size() << " files for training..." << endl;
    train.setTypeTrain(trainFilename.substr(12, 3));


    //-----------------------------TESTING SET---------------------------------
    //string testFilename = "DTWMSR/test_sample.txt";
    string testFilename = arguments[2].toStdString();
    TestingSet test = importer.CreateTestingSet(testFilename);
    cout << "LIST NAMES: Loaded " << test.size() << " files for testing..." << endl;
    test.setTypeTest(testFilename.substr(11, 3));
    test.addAnnotation(testFilename);
    test.addAnnotation(trainFilename);


    //-----------------------------COORDINATES-----------------------------------
    //Human Centered Coordinates
    if(arguments.contains("hcc", Qt::CaseInsensitive)){
        humanCenteredCoord(train);
        humanCenteredCoord(test);
        test.addAnnotation("Human Centered Coordinates");
    }

    //Spherical Coordinate System
    if(arguments.contains("scs", Qt::CaseInsensitive)){
        sphericalCoord(train);
        sphericalCoord(test);
        test.addAnnotation("Spherical Coordinate System");
    }

    //Feature Normalization
    if(arguments.contains("norm", Qt::CaseInsensitive)){
        centreAndNormalize(train);
        centreAndNormalize(test);
        test.addAnnotation("Centre and Normalize");
    }


    //-------------------------------FEATURES-------------------------------------
    if(arguments.contains("dtw", Qt::CaseInsensitive)){

        if(arguments.contains("coord", Qt::CaseInsensitive)){
            train.setTypeFeatures("coord");
            test.setTypeFeatures("coord");
            test.addAnnotation("Features: coordinate giunti");
        }

        else if(arguments.contains("dist", Qt::CaseInsensitive)){
            train.setTypeFeatures("dist");
            test.setTypeFeatures("dist");
            test.addAnnotation("Features: distanze coppie di giunti");
        }
    }
    if(arguments.contains("hmm", Qt::CaseInsensitive)){

    }

    //rendo calcolo feature modulare (riusabilità), nei metodi lascio solo la decodifica
    //fare classe o basta fila di metodi?
    //ci metto dentro anche la quantizzazione (del resto sono feature)
    //if(arguments.contains("some_feature", Qt::CaseInsensitive)){
    //getSomeFeatures(train, test);
    //test.addAnnotation("some feature")
    //if(...)
    //...
    //}


    //------------------------------------DTW--------------------------------------------
    if(arguments.contains("dtw", Qt::CaseInsensitive)){
        test.addAnnotation("DTW");
        _precognizer = new GestureRecognitionDTW();
        GestureRecognitionDTW *pgestureRec = dynamic_cast<GestureRecognitionDTW *> (this->_precognizer);
        if (pgestureRec != nullptr){

            //TRAIN
            if(arguments.contains("train", Qt::CaseInsensitive))
            pgestureRec->Train(train);

            //TEST
            if(arguments.contains("test", Qt::CaseInsensitive))
            pgestureRec->Test(test);

            exit(0);
        }
        else
            cout << "Nessun recognizer definito!" << endl;
    }


    //-----------------------------------CONTINUOUS HMM--------------------------------------------
    if(arguments.contains("hmm", Qt::CaseInsensitive)){
        test.addAnnotation("HMM");
        _precognizer = new GestureRecognitionHMMOffline();
        GestureRecognitionHMMOffline *pgestureRec = dynamic_cast<GestureRecognitionHMMOffline*> (this->_precognizer);
        if (pgestureRec != nullptr){

            //TRAIN
            if(arguments.contains("train", Qt::CaseInsensitive))
                pgestureRec->Train(train);

            //TEST
            if(arguments.contains("test", Qt::CaseInsensitive))
                pgestureRec->Test(test);

            exit(0);
        }
        else
            cout << "Nessun recognizer definito!" << endl;
    }


    //-----------------------------------DISCRETE HMM--------------------------------------------
    if(arguments.contains("dhmm", Qt::CaseInsensitive)){
        test.addAnnotation("DISCRETE HMM");
        _precognizer = new GestureRecognitionDHMM();
        GestureRecognitionDHMM *pgestureRec = dynamic_cast<GestureRecognitionDHMM*> (this->_precognizer);
        if (pgestureRec != nullptr){

            train._l = arguments[3].toDouble();
            train._k = arguments[4].toInt();
            test._l = arguments[3].toDouble();
            test._k = arguments[4].toInt();
            test.addAnnotation("l: " + arguments[3].toStdString());
            test.addAnnotation("k: " + arguments[4].toStdString());

            //TRAIN
            if(arguments.contains("train", Qt::CaseInsensitive))
                pgestureRec->Train(train);

            //TEST
            if(arguments.contains("test", Qt::CaseInsensitive))
                pgestureRec->Test(test);

            exit(0);
        }
        else
            cout << "Nessun recognizer definito!" << endl;
    }


}

