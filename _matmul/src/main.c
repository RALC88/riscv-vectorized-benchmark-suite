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

int read_matrix_dimensions(FILE *file, size_t *M, size_t *K, size_t *N);
void read_vector(FILE *file, double *vector, size_t size, size_t rowSize);
extern bool compare( size_t dm, size_t dn, data_t *a , data_t *b) ;
#ifdef USE_RISCV_VECTOR
extern void matrixmul_intrinsics(data_t *a, data_t *b, data_t *c, int n, int m, int p) ;
#else // !USE_RISCV_VECTOR
extern void matmul_serial(data_t *a, data_t *b, data_t *c, int n, int m, int p);
#endif


int main (int argc, char **argv)
{
    if (argc != 2){
        printf("Usage:\n\t%s <inputFile>\n", argv[0]);
        exit(1);
    }
    
    //Read input data from file
    char *inputFile = argv[1];
    FILE *file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }
   
    size_t M, K, N; 
    char line[16];
    
    if (read_matrix_dimensions(file, &M, &K, &N)) {
        printf("Error reading the matrix dimensions.\n");
    } else{
        printf("Matrix Dimensions: M %zu, K %zu, N %zu \n", M, K, N);
    } 
    
    data_t *M1          = (data_t*)malloc(M*K*sizeof(data_t));
    data_t *M2          = (data_t*)malloc(K*N*sizeof(data_t));
    data_t *result      = (data_t*)malloc(M*N*sizeof(data_t));
    data_t *reference   = (data_t*)malloc(M*N*sizeof(data_t));
    
    // Read Matrix A
    read_vector(file, M1, M*K, K);

    // Read Matrix B
    fgets(line, sizeof(line), file);  // Read the blank line
    read_vector(file, M2, K*N, N);

    // Read Matrix Reference
    fgets(line, sizeof(line), file);  // Read the blank line
    read_vector(file, reference, M*N, N);

    fclose(file);

    //**************************************************
    
    long long start,end;
    start = get_time();


#ifdef USE_RISCV_VECTOR

    matrixmul_intrinsics(M1, M2, result, N, M, K);
    printf("matrixmul_intrinsics done\n");
    
    end = get_time();
    printf("matrixmul_intrinsics time: %f\n", elapsed_time(start, end));

#else // !USE_RISCV_VECTOR

    matmul_serial(M1,M2,result, N, M, K);
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

void read_vector(FILE *file, double *vector, size_t size, size_t rowSize) {
    double *ptr = vector;
    int index = 0;
    double value;

    while (index < size) {
        // Read ELEMENTS_PER_LINE values from each line
        for (int i = 0; i < rowSize && index < size ; i++) {
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

int read_matrix_dimensions(FILE *file, size_t *M, size_t *K, size_t *N) {
    char line[100];  // Buffer to store the line read from the file

    // Read a line from the file
    if (fgets(line, sizeof(line), file) != NULL) {
        // Parse the dimensions using sscanf
        if (sscanf(line, "%zd %zd %zd", M, K, N) != 0) {
            return 0;  // Successfully read all dimensions
        } else {
            return 1;  // Error parsing the dimensions
        }
    } else {
        return 1;  // Error reading the line
    }
}
