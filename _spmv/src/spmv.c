#include <stdlib.h>
#include <stdio.h>
#include "../../common/riscv_util.h"
#ifdef USE_RISCV_VECTOR
#include <riscv_vector.h>
#include "vector_defines.h"
#endif

#ifdef USE_RISCV_VECTOR
void spmv_intrinsics(const size_t nrows, double *a, uint64_t *ia, uint64_t *ja, double *x, double *y) {
    
    for (size_t row = 0; row < nrows; row++) {
        
        uint64_t nnz_row = ia[row + 1] - ia[row];
        uint64_t idx     = ia[row];
        
        size_t gvl = __riscv_vsetvl_e64m1(nnz_row);
        _MMR_f64 vprod    = _MM_SET_f64(0, gvl); 
        _MMR_f64 part_res = _MM_SET_f64(0, gvl);
        _MMR_u64 vthree   = __riscv_vmv_v_x_u64m1(3, gvl);

        for(size_t colid = 0; colid < nnz_row; colid += gvl ) {
                     gvl       = __riscv_vsetvl_e64m1(nnz_row - colid);
            _MMR_f64 va        = _MM_LOAD_f64(&a[idx+colid],  gvl);
            _MMR_u64 v_idx_row = __riscv_vle64_v_u64m1(&ja[idx+colid], gvl);
                     v_idx_row = __riscv_vsll_vv_u64m1(v_idx_row, vthree, gvl);
            _MMR_f64 vx        = __riscv_vluxei64_v_f64m1(x, v_idx_row, gvl);
                     vprod     = _MM_MACC_f64(vprod,va, vx, gvl);
        }

        part_res  = _MM_REDSUM_f64(vprod, part_res, gvl);
        y[row]    = _MM_VGETFIRST_f64(part_res);
    }
}
#else
void spmv_serial(const size_t nrows, double *a, uint64_t *ia, uint64_t *ja, double *x, double *y) {
    for (int row=0; row<nrows; row++) {
        double sum = 0;
        for (size_t idx=ia[row]; idx<ia[row+1]; idx++)
        {
            printf("idx: %d ja: %d \n",idx,ja[idx]);
            sum += a[idx] * x[ja[idx]] ;
        }
        y[row] = sum;
    }
}
#endif
