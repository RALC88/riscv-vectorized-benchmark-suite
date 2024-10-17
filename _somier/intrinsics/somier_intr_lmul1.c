/*************************************************************************
* Somier - RISC-V Vectorized version
* Author: Jesus Labarta
* Barcelona Supercomputing Center
*************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>
#include "../somier.h"

#ifdef USE_RISCV_VECTOR
#include "../common/vector_defines.h"
#endif

static int count2=0;

void accel_intr(int n, double (*A)[n][n][n], double (*F)[n][n][n], double M)
{
   int i, j, k;
   unsigned long int rvl, gvl;
   _MMR_f64 vF0, vF1, vF2, vA0, vA1, vA2;
   double invM = 1/M;

   for (i = 0; i<n; i++)
      for (j = 0; j<n; j++)
         for (k = 0; k<n; ) {
            rvl = n-k;
            gvl = _MMR_VSETVL_E64M1(rvl);
            _MMR_f64 v_invM = _MM_SET_f64(invM, gvl);

            vF0 = _MM_LOAD_f64( &F[0][i][j][k], gvl );
            vA0 = _MM_MUL_f64(vF0, v_invM, gvl);
            _MM_STORE_f64(&A[0][i][j][k], vA0, gvl);

            vF1 = _MM_LOAD_f64( &F[1][i][j][k], gvl );
            vA1 = _MM_MUL_f64(vF1, v_invM, gvl);
            _MM_STORE_f64(&A[1][i][j][k], vA1, gvl);

            vF2 = _MM_LOAD_f64( &F[2][i][j][k], gvl );
            vA2 = _MM_MUL_f64(vF2, v_invM, gvl);
            _MM_STORE_f64(&A[2][i][j][k], vA2, gvl);

            k+=gvl;
	 }

}

#undef COLAPSED
#define COLAPSED

void vel_intr(int n, double (*V)[n][n][n], double (*A)[n][n][n], double dt)
{
   int i, j, k;
   unsigned long rvl, gvl;
   _MMR_f64 vV0, vV1, vV2, vA0, vA1, vA2;
//#ifndef COLAPSED
#if 1
   for (int cid = 0; cid<n*n*n; ) {
      i = cid/(n*n);
      j = (cid/n)%n;
      k= cid%n;
      rvl = n*n*n-cid;
#else
   MAX = n;
   for (i = 0; i<n; i++) {
      for (j = 0; j<n; j++) {
         for (k = 0; k<n; ) {
            rvl = n-k;
#endif
            gvl = _MMR_VSETVL_E64M1(rvl);
            _MMR_f64 vdt = _MM_SET_f64(dt, gvl);

            vV0 = _MM_LOAD_f64( &V[0][i][j][k], gvl );
            vA0 = _MM_LOAD_f64( &A[0][i][j][k], gvl );
            vV0 = _MM_MACC_f64(vV0, vA0, vdt, gvl);
            _MM_STORE_f64(&V[0][i][j][k], vV0, gvl);

            vV1 = _MM_LOAD_f64( &V[1][i][j][k], gvl );
            vA1 = _MM_LOAD_f64( &A[1][i][j][k], gvl );
            vV1 = _MM_MACC_f64(vV1, vA1, vdt, gvl);
            _MM_STORE_f64(&V[1][i][j][k], vV1, gvl);
            vV2 = _MM_LOAD_f64( &V[2][i][j][k], gvl );
            vA2 = _MM_LOAD_f64( &A[2][i][j][k], gvl );
            vV2 = _MM_MACC_f64(vV2, vA2, vdt, gvl);
            _MM_STORE_f64(&V[2][i][j][k], vV2, gvl);

//#ifndef COLAPSED
#if 0
            k+=gvl;
         }
      }
   }
#else
      cid+=gvl;
   }
#endif

}

void pos_intr(int n, double (*X)[n][n][n], double (*V)[n][n][n], double dt)
{
   int i, j, k;
   unsigned long int rvl, gvl;
   _MMR_f64 vV0, vV1, vV2, vX0, vX1, vX2;


   for (i = 0; i<n; i++) {
      for (j = 0; j<n; j++) {
         for (k = 0; k<n;) {
            rvl = n-k;
            gvl = _MMR_VSETVL_E64M1(rvl);
            _MMR_f64 vdt = _MM_SET_f64(dt, gvl);

            vX0 = _MM_LOAD_f64( &X[0][i][j][k], gvl );
            vV0 = _MM_LOAD_f64( &V[0][i][j][k], gvl );
            vX0 = _MM_MACC_f64(vX0, vV0, vdt, gvl);
            _MM_STORE_f64(&X[0][i][j][k], vX0, gvl);

            vX1 = _MM_LOAD_f64( &X[1][i][j][k], gvl );
            vV1 = _MM_LOAD_f64( &V[1][i][j][k], gvl );
            vX1 = _MM_MACC_f64(vX1, vV1, vdt, gvl);
            _MM_STORE_f64(&X[1][i][j][k], vX1, gvl);
            vX2 = _MM_LOAD_f64( &X[2][i][j][k], gvl );
            vV2 = _MM_LOAD_f64( &V[2][i][j][k], gvl );
            vX2 = _MM_MACC_f64(vX2, vV2, vdt, gvl);
            _MM_STORE_f64(&X[2][i][j][k], vX2, gvl);

            k+=gvl;
         }
      }
   }
   // would need to check that possition des not go beyond the box walls
}
//      for (i = 0; i<N; i++)
//         for (j = 0; j<N; j++)
//            for (kk = 0; kk<N; kk+=vl) {

//             int maxk = (kk+vl < N? kk+vl: N);
//               for (k = kk; k<maxk; k++) {
//               X[0][i][j][k] += V[0][i][j][k]*dt;
//               X[1][i][j][k] += V[1][i][j][k]*dt;
//               X[2][i][j][k] += V[2][i][j][k]*dt;
//             }
//            }

