// RanUnif.c
// Author: Mark Broadie
// Collaborator: Mikhail Smelyanskiy, Intel

/* See "Random Number Generators: Good Ones Are Hard To Find", */
/*     Park & Miller, CACM 31#10 October 1988 pages 1192-1201. */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "HJM_type.h"


FTYPE RanUnif( long *s );
void RanUnif_vector( long *s , int iFactors , int iN ,int  BLOCKSIZE, FTYPE **randZ);

FTYPE RanUnif( long *s )
{
  // uniform random number generator

  long   ix, k1;
  FTYPE dRes;
  
  ix = *s;
  k1 = ix/127773L;
  ix = 16807L*( ix - k1*127773L ) - k1 * 2836L;
  if (ix < 0) ix = ix + 2147483647L;
  *s   = ix;
  dRes = (ix * 4.656612875e-10);
  return (dRes);
  
} // end of RanUnif

#ifdef USE_RISCV_VECTOR

void RanUnif_vector( long *s , int iFactors , int iN ,int  BLOCKSIZE , FTYPE **randZ )
{
  // uniform random number generator
  // unsigned long int gvl = __builtin_epi_vsetvl(BLOCKSIZE, __epi_e64, __epi_m1);
  unsigned long int gvl =  vsetvl_e64m1(BLOCKSIZE) //PLCT
  _MMR_i64    k1;
  _MMR_i64      zero;
  _MMR_MASK_i64   mask1;
  _MMR_f64    dRes;

  _MMR_i64    cons1     = _MM_SET_i64(127773,gvl);
  _MMR_i64    cons2     = _MM_SET_i64(16807,gvl);
  _MMR_i64    cons3     = _MM_SET_i64(2836,gvl);  
  _MMR_i64    cons4     = _MM_SET_i64(2147483647,gvl);
  _MMR_f64    cons5     = _MM_SET_f64(4.656612875E-10,gvl);
  _MMR_i64    xSeed       = _MM_LOAD_i64(s,gvl);

  for (int l=0;l<=iFactors-1;++l){
    for (int j=1;j<=iN-1;++j){
          k1    = _MM_DIV_i64(xSeed,cons1,gvl);
          xSeed   = _MM_SUB_i64(_MM_MUL_i64(cons2,_MM_SUB_i64(xSeed,_MM_MUL_i64(k1,cons1,gvl),gvl),gvl), _MM_MUL_i64(k1,cons3,gvl) , gvl);
          zero    = _MM_SET_i64(0,gvl);
          mask1   = _MM_VMSLT_i64(xSeed,zero,gvl); 
          xSeed     = _MM_ADD_i64_MASK(xSeed,xSeed,cons4,mask1,gvl);
          dRes    = _MM_MUL_f64( cons5,_MM_VFCVT_F_X_f64(xSeed,gvl),gvl);

          _MM_STORE_f64(&randZ[l][BLOCKSIZE*j], dRes,gvl);
      }
  }
}

#endif
