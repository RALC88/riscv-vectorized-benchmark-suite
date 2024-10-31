/*************************************************************************
* Vectorized matrixmul Kernel
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#define DATA_TYPE 
typedef double data_t;

#ifdef USE_RISCV_VECTOR
#include <riscv_vector.h>
#include "../../common/vector_defines.h"

void matrixmul_intrinsics(data_t *a, data_t *b, data_t *c, int n, int m, int p) {

    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; j++) {
            size_t gvl = _MMR_VSETVL_E64M1(p);
            vfloat64m1_t vprod = _MM_SET_f64(0, gvl); 
            vfloat64m1_t vsum  = _MM_SET_f64(0, gvl);

            for (size_t k = 0; k < p; k += gvl){
                gvl = _MMR_VSETVL_E64M1(p - k);
                 
                // Matrix A row
                vfloat64m1_t va  = _MM_LOAD_f64(&a[i*p+k], gvl); 
                // Matrix B column
                vfloat64m1_t vb = _MM_LOAD_STRIDE_f64(&b[k*n+j], n * sizeof(data_t), gvl);
                
                // A[0]*B[0], A[1]*B[1],... A[n]*B[n]
                vprod  = _MM_MACC_f64(vprod,va, vb, gvl); 
  
            }//k
            gvl = _MMR_VSETVL_E64M1(p);
            vsum   = _MM_REDSUM_f64(vprod,vsum, gvl);
            c[i*n+j] = _MM_VGETFIRST_f64(vsum);
        }//j
    }//i
}


#else // !USE_RISCV_VECTOR

void matmul_serial(data_t *a, data_t *b, data_t *c, int n, int m, int p) {
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) {
            c[i * n + j] = 0;
            for (int k = 0; k < p; ++k) {
                c[i * n + j] += a[i * p + k] * b[k * n + j];
            }
        }
}

#endif


bool compare( size_t dm, size_t dn, data_t *a ,data_t *b) {
    bool result = false;
    for (int i = 0; i < dm; i++) {
        for (int j = 0; j < dn; j++) {
            if(a[i*dn+j] != b[i*dn+j]) {
              result = true;
            }
        }
 
    }
    return result;
}
