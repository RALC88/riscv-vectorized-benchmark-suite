/*************************************************************************
* Axpy Kernel
* Author: Jesus Labarta
* Barcelona Supercomputing Center
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "utils.h" 

#include "../../common/riscv_util.h"

/*************************************************************************/

void axpy_intrinsics(double a, double *dx, double *dy, int n); 
void axpy_serial(double a, double *dx, double *dy, int n); 


void init_vector(double *pv, long n, double value)
{
   for (int i=0; i<n; i++) pv[i]= value;
}

int main(int argc, char *argv[])
{
    long long start,end;
    start = get_time();

    double a=1.0;
    long n;

    if (argc == 2)
    n = 1024*atol(argv[1]); // input argument: vector size in Ks
    else
        n = (30*1024);

    /* Allocate the source and result vectors */
    double *dx     = (double*)malloc(n*sizeof(double));
    double *dy     = (double*)malloc(n*sizeof(double));
    double *dy_ref = (double*)malloc(n*sizeof(double));

    init_vector(dx, n, 1.0);
    init_vector(dy, n, 2.0);
    
    end = get_time();
    printf("init_vector time: %f\n", elapsed_time(start, end));

    printf ("doing reference axpy , vector size %d\n",n);
    start = get_time();

    axpy_serial(a, dx, dy, n); 

    end = get_time();
    printf("axpy_serial time: %f\n", elapsed_time(start, end));

    capture_ref_result(dy, dy_ref, n);
    init_vector(dx, n, 1.0);
    init_vector(dy, n, 2.0);

    printf ("doing vector axpy, vector size %d\n",n);
    start = get_time();

    axpy_intrinsics(a, dx, dy, n);

    end = get_time();
    printf("axpy_intrinsics time: %f\n", elapsed_time(start, end));

    printf ("done\n");
    test_result(dy, dy_ref, n);

    free(dx); free(dy); free(dy_ref);
    return 0;
}