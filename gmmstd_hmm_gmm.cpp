/*
**      Author: Tapas Kanungo, kanungo@cfar.umd.edu
**      Date:   15 December 1997
**      File:   hmm.h
**      Purpose: datastructures used for HMM. 
**      Organization: University of Maryland
**
**	Update:
**	Author: Tapas Kanungo
**	Purpose: include <math.h>. Not including this was
**		creating a problem with forward.c
**      $Id: hmm.h,v 1.9 1999/05/02 18:38:11 kanungo Exp kanungo $
**
**  Modified by Roberto Vezzani
**  to be compliant with the Imagelab Library
*/
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include "gmmstd_hmm_gmm.h"


namespace gmmstd{
// classe per gli HMM che usano GMM come probabilità di emissione


CHMM_GMM::CHMM_GMM(unsigned int N, unsigned  int M, unsigned int K)
		:	m_A(N,N,0.),
			m_pi(N,1,0.),
			m_B(N),
			m_final(N,1,0.)
		{
	
			m_iN = N;
			m_iM = M;
			m_iK = K;

			// HMM generico
			m_bDiagonalCovariance = true;  // altrim ciocca
			m_bLeftRight = false;
	
		
			m_dDurationMean=0;
			m_dDurationVariance=0;


			// resize dei modelli B
			unsigned int i;
			for (i=0; i<m_iN; i++)
			{
				m_B[i].ReSize(m_iM);  // dimensione feature
				m_B[i].SetGaussiansNumber(m_iK);  // numero di gaussiane fisso
			}

			// todo: impostare valori iniziali!!!
			fill(m_pi.begin(),m_pi.end(), 1.0/m_iN);
			fill(m_final.begin(),m_final.end(), 1.0/m_iN);

			inst = vector<int>(8, 0);

		}   
	




	// random initialization
	bool CHMM_GMM::RandomInit(){

		int i,j;
		double dVal,dSum=0;
		
		// probabilità iniziali: random se non left-right
		if (!m_bLeftRight){

			for (i=0; i<m_iN; i++){
				dVal  =random_Uniform(0,1);
				dSum+=dVal;
				m_pi(i,0)=dVal;
			}
			for (i=0; i<m_iN; i++){
				m_pi(i,0)/=dSum;
			}
			m_final = 1.0 / m_iN;

		}
		else{

			// solo il primo stato ha probabilità uguale ad 1
			m_pi(0,0)=1;
			for (i=1; i<m_iN; i++){
				m_pi(i,0)=0;
			}

			// solo lo stato finale ha probabilità uguale ad 1
			m_final(m_iN-1,0)=1;
			for (i=0; i<m_iN-1; i++){
				m_pi(i,0)=0;
			}

		}

		//stessa cosa per la matrice A
		//m_A.SetAllElems(1.0/m_iN);
		for (i=0; i<m_iN; i++){

			// se left-right devo settare solamente le auto-transizioni e gli avanzamenti di 1
			dSum=0;
			if (m_bLeftRight){
				for (j=0; j<m_iN; j++){
					if (i==j || j==i+1){
						dVal  =random_Uniform(0,1);
						dSum+=dVal;
						m_A(i,j)=dVal;
					}
					else
						m_A(i,j)=0;
				}

			}
			else			{
				for (j=0; j<m_iN; j++){
					dVal  =random_Uniform(0,1);
					dSum+=dVal;
					m_A(i,j)=dVal;
				}
			}

			//normalizzo
			for (j=0; j<m_iN; j++)
				m_A(i,j)/=dSum;
		}

		// matrice B: distribuzioni random
		for (i=0; i<m_iN; i++)
			m_B[i].RandomInit();

		m_dDurationMean=0;
		m_dDurationVariance=0;


		return true;
	}





	// leggo e scrivo su file
	bool CHMM_GMM::LoadFromFile (FILE *f){
		// numeri vari
			fread(&m_iN,sizeof(unsigned int),1,f);
			fread(&m_iM,sizeof(unsigned int),1,f);
			fread(&m_iK,sizeof(unsigned int),1,f);

			fread(&m_dDurationMean,sizeof(double),1,f);
			fread(&m_dDurationVariance,sizeof(double),1,f);

			fread(&m_bDiagonalCovariance,sizeof(bool),1,f);
			

			// resize
			m_A.create(m_iN,m_iN);
			m_pi.create(m_iN,1);
			m_B.resize(m_iN);

			m_final.create(m_iN,1);
			m_final = 1.0 / m_iN;

			m_dThreshold = 0;


			//vettore pi
			unsigned int i;
			for (i=0; i<m_iN; i++)
				fread(&(m_pi(i,0)),sizeof(double),1,f);

			//gmm
			for (i=0; i<m_iN; i++)
				m_B[i].LoadFromFile(f);

			//matrice A
			unsigned int j;
			for (i=0; i<m_iN; i++)
				for (j=0; j<m_iN; j++)
					fread(&(m_A(i,j)),sizeof(double),1,f);

			// lo metto in fondo per compatibilità
			fread(&m_bLeftRight,sizeof(bool),1,f);

			// caricamento del vettore final
			for (i=0; i<m_iN; i++)
				fread(&(m_final(i,0)),sizeof(double),1,f);

			fread(&(m_dThreshold),sizeof(double),1,f);

			fread(&(m_bThreshold_normalized),sizeof(bool),1,f);
			fread(&(m_bThreshold_alphat),sizeof(bool),1,f);
			fread(&(m_bThreshold_length),sizeof(bool),1,f);

			return true;
	}



	bool CHMM_GMM::SaveToFile (FILE *f){
		// numeri vari
			fwrite(&m_iN,sizeof(unsigned int),1,f);
			fwrite(&m_iM,sizeof(unsigned int),1,f);
			fwrite(&m_iK,sizeof(unsigned int),1,f);

			fwrite(&m_dDurationMean,sizeof(double),1,f);
			fwrite(&m_dDurationVariance,sizeof(double),1,f);


			fwrite(&m_bDiagonalCovariance,sizeof(bool),1,f);
			

			//vettore pi
			unsigned int i;
			for (i=0; i<m_iN; i++)
				fwrite(&(m_pi(i,0)),sizeof(double),1,f);

			//gmm
			for (i=0; i<m_iN; i++)
				m_B[i].SaveToFile(f);

			//matrice A
			unsigned int j;
			for (i=0; i<m_iN; i++)
				for (j=0; j<m_iN; j++)
					fwrite(&(m_A(i,j)),sizeof(double),1,f);

			fwrite(&m_bLeftRight,sizeof(bool),1,f);

			// salvataggio del vettore final
			for (i=0; i<m_iN; i++)
				fwrite(&(m_final(i,0)),sizeof(double),1,f);

			fwrite(&(m_dThreshold),sizeof(double),1,f);

			fwrite(&(m_bThreshold_normalized),sizeof(bool),1,f);
			fwrite(&(m_bThreshold_alphat),sizeof(bool),1,f);
			fwrite(&(m_bThreshold_length),sizeof(bool),1,f);

			return true;
	}


	bool CHMM_GMM::LoadFromFile (const char* strFileName){

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

	bool CHMM_GMM::SaveToFile (const char* strFileName){
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



	// calcolo della probabilità della lunghezza della osservazione
	// Contiene due termini, uno dovuto alla lunghezza, uno dovuto alla "affidabilità"
	// sono entrambi termini calcolati come cumulative di gaussiane

	

	double CHMM_GMM::LogLikelihoodOfDuration(int T){
		double dProbLung=0;
		double dVariance;
		
		if (m_dDurationVariance>0) 
			dVariance = m_dDurationVariance;
		else
			dVariance  = 10; // default value
		//	termine su lunghezza
		dProbLung = 1.0-cumGauss(m_dDurationMean,dVariance,(double)T);

		// termine su affidabilità -> fissato a priori... arg
		dProbLung *= cumGauss(15.0,50.0,(double)T);
		//dProbLung = -( log(2.0 * PI * 25)/2.0 ) - ((50-T)*(50-T)/ (2.0 * 25));

		dProbLung  = log(dProbLung);
	
		return dProbLung;
	}
	
	} // namespace