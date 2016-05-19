/*
**      Author: Tapas Kanungo, kanungo@cfar.umd.edu
**      Date:   15 December 1997
**      File:   baumwelch.c
**      Purpose: Baum-Welch algorithm for estimating the parameters
**              of a HMM model, given an observation sequence. 
**      Organization: University of Maryland
**
**	Update: 
**	Author: Tapas Kanungo
**	Date:	19 April 1999
**	Purpose: Changed the convergence criterion from ratio
**		to absolute value. 
**
**      $Id: baumwelch.c,v 1.6 1999/04/24 15:58:43 kanungo Exp kanungo $
*/

#include <stdio.h> 
#include "gmmstd_hmm_gmm.h"
#include <math.h>

namespace gmmstd{
static char rcsid[] = "$Id: baumwelch.c,v 1.6 1999/04/24 15:58:43 kanungo Exp kanungo $";


//double  CHMM::BackwardWithScale(int T, const CArray<int> &O, CArray2D<double> &beta,CArray<double> &scale)

#define DELTA 0.001 
//void CHMM_GMM::BaumWelch(const CArray<CArray<double>*> &O, CArray2D<double> &alpha, CArray2D<double> &beta,
//	CArray2D<double> &gamma, int *pniter, double *plogprobinit, double *plogprobfinal)


//void ComputeGamma(HMM *phmm, int T, double **alpha, double **beta, double **gamma)
//void CHMM_GMM::ComputeGamma(CArray2D<double> &alpha, CArray2D<double> &beta, CArray2D<double> &gamma)
void CHMM_GMM::ComputeGamma(Mat_<double> &alpha, Mat_<double> &beta, Mat_<double> &gamma)
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

	
	} // namespace