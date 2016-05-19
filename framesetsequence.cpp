#include "framesetsequence.h"

/// sequenza di frameset da usare, ad esempio, per il training
/// contiene un vettore di frameset e le annotazioni
FrameSetSequence::FrameSetSequence(int idGesto)
{
    // init della annotazione del gesto
    _idGesto = idGesto;
}

