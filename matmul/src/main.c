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

void read_vector(FILE *file, double *vector, size_t size);
extern bool compare( size_t dm, size_t dn, data_t *a , data_t *b) ;
#ifdef USE_RISCV_VECTOR
extern void matrixmul_intrinsics( size_t dm, size_t dk, size_t dn, data_t *c , data_t *a, data_t *b ) ;
#else // !USE_RISCV_VECTOR
extern void matmul_serial( size_t dm, size_t dk, size_t dn, data_t *c , data_t *a, data_t *b ) ;
#endif


int main (int argc, char **argv)
{
    if (argc != 4){
        printf("Usage:\n\t%s <Rows> <Colums> <inputFile>\n", argv[0]);
        exit(1);
    }

    size_t M = atoi(argv[1]);
    size_t N = atoi(argv[2]);
    size_t K = N;
    char *inputFile = argv[3];
    
    data_t *M1          = (data_t*)malloc(M*N*sizeof(data_t));
    data_t *M2          = (data_t*)malloc(N*K*sizeof(data_t));
    data_t *result      = (data_t*)malloc(N*K*sizeof(data_t));
    data_t *reference   = (data_t*)malloc(M*N*sizeof(data_t));

    //Read input data from file
    FILE *file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }
    
    char line[256];
    
    // Read Matrix A
    fgets(line, sizeof(line), file);  // Read the header line
    read_vector(file, M1, M*N);

    // Read Matrix B
    fgets(line, sizeof(line), file);  // Read the header line
    fgets(line, sizeof(line), file);  // Read the blank line
    read_vector(file, M2, N*K);

    // Read Matrix Reference
    fgets(line, sizeof(line), file);  // Read the header line
    fgets(line, sizeof(line), file);  // Read the blank line
    read_vector(file, reference, M*N);

    fclose(file);

    //**************************************************
    
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

void read_vector(FILE *file, double *vector, size_t size) {
    double *ptr = vector;
    int index = 0;
    double value;

    while (index < size) {
        // Read ELEMENTS_PER_LINE values from each line
        for (int i = 0; i < 20 && index < size; i++) {
            if (fscanf(file, "%lf", &value) != 1) {
                fprintf(stderr, "Error reading value\n");
                exit(EXIT_FAILURE);
            }
            *(ptr + index++) = value;
        }
        // Handle the newline character if present
        int ch = fgetc(file);
        if (ch != '\n' && ch != EOF) {
            ungetc(ch, file);
        }
    }
}

