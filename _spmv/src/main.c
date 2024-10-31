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
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "../../common/riscv_util.h"
#define TOLERANCE 1e-6

void spmv_intrinsics(const size_t nrows, double *a, uint64_t *ia, uint64_t *ja, double *x, double *y); 
void spmv_serial(const size_t nrows, double *a, uint64_t *ia, uint64_t *ja, double *x, double *y); 

int main(int argc, char *argv[]){
   
    FILE *f;
    size_t M, N, NZ; // M:rows N:cols, NZ:non zero values   
    uint64_t row, col;
    double value;
    long long start, end;
    bool verification = false;
    
    if (argc < 2) {
        printf("Usage: <file.mtx> <file.verif> \n *.verif is optional \n");
        return 1;
    }
    
    if (argv[2] != NULL) verification = true;

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
        if (row != currentRow){
            int diff = row - currentRow;
            for(int j=0;j < diff; j++) {
                currentRow++;
                ia[currentRow]=i;
            }
        } 
    }
    
    if(currentRow < M-1){
        int diff = M - currentRow;
        for(int j=0;j < diff; j++) {
            currentRow++;
            ia[currentRow]=NZ;
        }
    }
    ia[M]=NZ;

    
    fclose(f);
        
    double *verif = (double *) malloc(M * sizeof(double));

    if(verification == true){
        if ((f = fopen(argv[2], "r")) == NULL) {
            printf("ERROR: Unable to open file `%s'.\n",argv[2]);
            return 1;
        }

        for (size_t i = 0; i < M; i++) {
            if (fscanf(f, "%lf,", &verif[i]) != 1) {
                printf("Error reading file at line %zu\n", i);
                free(verif);
                fclose(f);
                return 1;
            }
        }
        fclose(f);
    }

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

    if(verification == true){
        for(size_t i=0; i < M ; i++){
            if (fabs(y[i] - verif[i]) > TOLERANCE) {
                printf("Verification fail \n");
                printf("%.17lf  -  %.17lf \n",y[i],verif[i]);
                return i+1;
            }
        }
        printf("Verification pass \n");
    }

    free(ia);
    free(ja);
    free(a);
    free(x);
    free(y);
    free(verif);

    printf ("done\n");

    return 0;
}
