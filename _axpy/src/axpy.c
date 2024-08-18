/*************************************************************************
* Vectorized Axpy Kernel
* Author: Jesus Labarta
* Barcelona Supercomputing Center
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <riscv_vector.h>

void axpy_intrinsics(double a, double *dx, double *dy, int n) {
  int i;

  long gvl = __riscv_vsetvl_e64m1(n);
  vfloat64m1_t v_a = __riscv_vfmv_s_f_f64m1(a, gvl);
  
  for (i = 0; i < n;) {
    gvl = __riscv_vsetvl_e64m1(n - i);
    vfloat64m1_t v_dx = __riscv_vle64_v_f64m1(&dx[i], gvl);
    vfloat64m1_t v_dy = __riscv_vle64_v_f64m1(&dy[i], gvl);
    vfloat64m1_t v_res = __riscv_vfmacc_vf_f64m1(v_dy, a, v_dx, gvl);
    __riscv_vse64_v_f64m1(&dy[i], v_res, gvl);

    i += gvl;
  }
}