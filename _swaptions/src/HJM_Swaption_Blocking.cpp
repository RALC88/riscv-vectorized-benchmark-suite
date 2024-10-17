//HJM_Swaption_Blocking.cpp
//Routines to compute various security prices using HJM framework (via Simulation).
//Authors: Mark Broadie, Jatin Dewanwala
//Collaborator: Mikhail Smelyanskiy, Intel, Jike Chong (Berkeley)

/*************************************************************************
* RISC-V Vectorized Version
* Author: Cristóbal Ramírez Lazo
* email: cristobal.ramirez@bsc.es
* Barcelona Supercomputing Center (2020)
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nr_routines.h"
#include "HJM_Securities.h"
#include "HJM.h"
#include "HJM_type.h"

#include <time.h>
#include <sys/time.h>


int HJM_Swaption_Blocking(FTYPE *pdSwaptionPrice, //Output vector that will store simulation results in the form:
			  //Swaption Price
			  //Swaption Standard Error
			  //Swaption Parameters 
			  FTYPE dStrike,				  
			  FTYPE dCompounding,     //Compounding convention used for quoting the strike (0 => continuous,
			  //0.5 => semi-annual, 1 => annual).
			  FTYPE dMaturity,	      //Maturity of the swaption (time to expiration)
			  FTYPE dTenor,	      //Tenor of the swap
			  FTYPE dPaymentInterval, //frequency of swap payments e.g. dPaymentInterval = 0.5 implies a swap payment every half
			  //year
			  //HJM Framework Parameters (please refer HJM.cpp for explanation of variables and functions)
			  int iN,						
			  int iFactors, 
			  FTYPE dYears, 
			  FTYPE *pdYield, 
			  FTYPE **ppdFactors,
			  //Simulation Parameters
			  long *iRndSeed, 
			  long lTrials,
			  int BLOCKSIZE, int tid)
  
{
  int iSuccess = 0;
  int i; 
  int b; //block looping variable
  long l; //looping variables
  
  FTYPE ddelt = (FTYPE)(dYears/iN);				//ddelt = HJM matrix time-step width. e.g. if dYears = 5yrs and
                                                                //iN = no. of time points = 10, then ddelt = step length = 0.5yrs
  int iFreqRatio = (int)(dPaymentInterval/ddelt + 0.5);		// = ratio of time gap between swap payments and HJM step-width.
                                                                //e.g. dPaymentInterval = 1 year. ddelt = 0.5year. This implies that a swap
                                                                //payment will be made after every 2 HJM time steps.
  
  FTYPE dStrikeCont;				//Strike quoted in continuous compounding convention. 
                                                //As HJM rates are continuous, the K in max(R-K,0) will be dStrikeCont and not dStrike.
  if(dCompounding==0) {
    dStrikeCont = dStrike;		//by convention, dCompounding = 0 means that the strike entered by user has been quoted
                                        //using continuous compounding convention
  } else {
    //converting quoted strike to continuously compounded strike
    dStrikeCont = (1/dCompounding)*log(1+dStrike*dCompounding);  
  }
                                         //e.g., let k be strike quoted in semi-annual convention. Therefore, 1$ at the end of
                                         //half a year would earn = (1+k/2). For converting to continuous compounding, 
                                         //(1+0.5*k) = exp(K*0.5)
                                         // => K = (1/0.5)*ln(1+0.5*k)
  
  //HJM Framework vectors and matrices
  int iSwapVectorLength;  // Length of the HJM rate path at the time index corresponding to swaption maturity.

  FTYPE **ppdHJMPath;    // **** per Trial data **** //

  FTYPE *pdForward;
  FTYPE **ppdDrifts; 
  FTYPE *pdTotalDrift;
  
  // *******************************
  // ppdHJMPath = dmatrix(0,iN-1,0,iN-1);
  ppdHJMPath = dmatrix(0,iN-1,0,iN*BLOCKSIZE-1);    // **** per Trial data **** //
  pdForward = dvector(0, iN-1);
  ppdDrifts = dmatrix(0, iFactors-1, 0, iN-2);
  pdTotalDrift = dvector(0, iN-2);
	
  //==================================
  // **** per Trial data **** //
  FTYPE *pdDiscountingRatePath;	  //vector to store rate path along which the swaption payoff will be discounted
  FTYPE *pdPayoffDiscountFactors;  //vector to store discount factors for the rate path along which the swaption 
  //payoff will be discounted
  FTYPE *pdSwapRatePath;			  //vector to store the rate path along which the swap payments made will be discounted	
  FTYPE *pdSwapDiscountFactors;	  //vector to store discount factors for the rate path along which the swap
  //payments made will be discounted	
  FTYPE *pdSwapPayoffs;			  //vector to store swap payoffs

  
  int iSwapStartTimeIndex;
  int iSwapTimePoints;
  FTYPE dSwapVectorYears;

  FTYPE dSwaptionPayoff;
  FTYPE dDiscSwaptionPayoff;
  FTYPE dFixedLegValue;

  // Accumulators
  FTYPE dSumSimSwaptionPrice; 
  FTYPE dSumSquareSimSwaptionPrice;

  // Final returned results
  FTYPE dSimSwaptionMeanPrice;
  FTYPE dSimSwaptionStdError;
  
  // *******************************
  pdPayoffDiscountFactors = dvector(0, iN*BLOCKSIZE-1);
  pdDiscountingRatePath = dvector(0, iN*BLOCKSIZE-1);
  // *******************************
  
  iSwapVectorLength = (int) (iN - dMaturity/ddelt + 0.5);	//This is the length of the HJM rate path at the time index
  //printf("iSwapVectorLength = %d\n",iSwapVectorLength);
  //corresponding to swaption maturity.
  // *******************************
  pdSwapRatePath = dvector(0, iSwapVectorLength*BLOCKSIZE - 1);
  pdSwapDiscountFactors  = dvector(0, iSwapVectorLength*BLOCKSIZE - 1);
  // *******************************
  pdSwapPayoffs = dvector(0, iSwapVectorLength - 1);


  iSwapStartTimeIndex = (int) (dMaturity/ddelt + 0.5);	//Swap starts at swaption maturity
  iSwapTimePoints = (int) (dTenor/ddelt + 0.5);			//Total HJM time points corresponding to the swap's tenor
  dSwapVectorYears = (FTYPE) (iSwapVectorLength*ddelt);



  //now we store the swap payoffs in the swap payoff vector
  for (i=0;i<=iSwapVectorLength-1;++i)
    pdSwapPayoffs[i] = 0.0; //initializing to zero
  for (i=iFreqRatio;i<=iSwapTimePoints;i+=iFreqRatio)
    {
      if(i != iSwapTimePoints)
	pdSwapPayoffs[i] = exp(dStrikeCont*dPaymentInterval) - 1; //the bond pays coupon equal to this amount
      if(i == iSwapTimePoints)
	pdSwapPayoffs[i] = exp(dStrikeCont*dPaymentInterval); //at terminal time point, bond pays coupon plus par amount
    }

  //generating forward curve at t=0 from supplied yield curve
  iSuccess = HJM_Yield_to_Forward(pdForward, iN, pdYield);
  if (iSuccess!=1)
    return iSuccess;
  
  //computation of drifts from factor volatilities
  iSuccess = HJM_Drifts(pdTotalDrift, ppdDrifts, iN, iFactors, dYears, ppdFactors);
  if (iSuccess!=1)
    return iSuccess;
  
  dSumSimSwaptionPrice = 0.0;
  dSumSquareSimSwaptionPrice = 0.0;

  //printf("lTrials = %d\n",lTrials);
  //printf("BLOCKSIZE = %d\n",BLOCKSIZE);
  //Simulations begin:
  for (l=0;l<=lTrials-1;l+=BLOCKSIZE) {

      int BLOCKSIZE_AUX= (BLOCKSIZE<(lTrials-l)) ? BLOCKSIZE:lTrials-l;
      //For each trial a new HJM Path is generated
      iSuccess = HJM_SimPath_Forward_Blocking(ppdHJMPath, iN, iFactors, dYears, pdForward, pdTotalDrift,ppdFactors, iRndSeed, BLOCKSIZE_AUX); /* GC: 51% of the time goes here */
      if (iSuccess!=1)
	     return iSuccess;
    
      //now we compute the discount factor vector

      for(i=0;i<=iN-1;++i){
      	for(b=0;b<=BLOCKSIZE_AUX-1;b++){
      	  pdDiscountingRatePath[BLOCKSIZE_AUX*i + b] = ppdHJMPath[i][0 + b];
      	}
      }

      #ifdef USE_RISCV_VECTOR
            iSuccess = Discount_Factors_Blocking_vector(pdPayoffDiscountFactors, iN, dYears, pdDiscountingRatePath, BLOCKSIZE_AUX); /* 15% of the time goes here */
      #else
            iSuccess = Discount_Factors_Blocking(pdPayoffDiscountFactors, iN, dYears, pdDiscountingRatePath, BLOCKSIZE_AUX); /* 15% of the time goes here */
      #endif

      if (iSuccess!=1)
        return iSuccess;
          
      //now we compute discount factors along the swap path
      for (i=0;i<=iSwapVectorLength-1;++i){
        for(b=0;b<BLOCKSIZE_AUX;b++){
          pdSwapRatePath[i*BLOCKSIZE_AUX + b] = 
          ppdHJMPath[iSwapStartTimeIndex][i*BLOCKSIZE_AUX + b];
        }
      }

      #ifdef USE_RISCV_VECTOR
            iSuccess = Discount_Factors_Blocking_vector(pdSwapDiscountFactors, iSwapVectorLength, dSwapVectorYears, pdSwapRatePath, BLOCKSIZE_AUX);
      #else
            iSuccess = Discount_Factors_Blocking(pdSwapDiscountFactors, iSwapVectorLength, dSwapVectorYears, pdSwapRatePath, BLOCKSIZE_AUX);
      #endif
      
      if (iSuccess!=1)
       return iSuccess;
      // ========================
      // Simulation
      
      #ifdef USE_RISCV_VECTOR
            unsigned long int gvl = _MMR_VSETVL_E64M1(BLOCKSIZE_AUX);
            
            _MMR_f64    xpdSwapDiscountFactors;
            _MMR_f64    xpdSwapPayoffs;
            _MMR_f64    xdFixedLegValue             = _MM_SET_f64(0.0,gvl);
            _MMR_f64    zero                        = _MM_SET_f64(0.0,gvl);
            _MMR_f64    oNE                         = _MM_SET_f64(1.0,gvl);
            _MMR_f64    xdSumSimSwaptionPrice;
            _MMR_f64    xdSumSquareSimSwaptionPrice;

            for (i=0;i<=iSwapVectorLength-1;++i){
                  xpdSwapDiscountFactors = _MM_LOAD_f64(&pdSwapDiscountFactors[i*BLOCKSIZE_AUX],gvl);
                  xpdSwapPayoffs = _MM_SET_f64(pdSwapPayoffs[i],gvl);
                  xdFixedLegValue = _MM_MACC_f64(xdFixedLegValue,xpdSwapPayoffs,xpdSwapDiscountFactors,gvl);
              }

            xdFixedLegValue = _MM_MAX_f64(_MM_SUB_f64(xdFixedLegValue,oNE,gvl), zero,gvl);
            xdFixedLegValue = _MM_MUL_f64(xdFixedLegValue,_MM_SET_f64(pdPayoffDiscountFactors[iSwapStartTimeIndex*BLOCKSIZE_AUX],gvl),gvl);

            // ========= end simulation ======================================
            //xdSumSimSwaptionPrice       = _MM_LOAD_f64(&dSumSimSwaptionPrice,1);
            //xdSumSquareSimSwaptionPrice = _MM_LOAD_f64(&dSumSquareSimSwaptionPrice,1);
            xdSumSimSwaptionPrice       = _MM_SET_f64(dSumSimSwaptionPrice,1);
            xdSumSquareSimSwaptionPrice = _MM_SET_f64(dSumSquareSimSwaptionPrice,1);

            // accumulate into the aggregating variables =====================
            xdSumSimSwaptionPrice = _MM_REDSUM_f64(xdFixedLegValue,xdSumSimSwaptionPrice,gvl);
            xdSumSquareSimSwaptionPrice = _MM_REDSUM_f64(_MM_MUL_f64(xdFixedLegValue,xdFixedLegValue,gvl),xdSumSquareSimSwaptionPrice,gvl);

            _MM_STORE_f64(&dSumSimSwaptionPrice,xdSumSimSwaptionPrice,1);
            _MM_STORE_f64(&dSumSquareSimSwaptionPrice,xdSumSquareSimSwaptionPrice,1);
      #else

      for (b=0;b<BLOCKSIZE_AUX;b++){
          dFixedLegValue = 0.0;
          for (i=0;i<=iSwapVectorLength-1;++i){
              dFixedLegValue += pdSwapPayoffs[i]*pdSwapDiscountFactors[i*BLOCKSIZE_AUX + b];
          }
          dSwaptionPayoff = dMax(dFixedLegValue - 1.0, 0);
          dDiscSwaptionPayoff = dSwaptionPayoff*pdPayoffDiscountFactors[iSwapStartTimeIndex*BLOCKSIZE_AUX + b];
          // ========= end simulation ======================================
          // accumulate into the aggregating variables =====================
          dSumSimSwaptionPrice += dDiscSwaptionPayoff;
          dSumSquareSimSwaptionPrice += dDiscSwaptionPayoff*dDiscSwaptionPayoff;
      } // END BLOCK simulation

      #endif
  }

  // Simulation Results Stored
  dSimSwaptionMeanPrice = dSumSimSwaptionPrice/lTrials;
  dSimSwaptionStdError = sqrt((dSumSquareSimSwaptionPrice-dSumSimSwaptionPrice*dSumSimSwaptionPrice/lTrials)/
			      (lTrials-1.0))/sqrt((FTYPE)lTrials);

  //results returned
  pdSwaptionPrice[0] = dSimSwaptionMeanPrice;
  pdSwaptionPrice[1] = dSimSwaptionStdError;
  
  iSuccess = 1;
  return iSuccess;
}

