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
#include "../../common/riscv_util.h"

void spmv_intrinsics(const size_t nrows, double *a, uint64_t *ia, uint64_t *ja, double *x, double *y); 
void spmv_serial(const size_t nrows, double *a, uint64_t *ia, uint64_t *ja, double *x, double *y); 

int main(int argc, char *argv[]){
   
    FILE *f;
    size_t M, N, NZ; // M:rows N:cols, NZ:non zero values   
    uint64_t row, col;
    double value;
    long long start, end;

    if (argc < 2) {
        printf("Usage: <file.mtx> \n");
        return 1;
    }
    

    if ((f = fopen(argv[1], "r")) == NULL) {
        printf("ERROR: Unable to open file `%s'.\n",argv[1]);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] != '%') {
            sscanf(line, "%zd %zd %zd", &M, &N, &NZ);
            break;
        }
    }
    
    uint64_t *ia = (uint64_t *) calloc((M+1) , sizeof(uint64_t));
    uint64_t *ja = (uint64_t *) malloc(NZ * sizeof(uint64_t));
    double *a = (double *) malloc(NZ * sizeof(double));
    double *x = (double *) malloc(N * sizeof(double));
    double *y = (double *) malloc(M * sizeof(double));

    ia[0]=0;
    size_t currentRow = 0;
    for (size_t i = 0; i < NZ; i++) {
        if (fscanf(f, "%lu %lu %lf", &col, &row, &value) != 3) {
            printf("Error reading file at line %zu\n", i);
            break; 
        }
        ja[i] = col;
        a[i] = value;
        if (row == currentRow+1) {
            currentRow++;
            ia[currentRow]=i;
        }
    }
    ia[M]=NZ;
    
    fclose(f);

    for (size_t i = 0; i < N; i++) x[i] = 1.0 ;

#ifdef USE_RISCV_VECTOR
    start = get_time();
    spmv_intrinsics(M, a, ia, ja, x, y);
    end = get_time();
    printf("spmv_intrinsics time: %f\n", elapsed_time(start, end));
#else // !USE_RISCV_VECTOR
    start = get_time();
    spmv_serial(M, a, ia, ja, x, y); 
    end = get_time();
    printf("spmv_serial time: %f\n", elapsed_time(start, end));
#endif

    free(ia);
    free(ja);
    free(a);
    free(x);
    free(y);

    printf ("done\n");

    return 0;
}
