#ifndef TRAININGSET_H
#define TRAININGSET_H

#include <vector>
#include "framesetsequence.h"

/// classe per memorizzare un intero training set
/// contiene un vettore di framesetsequences
class TrainingSet : public  std::vector<FrameSetSequence>{

public:

    TrainingSet();

    void setTypeTrain(std::string type){
        typeTrain = type;
    }

    std::string getTypeTest() const{
        return typeTrain;
    }


    void setTypeFeatures(std::string type){
        typeFeatures = type;
    }

    std::string getTypeFeatures() const{
        return typeFeatures;
    }

    float _l;
    unsigned int _k;

protected:

    std::string typeTrain;

    std::string typeFeatures;



};

#endif // TRAININGSET_H
