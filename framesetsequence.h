#ifndef FRAMESETSEQUENCE_H
#define FRAMESETSEQUENCE_H

#include <stdlib.h>
#include <vector>
#include "frameset.h"


/// sequenza di frameset da usare, ad esempio, per il training
/// contiene un vettore di frameset riguardante un singolo gesto
/// e la rispettiva annotazione
///
class FrameSetSequence: public std::vector<FrameSet>
{
public:
    FrameSetSequence(int idGesto = -1);

    // append di un frameset
    void AppendFrameSet(const FrameSet &frameset){
        push_back(frameset);
    }

    // set annotazione
    void SetGestureId(int id){
        _idGesto= id;
    }

    // clear
    void Clear(){
        clear();
    }

    int GetGestureId() const {return _idGesto;}

    void SetDescription(std::string descr){
        _strDescription=descr;
    }
    std::string GetDescription() const { return _strDescription;}


protected:

    //annotazione (id gesto)
    int _idGesto;

    std::string _strDescription;
};

#endif // FRAMESETSEQUENCE_H
