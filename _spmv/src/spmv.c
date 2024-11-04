/*                                  
 * Neiel Israel Leyva Santes
 * neiel.leyva@bsc.es
 * Barcelona Supercomputing Center
 *
 * SpMV Vector Implementation.
 * Sparse Matrix-Vector Multiplication (SpMV) is a mathematical operation 
 * in which a sparse matrix is multiplied by a dense vector.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../../common/riscv_util.h"
#ifdef USE_RISCV_VECTOR
#include <riscv_vector.h>
#include "vector_defines.h"
#endif

#ifdef USE_RISCV_VECTOR
void spmv_intrinsics(const size_t nrows, double *a, uint64_t *ia, uint64_t *ja, double *x, double *y) {
    
    _MMR_f64 va;
    _MMR_u64 v_idx_row;
    _MMR_f64 vx;
    _MMR_f64 vprod;
    _MMR_f64 part_res;

    for (size_t row = 0; row < nrows; row++) {
        int64_t nnz_row = ia[row + 1] - ia[row];
        int64_t idx     = ia[row];

        if(nnz_row == 0) {
            y[row] = 0; 
        } else {
            size_t gvl = _MMR_VSETVL_E64M1(nnz_row);
            vprod    = _MM_SET_f64(0.0, gvl);
            part_res = _MM_SET_f64(0.0, 1);

            for(size_t colid = 0; colid < nnz_row; colid += gvl ) {
                gvl       = _MMR_VSETVL_E64M1(nnz_row - colid);
                va        = _MM_LOAD_f64(&a[idx+colid],  gvl);
                v_idx_row = _MM_LOAD_u64(&ja[idx+colid], gvl);
                v_idx_row = _MM_SLL_VX_u64(v_idx_row, 3, gvl);
                vx        = _MM_LOAD_INDEX_f64(x, v_idx_row, gvl);
                vprod     = _MM_MACC_f64(vprod,va, vx, gvl);
            }
            
            gvl       = _MMR_VSETVL_E64M1(nnz_row);
            part_res  = _MM_REDSUM_f64(vprod, part_res, gvl);
            y[row]    = _MM_VGETFIRST_f64(part_res);
        }
    }
}
#else
void spmv_serial(const size_t nrows, double *a, uint64_t *ia, uint64_t *ja, double *x, double *y) {
    for (int row=0; row<nrows; row++) {
        double sum = 0;
        for (size_t idx=ia[row]; idx<ia[row+1]; idx++){
            sum += a[idx] * x[ja[idx]] ;
        }
        y[row] = sum;
    }
}
#endif
