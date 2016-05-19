#include "msdhmm_hmm.h"


// Cmsdhmm_hmm (unsigned int N, vector<unsigned int> quantvector);
Cmsdhmm_hmm::Cmsdhmm_hmm (unsigned int N, vector<unsigned int> quantlevels, bool bLeftRight, int iDeltaSteps )
        :	m_iN (N),
            m_A(N,N,0.),
            m_pi(N,1,0.),
            m_B(N),
            m_final(N,1,0.),
            m_quantlevels(quantlevels)
        {

            m_bLeftRight = bLeftRight;
            m_iDeltaSteps=iDeltaSteps;

            // sistemo i modelli B
            unsigned int i;
            for (i=0; i<m_iN; i++)
            {
                m_B[i].Init(m_quantlevels);
            }

            // todo: impostare valori iniziali!!!
            fill(m_pi.begin(),m_pi.end(), 1.0/m_iN);
            fill(m_final.begin(),m_final.end(), 1.0/m_iN);

        }


void Cmsdhmm_hmm::ResetB(){
    // azzero gli istogrammi. Attenzione con i pesi!!!!
    int iState;
    for (iState =0; iState<m_iN; iState++)
        m_B[iState].Reset();

}


bool Cmsdhmm_hmm::Init_Random(){
    return false;

}



void Cmsdhmm_hmm::ComputeGamma(Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &gamma)
{
    unsigned int 	i, j;
    unsigned int	t;
    double	denominator;

    unsigned int T;
        T= alpha.rows;


    for (t = 0; t < T; t++) {
        denominator = 0.0;
        for (j = 0; j < m_iN; j++) {
            gamma(t,j) = alpha(t,j)*beta(t,j);
            denominator += gamma(t,j);
        }

        assert (denominator);
        for (i = 0; i < m_iN; i++)
            gamma(t,i) = gamma(t,i)/denominator;
    }
}

bool Cmsdhmm_hmm::SaveToFile(FILE *f){

    // numeri vari
    fwrite(&m_iN,sizeof(unsigned int),1,f);

    //quantlevel
    int tmp = m_quantlevels.size();
    fwrite(&tmp, sizeof(int), 1, f);

    unsigned int i;
    for(i=0; i<tmp; ++i){
        fwrite(&m_quantlevels[i], sizeof(unsigned int), 1, f);
    }

    //vettore pi
    for (i=0; i<m_iN; i++)
        fwrite(&(m_pi(i,0)),sizeof(double),1,f);

    //dprob
    for (i=0; i<m_iN; i++)
        m_B[i].SaveToFile(f);

    //matrice A
    unsigned int j;
    for (i=0; i<m_iN; i++)
        for (j=0; j<m_iN; j++)
            fwrite(&(m_A(i,j)),sizeof(double),1,f);

    //bool
    fwrite(&m_bLeftRight,sizeof(bool),1,f);
    fwrite(&m_iDeltaSteps, sizeof(int), 1, f);

    // pesi
    for (i=0; i<tmp; i++)
        fwrite(&(_dweigths[i]),sizeof(double),1,f);

    // salvataggio del vettore final
    for (i=0; i<m_iN; i++)
        fwrite(&(m_final(i,0)),sizeof(double),1,f);


    return true;

}

bool Cmsdhmm_hmm::SaveToFile (const char* strFileName){
    // provo ad aprire il file in scrittura
    FILE *f;
    bool ris;
    f = fopen(strFileName,"wb+");
    if (!f) return false;
    ris = SaveToFile(f);
    // chiudo il file
    fclose(f);
    return ris;
}

bool Cmsdhmm_hmm::LoadFromFile (const char* strFileName){

    // provo ad aprire il file
    FILE *f;
    bool ris;
            f = fopen(strFileName,"rb");
    if (!f) return false;
    ris = LoadFromFile(f);
    // chiudo il file
    fclose(f);
    return ris;
}

bool Cmsdhmm_hmm::LoadFromFile (FILE *f){
    // numeri vari
        fread(&m_iN,sizeof(unsigned int),1,f);;

        //quantlevel
        int tmp;
        fread(&tmp, sizeof(int), 1, f);

        unsigned int i;
        m_quantlevels.resize(tmp);
        for(i=0; i<tmp; ++i){
            fread(&m_quantlevels[i], sizeof(unsigned int), 1, f);
        }


        // resize
        m_A.create(m_iN,m_iN);
        m_pi.create(m_iN,1);
        m_B.resize(m_iN);

        m_final.create(m_iN,1);
        m_final = 1.0 / m_iN;
        _dweigths.resize(tmp,1);


        //vettore pi
        for (i=0; i<m_iN; i++)
            fread(&(m_pi(i,0)),sizeof(double),1,f);

        //B
        unsigned int k;
        for (i=0; i<m_iN; i++)
                m_B[i].LoadFromFile(f);

        //matrice A
        unsigned int j;
        for (i=0; i<m_iN; i++)
            for (j=0; j<m_iN; j++)
                fread(&(m_A(i,j)),sizeof(double),1,f);

        // lo metto in fondo per compatibilit�
        fread(&m_bLeftRight,sizeof(bool),1,f);
        fread(&m_iDeltaSteps, sizeof(int), 1, f);

        // pesi
        for (i=0; i<tmp; i++)
            fread(&(_dweigths[i]),sizeof(double),1,f);

        //todo: controllare pesi singoli...
        for (i=0; i<m_iN; i++)
                m_B[i].SetWeights(_dweigths);


        // caricamento del vettore final
        for (i=0; i<m_iN; i++)
            fread(&(m_final(i,0)),sizeof(double),1,f);

        return true;
}
