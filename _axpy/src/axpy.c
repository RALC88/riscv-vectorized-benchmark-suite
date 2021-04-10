/*************************************************************************
* Vectorized Axpy Kernel
* Author: Jesus Labarta
* Barcelona Supercomputing Center
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "../../common/vector_defines.h"

void axpy_intrinsics(double a, double *dx, double *dy, int n) {
  int i;

  long gvl = __builtin_epi_vsetvl(n, __epi_e64, __epi_m1);
  _MMR_f64 v_a = _MM_SET_f64(a, gvl);
  
  for (i = 0; i < n;) {
    gvl = __builtin_epi_vsetvl(n - i, __epi_e64, __epi_m1);
    _MMR_f64 v_dx = _MM_LOAD_f64(&dx[i], gvl);
    _MMR_f64 v_dy = _MM_LOAD_f64(&dy[i], gvl);
    _MMR_f64 v_res = _MM_MACC_f64(v_dy, v_a, v_dx, gvl);
    _MM_STORE_f64(&dy[i], v_res, gvl);

    i += gvl;
  }

FENCE();
}