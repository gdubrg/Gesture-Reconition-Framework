#ifndef TESTINGSET_H
#define TESTINGSET_H

#include <vector>
#include "framesetsequence.h"

/// classe per memorizzare un intero testing set
/// contiene un vettore di framesetsequences
class TestingSet : public std::vector<FrameSetSequence>{

public:

    TestingSet();

    std::string parameter;

    //Type useful creating path
    void setTypeTest(std::string type){
        typeTest = type;
    }

    std::string getTypeTest() const{
        return typeTest;
    }

    //Annotations useful for final output file
    void addAnnotation(std::string annotation){
        annotations.push_back(annotation);
    }

    //Return vector of annotations
    std::vector<std::string> getAnnotations() const{
        return annotations;
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

    std::string typeTest;

    std::string typeFeatures;

    std::vector<std::string> annotations;



};

#endif // TESTINGSET_H
