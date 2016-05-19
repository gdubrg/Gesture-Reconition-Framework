#include "getfeatures.h"

GetFeatures::GetFeatures(){

}

void GetFeatures::createTrainFeatures(TrainingSet& train, TestingSet& test){

    TrainingSet newTrainSet;
    TestingSet newTestSet;


    //TRAIN FEATURES

    //For each action
    for(auto it = train.begin(); it != train.end(); ++it){

        //For each frame
        for (auto itframeset = it->begin(); itframeset != it->end(); ++itframeset ){

        }
    }

}
