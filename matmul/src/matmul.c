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

void matrixmul_intrinsics(data_t *a, data_t *b, data_t *c, int n, int m, int p) {

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            size_t gvl = __riscv_vsetvl_e64m1(p);
            vfloat64m1_t vprod = __riscv_vfmv_v_f_f64m1(0, gvl); 
            vfloat64m1_t vsum  = __riscv_vfmv_v_f_f64m1(0, gvl);

            for (size_t k = 0; k < p; k += gvl){
                gvl = __riscv_vsetvl_e64m1(p - k);
                 
                // Matrix A row
                vfloat64m1_t va  = __riscv_vle64_v_f64m1(&a[i*p+k], gvl); 
                // Matrix B column
                vfloat64m1_t vb = __riscv_vlse64_v_f64m1(&b[k*m+j], m * sizeof(data_t), gvl);
                
                // A[0]*B[0], A[1]*B[1],... A[n]*B[n]
                vprod  = __riscv_vfmacc_vv_f64m1(vprod,va, vb, gvl); 
  
            }//k
            vsum   = __riscv_vfredosum_vs_f64m1_f64m1(vprod,vsum, gvl);
            c[i*m+j] = __riscv_vfmv_f_s_f64m1_f64(vsum);
        }//j
    }//i
}


#else // !USE_RISCV_VECTOR

void matmul_serial(data_t *a, data_t *b, data_t *c, int n, int m, int p) {
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j) {
      c[i * m + j] = 0;
      for (int k = 0; k < p; ++k) {
        c[i * m + j] += a[i * p + k] * b[k * m + j];
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
