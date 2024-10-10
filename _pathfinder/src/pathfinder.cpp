//#include <stdio.h>
//#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*************************************************************************
* RISC-V Vectorized Version
* Author: Cristóbal Ramírez Lazo
* email: cristobal.ramirez@bsc.es
* Barcelona Supercomputing Center (2020)
*************************************************************************/

#include "../../common/riscv_util.h"

#ifdef USE_RISCV_VECTOR
#include <riscv_vector.h>
#include "vector_defines.h"
#endif

/************************************************************************/

//Enable RESULT_PRINT in order to see the result vector, for instruction count it should be disable
//#define RESULT_PRINT
//Enable INPUT_PRINT in order to see the input matrix, for instruction count it should be disable
//#define INPUT_PRINT

#define MAXNAMESIZE 1024 // max filename length
#define M_SEED 9
#define MAX_WEIGHT 10
#define NUM_RUNS 100

size_t rows, cols;
int* wall;
int* result;
int* reference;
string inputfilename;
string outfilename;

void init(int argc, char** argv);
void run();
void run_vector();
void output_print(int *dst, int cols);
int read_matrix_dimensions(FILE *file, size_t *M, size_t *N);
void read_vector(FILE *file, int *vector, size_t size, size_t rowSize);

bool compare( int cols, int* result, int* reference);

void init(int argc, char** argv)
{
    if(argc!=2){
        printf("Usage: <input_file> \n");
        exit(0);
    }

    //Read input data from file
    char *inputFile = argv[1];
    FILE *file = fopen(inputFile, "r");
    if(file == NULL) {
      printf("ERROR: Unable to open file `%s'.\n", inputFile);
      exit(1);
    }

    if (read_matrix_dimensions(file, &rows, &cols)) {
        printf("Error reading the matrix dimensions.\n");
    } else{
        printf("Matrix Dimensions: M %zu, N %zu \n", rows, cols);
    } 

    char line[16];
    wall = new int[rows * cols];
    result = new int[cols];
    reference = new int[cols];

    // Read Matrix
    read_vector(file, wall, rows * cols, cols);

    // Read reference
    fgets(line, sizeof(line), file);  // Read the blank line
    read_vector(file, reference, cols, cols);


#ifdef INPUT_PRINT
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            printf("%d ",wall[i*cols+j]) ;
        }
        printf("\n") ;
    }
#endif
}

void fatal(char *s)
{
    fprintf(stderr, "error: %s\n", s);

}

#define IN_RANGE(x, min, max)   ((x)>=(min) && (x)<=(max))
#define CLAMP_RANGE(x, min, max) x = (x<(min)) ? min : ((x>(max)) ? max : x )
#define MIN(a, b) ((a)<=(b) ? (a) : (b))

int main(int argc, char** argv)
{

long long start_0 = get_time();
init(argc,argv);
long long end_0 = get_time();
printf("TIME TO INIT DATA: %f\n", elapsed_time(start_0, end_0));

#ifndef USE_RISCV_VECTOR
    run();
#else
    run_vector();
#endif

}

#ifndef USE_RISCV_VECTOR

void run()
{
    int min;
    int *src,*dst, *temp;
    
    printf("NUMBER OF RUNS: %d\n",NUM_RUNS);
    long long start = get_time();

    for (int j=0; j<NUM_RUNS; j++) {
        src = new int[cols];
        for (int x = 0; x < cols; x++){
            result[x] = wall[x];
        }

        dst = result;
        for (int t = 0; t < rows-1; t++) {
            temp = src;
            src = dst;
            dst = temp;
            for(int n = 0; n < cols; n++){
              min = src[n];
              if (n > 0)
                min = MIN(min, src[n-1]);
              if (n < cols-1)
                min = MIN(min, src[n+1]);
              dst[n] = wall[(t+1)*cols + n]+min;
            }
        }   
    }

    long long end = get_time();
    printf("TIME TO FIND THE SMALLEST PATH: %f\n", elapsed_time(start, end));

    if(compare(cols, dst, reference)){
        printf("Verification failed!\n");
    } else {
        printf("Verification passed!\n");
    }


#ifdef RESULT_PRINT
    output_print(dst, cols);
#endif  // RESULT_PRINT
    
    free(dst);
    free(wall);
    free(src);
}

#else // USE_RISCV_VECTOR

void run_vector()
{
    int *dst;

    long long start = get_time();
    printf("NUMBER OF RUNS: %d\n",NUM_RUNS);

    for (int j=0; j<NUM_RUNS; j++) {
        for (int x = 0; x < cols; x++){
            result[x] = wall[x];
        }
        dst = result;

        size_t gvl = __riscv_vsetvl_e32m1(cols);

        _MMR_i32    xSrc_slideup;
        _MMR_i32    xSrc_slidedown;
        _MMR_i32    xSrc;
        _MMR_i32    xNextrow; 

        int aux,aux2;

        for (size_t t = 0; t < rows-1; t++) 
        {
            aux = dst[0] ;
            for(size_t n = 0; n < cols; n = n + gvl)
            {
                gvl = __riscv_vsetvl_e32m1(cols-n);
                xNextrow = _MM_LOAD_i32(&dst[n],gvl);
                xSrc = xNextrow;
                aux2 = (n+gvl >= cols) ?  dst[n+gvl-1] : dst[n+gvl];
                xSrc_slideup = _MM_VSLIDE1UP_i32(xSrc,aux,gvl);
                xSrc_slidedown = _MM_VSLIDE1DOWN_i32(xSrc,aux2,gvl);
                xSrc = _MM_MIN_i32(xSrc,xSrc_slideup,gvl);
                xSrc = _MM_MIN_i32(xSrc,xSrc_slidedown,gvl);
                xNextrow = _MM_LOAD_i32(&wall[(t+1)*cols + n],gvl);
                xNextrow = _MM_ADD_i32(xNextrow,xSrc,gvl);
                aux = dst[n+gvl-1];
                _MM_STORE_i32(&dst[n],xNextrow,gvl);
            }
        }
    }
    long long end = get_time();
    printf("TIME TO FIND THE SMALLEST PATH: %f\n", elapsed_time(start, end));


    if(compare(cols, dst, reference)){
        printf("Verification failed!\n");
    } else {
        printf("Verification passed!\n");
    }



#ifdef RESULT_PRINT
    output_print(dst, cols);
#endif // RESULT_PRINT

    free(wall);
    free(dst);
}
#endif // USE_RISCV_VECTOR

void output_print(int *dst, int cols) {

    for (int j = 0; j < cols; j++){
        printf("%d ",dst[j]) ;
    }
        printf("\n") ;
}

bool compare( int cols, int *result, int *reference){

    bool error = 0;
    for (int j = 0; j < cols; j++){
        if(result[j] != reference[j]){
            error = 1;
        }
        return error;
    }
    return error;
}

void read_vector(FILE *file, int *vector, size_t size, size_t rowSize) {
    int *ptr = vector;
    int index = 0;
    int value;

    while (index < size) {
        // Read ELEMENTS_PER_LINE values from each line
        for (int i = 0; i < rowSize && index < size ; i++) {
            if (fscanf(file, "%d", &value) != 1) {
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

int read_matrix_dimensions(FILE *file, size_t *M, size_t *N) {
    char line[100];  // Buffer to store the line read from the file

    // Read a line from the file
    if (fgets(line, sizeof(line), file) != NULL) {
        // Parse the dimensions using sscanf
        if (sscanf(line, "%zd %zd", M, N) != 0) {
            return 0;  // Successfully read all dimensions
        } else {
            return 1;  // Error parsing the dimensions
        }
    } else {
        return 1;  // Error reading the line
    }
}
