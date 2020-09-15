// CumNormalInv.c
// Author: Mark Broadie

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "HJM_type.h"


FTYPE CumNormalInv( FTYPE u );

void CumNormalInv_vector( FTYPE* u ,FTYPE* output ,unsigned long int gvl);

/**********************************************************************/
static FTYPE a[4] = {
  2.50662823884,
    -18.61500062529,
    41.39119773534,
    -25.44106049637
};

static FTYPE b[4] = {
  -8.47351093090,
    23.08336743743,
    -21.06224101826,
    3.13082909833
};

static FTYPE c[9] = {
  0.3374754822726147,
    0.9761690190917186,
    0.1607979714918209,
    0.0276438810333863,
    0.0038405729373609,
    0.0003951896511919,
    0.0000321767881768,
    0.0000002888167364,
    0.0000003960315187
};



/**********************************************************************/
FTYPE CumNormalInv( FTYPE u )
{
  // Returns the inverse of cumulative normal distribution function.
  // Reference: Moro, B., 1995, "The Full Monte," RISK (February), 57-58.
  
  FTYPE x, r;
  
  x = u - 0.5;
  if( fabs (x) < 0.42 )
  { 
    r = x * x;
    r = x * ((( a[3]*r + a[2]) * r + a[1]) * r + a[0])/
          ((((b[3] * r+ b[2]) * r + b[1]) * r + b[0]) * r + 1.0);
  //  ---------------------------------------
  //  TESTING
  //  ---------------------------------------
    // printf("primer resultado\n");
    // printf("r = %f \n",r);
  //  ---------------------------------------
    return (r);
  }
  
  r = u;
  if( x > 0.0 ) r = 1.0 - u;
  r = log(-log(r));
  r = c[0] + r * (c[1] + r * 
       (c[2] + r * (c[3] + r * 
       (c[4] + r * (c[5] + r * (c[6] + r * (c[7] + r*c[8])))))));
  if( x < 0.0 ) r = -r;
  
  
  //  ---------------------------------------
  //  TESTING
  //  ---------------------------------------
    // printf("segundo resultado\n");
    // printf("r1 = %f \n",r);
  //  ---------------------------------------
  return (r);
  
} // end of CumNormalInv

#ifdef USE_RISCV_VECTOR
//#else

void CumNormalInv_vector( FTYPE* u ,FTYPE* output ,unsigned long int gvl)
{
  // Returns the inverse of cumulative normal distribution function.
  // Reference: Moro, B., 1995, "The Full Monte," RISK (February), 57-58.
  
  _MMR_f64   x;
  _MMR_f64   r1;
  _MMR_f64   r;

  _MMR_f64   zero    = _MM_SET_f64(0.0,gvl);
  _MMR_f64   one     = _MM_SET_f64(1.0,gvl);
  _MMR_f64   Cons1   = _MM_SET_f64(0.5,gvl);
  _MMR_f64   Cons2   = _MM_SET_f64(0.42,gvl);
  _MMR_f64   vU      = _MM_LOAD_f64(u,gvl);

  _MMR_f64   a0      = _MM_SET_f64(a[0],gvl);
  _MMR_f64   a1      = _MM_SET_f64(a[1],gvl);
  _MMR_f64   a2      = _MM_SET_f64(a[2],gvl);
  _MMR_f64   a3      = _MM_SET_f64(a[3],gvl);

  _MMR_f64   b0      = _MM_SET_f64(b[0],gvl);
  _MMR_f64   b1      = _MM_SET_f64(b[1],gvl);
  _MMR_f64   b2      = _MM_SET_f64(b[2],gvl);
  _MMR_f64   b3      = _MM_SET_f64(b[3],gvl);

  _MMR_f64   c0      = _MM_SET_f64(c[0],gvl);
  _MMR_f64   c1      = _MM_SET_f64(c[1],gvl);
  _MMR_f64   c2      = _MM_SET_f64(c[2],gvl);
  _MMR_f64   c3      = _MM_SET_f64(c[3],gvl);
  _MMR_f64   c4      = _MM_SET_f64(c[4],gvl);
  _MMR_f64   c5      = _MM_SET_f64(c[5],gvl);
  _MMR_f64   c6      = _MM_SET_f64(c[6],gvl);
  _MMR_f64   c7      = _MM_SET_f64(c[7],gvl);
  _MMR_f64   c8      = _MM_SET_f64(c[8],gvl);

  _MMR_MASK_i64  mask1;
  _MMR_MASK_i64  mask2;
  _MMR_MASK_i64  mask3;

  x = _MM_SUB_f64(vU,Cons1 ,gvl);


  r = _MM_MUL_f64(x,x ,gvl);

  r = _MM_DIV_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(a3,r ,gvl),a2,gvl),r,gvl),a1,gvl),r,gvl),a0,gvl),x,gvl),_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(b3,r ,gvl),b2,gvl),r,gvl),b1,gvl),r,gvl),b0,gvl),r,gvl),one,gvl),gvl);

  // SECOND PART
  mask2  = _MM_VFGT_f64(x,zero,gvl); 
  r1 = vU;
  r1   = _MM_SUB_f64_MASK(r1,one,vU,mask2,gvl); //sub(vs2,vs1)
  Cons1 = _MM_LOG_f64(r1,gvl);
  r1 = _MM_VFSGNJN_f64(Cons1,Cons1,gvl);
  r1 = _MM_LOG_f64(r1,gvl);

  r1 = _MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(_MM_ADD_f64(_MM_MUL_f64(c8,r1 ,gvl),c7,gvl),r1,gvl),c6,gvl),r1,gvl),c5,gvl),r1,gvl),c4,gvl),r1,gvl),c3,gvl),r1,gvl),c2,gvl),r1,gvl),c1,gvl),r1,gvl),c0,gvl);
  mask3  = _MM_VFLT_f64(x,zero,gvl); 
  r1 = _MM_MERGE_f64(r1,_MM_VFSGNJN_f64(r1,r1,gvl), mask3,gvl);

  mask1  = _MM_VFLT_f64(_MM_VFSGNJX_f64(x,x,gvl),Cons2,gvl); 
  r = _MM_MERGE_f64(r1,r, mask1,gvl);

  _MM_STORE_f64(output,r,gvl);

} // end of CumNormalInv

#endif // USE_RISCV_VECTOR
/**********************************************************************/
// end of CumNormalInv.c  
