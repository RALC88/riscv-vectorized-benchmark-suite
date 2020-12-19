// Copyright (c) 2007 Intel Corp.

// Black-Scholes
// Analytical method for calculating European Options
//
// 
// Reference Source: Options, Futures, and Other Derivatives, 3rd Edition, Prentice 
// Hall, John C. Hull,

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <sys/time.h>

// RISC-V VECTOR Version by Cristóbal Ramírez Lazo, "Barcelona 2019"
#ifdef USE_RISCV_VECTOR
#include "../../common/vector_defines.h"
#endif

#ifdef ENABLE_PARSEC_HOOKS
#include <hooks.h>
#endif

// Multi-threaded pthreads header
#ifdef ENABLE_THREADS
// Add the following line so that icc 9.0 is compatible with pthread lib.
#define __thread __threadp
MAIN_ENV
#undef __thread
#endif

// Multi-threaded OpenMP header
#ifdef ENABLE_OPENMP
#include <omp.h>
#endif

#ifdef ENABLE_TBB
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"

using namespace std;
using namespace tbb;
#endif //ENABLE_TBB

// Multi-threaded header for Windows
#ifdef WIN32
#pragma warning(disable : 4305)
#pragma warning(disable : 4244)
#include <windows.h>
#endif

//Precision to use for calculations
#define fptype float

#define NUM_RUNS  100

typedef struct OptionData_ {
        fptype s;          // spot price
        fptype strike;     // strike price
        fptype r;          // risk-free interest rate
        fptype divq;       // dividend rate
        fptype v;          // volatility
        fptype t;          // time to maturity or option expiration in years
                           //     (1yr = 1.0, 6mos = 0.5, 3mos = 0.25, ..., etc)
        char OptionType;   // Option type.  "P"=PUT, "C"=CALL
        fptype divs;       // dividend vals (not used in this test)
        fptype DGrefval;   // DerivaGem Reference Value
} OptionData;

OptionData* data;
fptype* prices;
int numOptions;

int    * otype;
fptype * sptprice;
fptype * strike;
fptype * rate;
fptype * volatility;
fptype * otime;
int numError = 0;
int nThreads;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Cumulative Normal Distribution Function
// See Hull, Section 11.8, P.243-244
#define inv_sqrt_2xPI 0.39894228040143270286


#ifdef USE_RISCV_VECTOR

_MMR_f32 CNDF_SIMD  (_MMR_f32 xInput ,unsigned long int gvl) 
{

  _MMR_f32 xNPrimeofX;
  _MMR_f32 xK2;
  _MMR_f32 xK2_2;
  _MMR_f32 xK2_3;
  _MMR_f32 xK2_4;
  _MMR_f32 xK2_5;
  _MMR_f32 xLocal;
  _MMR_f32 xLocal_1;
  _MMR_f32 xLocal_2;
  _MMR_f32 xLocal_3;

  _MMR_f32 xVLocal_2;
  _MMR_MASK_i32 xMask;

  _MMR_f32 expValues;

  _MMR_f32 xOne   = _MM_SET_f32(1.0,gvl);

  xVLocal_2   = _MM_SET_f32(0.0,gvl);
  xMask       = _MM_VFLT_f32(xInput,xVLocal_2,gvl);  //_MM_VFLT_f32(src2,src1)
  xInput      = _MM_VFSGNJX_f32(xInput,xInput,gvl); //_MM_VFSGNJX_f32(src2,src1)

  expValues   = _MM_MUL_f32(xInput, xInput,gvl);
  expValues   = _MM_MUL_f32(expValues, _MM_SET_f32(-0.5,gvl),gvl);

  xNPrimeofX = _MM_EXP_f32(expValues ,gvl);
  FENCE();
  xNPrimeofX = _MM_MUL_f32(xNPrimeofX, _MM_SET_f32(inv_sqrt_2xPI,gvl),gvl);
  //xK2 = _MM_MUL_f32(_MM_SET_f32(0.2316419,gvl), xInput,gvl);
  //xK2 = _MM_ADD_f32(xK2, xOne,gvl);
  xK2   = _MM_SET_f32(0.2316419,gvl);
  xK2   = _MM_MADD_f32(xK2,xInput,xOne,gvl);

  xK2   = _MM_DIV_f32(xOne,xK2,gvl);
  xK2_2 = _MM_MUL_f32(xK2, xK2,gvl);
  xK2_3 = _MM_MUL_f32(xK2_2, xK2,gvl);
  xK2_4 = _MM_MUL_f32(xK2_3, xK2,gvl);
  xK2_5 = _MM_MUL_f32(xK2_4, xK2,gvl);

  xLocal_1 = _MM_MUL_f32(xK2, _MM_SET_f32(0.319381530,gvl),gvl);
  xLocal_2 = _MM_MUL_f32(xK2_2, _MM_SET_f32(-0.356563782,gvl),gvl);

  xLocal_3 = _MM_MUL_f32(xK2_3, _MM_SET_f32(1.781477937,gvl),gvl);
  xLocal_2 = _MM_ADD_f32(xLocal_2, xLocal_3,gvl);
  //xLocal_2   = _MM_MACC_f32(xLocal_2,xK2_3,_MM_SET_f32(1.781477937,gvl),gvl);

  //xLocal_3 = _MM_MUL_f32(xK2_4, _MM_SET_f32(-1.821255978,gvl),gvl);
  //xLocal_2 = _MM_ADD_f32(xLocal_2, xLocal_3,gvl);
  xLocal_2   = _MM_MACC_f32(xLocal_2,xK2_4,_MM_SET_f32(-1.821255978,gvl),gvl);

  xLocal_3 = _MM_MUL_f32(xK2_5, _MM_SET_f32(1.330274429,gvl),gvl);
  xLocal_2 = _MM_ADD_f32(xLocal_2, xLocal_3,gvl);
  //xLocal_2   = _MM_MACC_f32(xLocal_2,xK2_5,_MM_SET_f32(1.330274429,gvl),gvl);

  xLocal_1 = _MM_ADD_f32(xLocal_2, xLocal_1,gvl);

  xLocal   = _MM_MUL_f32(xLocal_1, xNPrimeofX,gvl);
  xLocal   = _MM_SUB_f32(xOne,xLocal,gvl);
  //xLocal   = _MM_NMSUB_f32(xLocal,xNPrimeofX,xOne,gvl);

  xLocal   = _MM_SUB_f32_MASK(xLocal,xOne,xLocal,xMask,gvl); //sub(vs2,vs1)
  return xLocal;
}


void BlkSchlsEqEuroNoDiv_vector (fptype * OptionPrice, int numOptions, fptype * sptprice,
                          fptype * strike, fptype * rate, fptype * volatility,
                          fptype * time, int * otype/*,long int *  otype_d*/, float timet ,unsigned long int gvl)
{
    // local private working variables for the calculation
    _MMR_f32 xStockPrice;
    _MMR_f32 xStrikePrice;
    _MMR_f32 xRiskFreeRate;
    _MMR_f32 xVolatility;
    _MMR_f32 xTime;
    _MMR_f32 xSqrtTime;
    _MMR_f32 xLogTerm;
    _MMR_f32 xD1, xD2;
    _MMR_f32 xPowerTerm;
    _MMR_f32 xDen;

    _MMR_f32 xRatexTime;
    _MMR_f32 xFutureValueX;

    _MMR_MASK_i32 xMask;
    _MMR_i32 xOtype;
    _MMR_i32  xZero;

    _MMR_f32 xOptionPrice;
    _MMR_f32 xOptionPrice1;
    _MMR_f32 xOptionPrice2;
    _MMR_f32 xfXd1;
    _MMR_f32 xfXd2;
    
    FENCE();
    xStrikePrice = _MM_LOAD_f32(strike,gvl);
    xStockPrice = _MM_LOAD_f32(sptprice,gvl);
    xStrikePrice = _MM_DIV_f32(xStockPrice,xStrikePrice,gvl);
    xLogTerm = _MM_LOG_f32(xStrikePrice,gvl);
    //FENCE();
    xRiskFreeRate = _MM_LOAD_f32(rate,gvl);
    xVolatility = _MM_LOAD_f32(volatility,gvl);
    xTime = _MM_LOAD_f32(time,gvl);
    xSqrtTime = _MM_SQRT_f32(xTime,gvl);
    xRatexTime = _MM_MUL_f32(xRiskFreeRate, xTime,gvl);
    xRatexTime = _MM_VFSGNJN_f32(xRatexTime, xRatexTime,gvl);

    xFutureValueX = _MM_EXP_f32(xRatexTime,gvl);
    FENCE();
    xPowerTerm = _MM_MUL_f32(xVolatility, xVolatility,gvl);
    xPowerTerm = _MM_MUL_f32(xPowerTerm, _MM_SET_f32(0.5,gvl),gvl);
    xD1 = _MM_ADD_f32( xRiskFreeRate , xPowerTerm,gvl);
    
    //xD1 = _MM_MUL_f32(xD1, xTime,gvl);
    //xD1 = _MM_ADD_f32(xD1,xLogTerm,gvl);
    xD1   = _MM_MADD_f32(xD1,xTime,xLogTerm,gvl);

    xDen = _MM_MUL_f32(xVolatility, xSqrtTime,gvl);
    xD1 = _MM_DIV_f32(xD1,xDen,gvl);
    xD2 = _MM_SUB_f32(xD1,xDen ,gvl);

    xfXd1 = CNDF_SIMD( xD1 ,gvl);
    xfXd2 = CNDF_SIMD( xD2 ,gvl);

    xStrikePrice = _MM_LOAD_f32(strike,gvl);
    FENCE();
    xFutureValueX = _MM_MUL_f32(xFutureValueX, xStrikePrice,gvl);

    xOtype    = _MM_LOAD_i32(otype,gvl);
    xZero     = _MM_SET_i32(0,gvl);
    xMask     = _MM_VMSEQ_i32(xZero,xOtype,gvl);
    xfXd1   = _MM_MERGE_f32(_MM_SUB_f32(_MM_SET_f32(1.0,gvl),xfXd1,gvl),xfXd1, xMask,gvl);
    xStockPrice = _MM_LOAD_f32(sptprice,gvl);
    xOptionPrice1 = _MM_MUL_f32(xStockPrice, xfXd1,gvl);
    FENCE();
    xfXd2   = _MM_MERGE_f32(_MM_SUB_f32(_MM_SET_f32(1.0,gvl),xfXd2,gvl),xfXd2, xMask,gvl);
    xOptionPrice2 = _MM_MUL_f32(xFutureValueX, xfXd2,gvl);
    xOptionPrice = _MM_SUB_f32(xOptionPrice2,xOptionPrice1,gvl);
    xOptionPrice = _MM_VFSGNJX_f32(xOptionPrice,xOptionPrice,gvl);
    _MM_STORE_f32(OptionPrice, xOptionPrice,gvl);
    FENCE();
}

#endif // USE_RISCV_VECTOR

fptype CNDF ( fptype InputX )
{
  int sign;

  fptype OutputX;
  fptype xInput;
  fptype xNPrimeofX;
  fptype expValues;
  fptype xK2;
  fptype xK2_2, xK2_3;
  fptype xK2_4, xK2_5;
  fptype xLocal, xLocal_1;
  fptype xLocal_2, xLocal_3;

  // Check for negative value of InputX
  if (InputX < 0.0) {
    InputX = -InputX;
    sign = 1;
  } else
    sign = 0;

  xInput = InputX;

  // Compute NPrimeX term common to both four & six decimal accuracy calcs
  expValues = exp(-0.5f * InputX * InputX);
  xNPrimeofX = expValues;
  xNPrimeofX = xNPrimeofX * inv_sqrt_2xPI;

  xK2 = 0.2316419 * xInput;
  xK2 = 1.0 + xK2;
  xK2 = 1.0 / xK2;
  xK2_2 = xK2 * xK2;
  xK2_3 = xK2_2 * xK2;
  xK2_4 = xK2_3 * xK2;
  xK2_5 = xK2_4 * xK2;

  xLocal_1 = xK2 * 0.319381530;
  xLocal_2 = xK2_2 * (-0.356563782);
  xLocal_3 = xK2_3 * 1.781477937;
  xLocal_2 = xLocal_2 + xLocal_3;
  xLocal_3 = xK2_4 * (-1.821255978);
  xLocal_2 = xLocal_2 + xLocal_3;
  xLocal_3 = xK2_5 * 1.330274429;
  xLocal_2 = xLocal_2 + xLocal_3;

  xLocal_1 = xLocal_2 + xLocal_1;
  xLocal   = xLocal_1 * xNPrimeofX;
  xLocal   = 1.0 - xLocal;

  OutputX  = xLocal;

  if (sign) {
    OutputX = 1.0 - OutputX;
  }

  return OutputX;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
fptype BlkSchlsEqEuroNoDiv( fptype sptprice,
                            fptype strike, fptype rate, fptype volatility,
                            fptype time, int otype, float timet )
{
  fptype OptionPrice;

  // local private working variables for the calculation
  fptype xStockPrice;
  fptype xStrikePrice;
  fptype xRiskFreeRate;
  fptype xVolatility;
  fptype xTime;
  fptype xSqrtTime;

  fptype logValues;
  fptype xLogTerm;
  fptype xD1;
  fptype xD2;
  fptype xPowerTerm;
  fptype xDen;
  fptype d1;
  fptype d2;
  fptype FutureValueX;
  fptype NofXd1;
  fptype NofXd2;
  fptype NegNofXd1;
  fptype NegNofXd2;

  xStockPrice = sptprice;
  xStrikePrice = strike;
  xRiskFreeRate = rate;
  xVolatility = volatility;

  xTime = time;
  xSqrtTime = sqrt(xTime);

  logValues = log( sptprice / strike );

  xLogTerm = logValues;


  xPowerTerm = xVolatility * xVolatility;
  xPowerTerm = xPowerTerm * 0.5;

  xD1 = xRiskFreeRate + xPowerTerm;
  xD1 = xD1 * xTime;
  xD1 = xD1 + xLogTerm;

  xDen = xVolatility * xSqrtTime;
  xD1 = xD1 / xDen;
  xD2 = xD1 -  xDen;

  d1 = xD1;
  d2 = xD2;

  NofXd1 = CNDF( d1 );
  NofXd2 = CNDF( d2 );

  FutureValueX = strike * ( exp( -(rate)*(time) ) );
  if (otype == 0) {
    OptionPrice = (sptprice * NofXd1) - (FutureValueX * NofXd2);
  } else {
    NegNofXd1 = (1.0 - NofXd1);
    NegNofXd2 = (1.0 - NofXd2);
    OptionPrice = (FutureValueX * NegNofXd2) - (sptprice * NegNofXd1);
  }

  return OptionPrice;
}


#ifdef ENABLE_TBB
struct mainWork {
  mainWork(){}
  mainWork(mainWork &w, tbb::split){}

  void operator()(const tbb::blocked_range<int> &range) const {
    fptype price;
    int begin = range.begin();
    int end = range.end();

    for (int i=begin; i!=end; i++) {
      /* Calling main function to calculate option value based on
       * Black & Scholes's equation.
       */

      price = BlkSchlsEqEuroNoDiv( sptprice[i], strike[i],
                                   rate[i], volatility[i], otime[i],
                                   otype[i], 0);
      prices[i] = price;

#ifdef ERR_CHK
      fptype priceDelta = data[i].DGrefval - price;
      if( fabs(priceDelta) >= 1e-5 ){
        fprintf(stderr,"Error on %d. Computed=%.5f, Ref=%.5f, Delta=%.5f\n",
		i, price, data[i].DGrefval, priceDelta);
        numError ++;
      }
#endif
    }
  }
};
#endif // ENABLE_TBB

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_TBB
int bs_thread(void *tid_ptr) {
    int j;
    tbb::affinity_partitioner a;

    mainWork doall;
    for (j=0; j<NUM_RUNS; j++) {
      tbb::parallel_for(tbb::blocked_range<int>(0, numOptions), doall, a);
    }

    return 0;
}
#else // !ENABLE_TBB

#ifdef WIN32
DWORD WINAPI bs_thread(LPVOID tid_ptr){
#else
int bs_thread(void *tid_ptr) {
#endif

#ifdef USE_RISCV_VECTOR
    int i, j, k;
    fptype priceDelta;
    int tid = *(int *)tid_ptr;
    int start = tid * (numOptions / nThreads);
    int end = start + (numOptions / nThreads);

    // unsigned long int gvl = __builtin_epi_vsetvl(end, __epi_e32, __epi_m1);
    unsigned long int gvl = vsetvl_e32m1(end) //PLCT
    fptype* price;
    price = (fptype*)malloc(gvl*sizeof(fptype));
    //price = aligned_alloc(64, gvl*sizeof(fptype));

#ifdef ENABLE_PARSEC_HOOKS
    __parsec_thread_begin();
#endif

    for (j=0; j<NUM_RUNS; j++) {
#ifdef ENABLE_OPENMP
#pragma omp parallel for private(i, price, priceDelta)
        for (i=0; i<numOptions; i += gvl) {
#else  //ENABLE_OPENMP
        for (i=start; i<end; i += gvl) {
#endif //ENABLE_OPENMP
            // Calling main function to calculate option value based on Black & Scholes's
            // equation.
            // gvl = __builtin_epi_vsetvl(end-i, __epi_e32, __epi_m1);
            gvl = vsetvl_e32m1(end-i) //PLCT
            BlkSchlsEqEuroNoDiv_vector( price, gvl, &(sptprice[i]), &(strike[i]),
                                &(rate[i]), &(volatility[i]), &(otime[i]), &(otype[i])/*,&(otype_d[i])*/, 0,gvl);
            for (k=0; k<gvl; k++) {
              prices[i+k] = price[k];
            }
#ifdef ERR_CHK
            for (k=0; k<gvl; k++) {
                priceDelta = data[i+k].DGrefval - price[k];
                if (fabs(priceDelta) >= 1e-4) {
                    printf("Error on %d. Computed=%.5f, Ref=%.5f, Delta=%.5f\n",
                           i + k, price[k], data[i+k].DGrefval, priceDelta);
                    numError ++;
                }
            }
#endif
        }
    }

#ifdef ENABLE_PARSEC_HOOKS
	__parsec_thread_end();
#endif

    return 0;
    }
#else // ! USE_RISCV_VECTOR
      int i, j;
      fptype price;
      fptype priceDelta;
      int tid = *(int *)tid_ptr;
      int start = tid * (numOptions / nThreads);
      int end = start + (numOptions / nThreads);

for (j=0; j<NUM_RUNS; j++) {
#ifdef ENABLE_OPENMP
#pragma omp parallel for private(i, price, priceDelta)
        for (i=0; i<numOptions; i++) {
#else  //ENABLE_OPENMP
	  for (i=start; i<end; i++) {
#endif //ENABLE_OPENMP
            /* Calling main function to calculate option value based on
             * Black & Scholes's equation.
             */
            price = BlkSchlsEqEuroNoDiv( sptprice[i], strike[i],
                                         rate[i], volatility[i], otime[i],
                                         otype[i], 0);
            prices[i] = price;

#ifdef ERR_CHK
            priceDelta = data[i].DGrefval - price;
            if( fabs(priceDelta) >= 1e-4 ){
	      printf("Error on %d. Computed=%.5f, Ref=%.5f, Delta=%.5f\n",
		     i, price, data[i].DGrefval, priceDelta);
	      numError ++;
            }
#endif
	  }
	}

	return 0;
}

#endif // USE_RISCV_VECTOR
#endif //ENABLE_TBB




int main (int argc, char **argv)
{
    FILE *file;
    int i;
    int loopnum;
    fptype * buffer;
    int * buffer2;
    int rv;

//#ifdef USE_RISCV_VECTOR
    struct timeval tv1_0, tv2_0;
    struct timezone tz_0;
    double elapsed0=0.0;
    gettimeofday(&tv1_0, &tz_0);
//#endif


#ifdef PARSEC_VERSION
#define __PARSEC_STRING(x) #x
#define __PARSEC_XSTRING(x) __PARSEC_STRING(x)
        printf("PARSEC Benchmark Suite Version "__PARSEC_XSTRING(PARSEC_VERSION)"\n");
	fflush(NULL);
#else
        printf("PARSEC Benchmark Suite\n");
	fflush(NULL);
#endif //PARSEC_VERSION
#ifdef ENABLE_PARSEC_HOOKS
   __parsec_bench_begin(__parsec_blackscholes);
#endif


   if (argc != 4)
        {
                printf("Usage:\n\t%s <nthreads> <inputFile> <outputFile>\n", argv[0]);
                exit(1);
        }
    nThreads = atoi(argv[1]);
    char *inputFile = argv[2];
    char *outputFile = argv[3];

    //Read input data from file
    file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }
    rv = fscanf(file, "%i", &numOptions);
    if(rv != 1) {
      printf("ERROR: Unable to read from file `%s'.\n", inputFile);
      fclose(file);
      exit(1);
    }

#if !defined(ENABLE_THREADS) && !defined(ENABLE_OPENMP) && !defined(ENABLE_TBB)
    if(nThreads != 1) {
        printf("Error: <nthreads> must be 1 (serial version)\n");
        exit(1);
    }
#endif

    data = (OptionData*)malloc(numOptions*sizeof(OptionData));
    prices = (fptype*)malloc(numOptions*sizeof(fptype));
    for ( loopnum = 0; loopnum < numOptions; ++ loopnum )
    {
        rv = fscanf(file, "%f %f %f %f %f %f %c %f %f", &data[loopnum].s, &data[loopnum].strike, &data[loopnum].r, &data[loopnum].divq, &data[loopnum].v, &data[loopnum].t, &data[loopnum].OptionType, &data[loopnum].divs, &data[loopnum].DGrefval);
        if(rv != 9) {
          printf("ERROR: Unable to read from file `%s'.\n", inputFile);
          fclose(file);
          exit(1);
        }
    }

    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", inputFile);
      exit(1);
    }

#ifdef ENABLE_THREADS
    MAIN_INITENV(,8000000,nThreads);
#endif
    printf("Num of Options: %d\n", numOptions);
    printf("Num of Runs: %d\n", NUM_RUNS);

#define PAD 256
#define LINESIZE 64

    buffer = (fptype *) malloc(5 * numOptions * sizeof(fptype) + PAD);
    sptprice = (fptype *) (((unsigned long long)buffer + PAD) & ~(LINESIZE - 1));
    strike = sptprice + numOptions;
    rate = strike + numOptions;
    volatility = rate + numOptions;
    otime = volatility + numOptions;

    buffer2 = (int *) malloc(numOptions * sizeof(fptype) + PAD);
    otype = (int *) (((unsigned long long)buffer2 + PAD) & ~(LINESIZE - 1));

    for (i=0; i<numOptions; i++) {
        otype[i]      = (data[i].OptionType == 'P') ? 1 : 0;
        sptprice[i]   = data[i].s;
        strike[i]     = data[i].strike;
        rate[i]       = data[i].r;
        volatility[i] = data[i].v;
        otime[i]      = data[i].t;
    }

    printf("Size of data: %lu\n", numOptions * (sizeof(OptionData) + sizeof(int)));

//#ifdef USE_RISCV_VECTOR
    gettimeofday(&tv2_0, &tz_0);
    elapsed0 = (double) (tv2_0.tv_sec-tv1_0.tv_sec) + (double) (tv2_0.tv_usec-tv1_0.tv_usec) * 1.e-6; 
    printf("\n\nBlackScholes Initialization took %8.8lf secs   \n", elapsed0 );
//#endif

//#ifdef USE_RISCV_VECTOR
    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed1=0.0;
    gettimeofday(&tv1, &tz);
//#endif

#ifdef ENABLE_PARSEC_HOOKS
    __parsec_roi_begin();
#endif

#ifdef ENABLE_THREADS
#ifdef WIN32
    HANDLE *threads;
    int *nums;
    threads = (HANDLE *) malloc (nThreads * sizeof(HANDLE));
    nums = (int *) malloc (nThreads * sizeof(int));

    for(i=0; i<nThreads; i++) {
        nums[i] = i;
        threads[i] = CreateThread(0, 0, bs_thread, &nums[i], 0, 0);
    }
    WaitForMultipleObjects(nThreads, threads, TRUE, INFINITE);
    free(threads);
    free(nums);
#else
    int *tids;
    tids = (int *) malloc (nThreads * sizeof(int));

    for(i=0; i<nThreads; i++) {
        tids[i]=i;
        CREATE_WITH_ARG(bs_thread, &tids[i]);
    }
    WAIT_FOR_END(nThreads);
    free(tids);
#endif //WIN32
#else //ENABLE_THREADS
#ifdef ENABLE_OPENMP
    {
        int tid=0;
        omp_set_num_threads(nThreads);
        bs_thread(&tid);
    }
#else //ENABLE_OPENMP
#ifdef ENABLE_TBB
    tbb::task_scheduler_init init(nThreads);

    int tid=0;
    bs_thread(&tid);
#else //ENABLE_TBB
    //serial version
    int tid=0;
    bs_thread(&tid);
#endif //ENABLE_TBB
#endif //ENABLE_OPENMP
#endif //ENABLE_THREADS

#ifdef ENABLE_PARSEC_HOOKS
    __parsec_roi_end();
#endif

//#ifdef USE_RISCV_VECTOR
    gettimeofday(&tv2, &tz);
    elapsed1 = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec) * 1.e-6; 
    printf("\n\nBlackScholes Kernel took %8.8lf secs   \n", elapsed1 );
//#endif

    //Write prices to output file
    file = fopen(outputFile, "w");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", outputFile);
      exit(1);
    }
    rv = fprintf(file, "%i\n", numOptions);
    if(rv < 0) {
      printf("ERROR: Unable to write to file `%s'.\n", outputFile);
      fclose(file);
      exit(1);
    }
    for(i=0; i<numOptions; i++) {
      rv = fprintf(file, "%.18f\n", prices[i]);
      if(rv < 0) {
        printf("ERROR: Unable to write to file `%s'.\n", outputFile);
        fclose(file);
        exit(1);
      }
    }
    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", outputFile);
      exit(1);
    }

#ifdef ERR_CHK
    printf("Num Errors: %d\n", numError);
#endif
    free(data);
    free(prices);

#ifdef ENABLE_PARSEC_HOOKS
    __parsec_bench_end();
#endif

    return 0;
}

