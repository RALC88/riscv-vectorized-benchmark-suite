/*************************************************************************
** * * * * * * * * *  MATRIX MULTIPLICATION * * * * * * * * * * * * * * ** 
**************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#include "../../common/riscv_util.h"

#define DATA_TYPE 
typedef double data_t;

extern bool compare( size_t dm, size_t dn, data_t *a , data_t *b) ;
#ifdef USE_RISCV_VECTOR
extern void matrixmul_intrinsics( size_t dm, size_t dk, size_t dn, data_t *c , data_t *a, data_t *b ) ;
#else // !USE_RISCV_VECTOR
extern void matmul_serial( size_t dm, size_t dk, size_t dn, data_t *c , data_t *a, data_t *b ) ;
#endif


int main (int argc, char **argv)
{
    FILE *file;
    data_t * buffer;
    int rv;

    if (argc != 4){
        printf("Usage:\n\t%s <Rows> <Colums> <inputFile>\n", argv[0]);
        exit(1);
    }

    int M = atoi(argv[1]);
    int N = atoi(argv[2]);
    int K = N;
    char *inputFile = argv[3];
    int numOptions;

    //Read input data from file
    file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }

    rv = fscanf(file, "%i", &numOptions);
    if(rv != 1) {
      printf("ERROR: Unable to read from file `%s'.\n", inputFile);
      fclose(file);
      exit(1);
    }

    data_t *M1 = (data_t*)malloc(M*N*sizeof(data_t));
    data_t *M2 = (data_t*)malloc(N*K*sizeof(data_t));
    data_t *result = (data_t*)malloc(N*K*sizeof(data_t));
    data_t *reference = (data_t*)malloc(M*N*sizeof(data_t));

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            rv = fscanf(file, "%f", M1[i*N+j]);
            if(rv != M*N) {
              printf("ERROR: Unable to read from file `%s'.\n", inputFile);
              fclose(file);
              exit(1);
            }
        }
 
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            rv = fscanf(file, "%f", M2[i*N+j]);
            if(rv != M*N) {
              printf("ERROR: Unable to read from file `%s'.\n", inputFile);
              fclose(file);
              exit(1);
            }
        }
 
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            rv = fscanf(file, "%f", reference[i*N+j]);
            if(rv != M*N) {
              printf("ERROR: Unable to read from file `%s'.\n", inputFile);
              fclose(file);
              exit(1);
            }
        }
 
    }


    rv = fclose(file);
    if(rv != 0) {
      printf("ERROR: Unable to close file `%s'.\n", inputFile);
      exit(1);
    }

    
    long long start,end;
    start = get_time();


#ifdef USE_RISCV_VECTOR

    matrixmul_intrinsics(M, K, N, result, M1, M2);
    printf("matrixmul_intrinsics done\n");
    
    end = get_time();
    printf("matrixmul_intrinsics time: %f\n", elapsed_time(start, end));

#else // !USE_RISCV_VECTOR

    matmul_serial(M, K, N, result, M1, M2);
    printf("matmul_serial done\n");
    
    end = get_time();
    printf("matmul_serial time: %f\n", elapsed_time(start, end));
#endif

    
    if(compare(M, N, result, reference)){
        printf("Verification failed!\n");
        return 1;
    }


    free(M1);
    free(M2);
    free(result);
    free(reference);

    return 0;
}
