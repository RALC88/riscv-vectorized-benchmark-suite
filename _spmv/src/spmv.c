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
void spmv_intrinsics(const size_t nrows, const size_t ncols, double *a, uint64_t *ia, uint64_t *ja, double *x, double *y) {
    size_t   gvl      = _MMR_VSETVL_E64M1(ncols);
    _MMR_u64 vthree   = _MM_SET_u64(3, gvl);
    _MMR_f64 vprod    = _MM_SET_f64(0, gvl); 
    _MMR_f64 part_res = _MM_SET_f64(0, gvl); 
    
    for (size_t row = 0; row < nrows; row++) {
        uint64_t nnz_row = ia[row + 1] - ia[row];
        uint64_t idx     = ia[row];

        for(size_t colid = 0; colid < nnz_row; colid += gvl ) {
                     gvl       = _MMR_VSETVL_E64M1(nnz_row - colid);
            _MMR_f64 va        = _MM_LOAD_f64(&a[idx+colid],  gvl);
            _MMR_u64 v_idx_row = _MM_LOAD_u64(&ja[idx+colid], gvl);
                     v_idx_row = _MM_SLL_u64(v_idx_row, vthree, gvl);
            _MMR_f64 vx        = _MM_LOAD_INDEX_f64(x, v_idx_row, gvl);
                     vprod     = _MM_MACC_f64(vprod,va, vx, gvl);
        }
        
        gvl       = _MMR_VSETVL_E64M1(ncols);
        part_res  = _MM_REDSUM_f64(vprod, part_res, gvl);
        y[row]    = _MM_VGETFIRST_f64(part_res);
        vprod     = _MM_SET_f64(0, gvl); 
        part_res  = _MM_SET_f64(0, gvl);
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
