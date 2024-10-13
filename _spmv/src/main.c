#include <stdlib.h>
#include <stdio.h>

#include "../../common/riscv_util.h"

/*************************************************************************/

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
    
    uint64_t *ia = (uint64_t *) malloc(M+1 * sizeof(uint64_t));
    uint64_t *ja = (uint64_t *) malloc(NZ * sizeof(uint64_t));
    double *a = (double *) malloc(NZ * sizeof(double));
    double *x = (double *) malloc(N * sizeof(double));
    double *y = (double *) calloc(M, sizeof(double));

    uint64_t readRow;
    size_t currentRow = 0;
    ia[0]=0;
    for (size_t i = 0; i < NZ; i++) {
        if (fscanf(f, "%llu %llu %lf", &col, &row, &value) != 3) {
            printf("Error reading file at line %zu\n", i);
            break; 
        }

        ja[i] = col;
        a[i] = value;
        readRow = row; 
        if (readRow == currentRow+1) {
	        currentRow++;
	        ia[currentRow]=i;
	    }
        if(ja[0] != 1) 
        {
            printf("i: %ld ja[0]: %ld col: %ld value: %lf xcol: %ld ycol: %ld  \n",i,ja[0],col,value,ja[i-1],ja[i]);
            return 0;
        }
    
    }
    
    ia[currentRow++]=NZ;

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

    printf ("done\n");

    return 0;
}
