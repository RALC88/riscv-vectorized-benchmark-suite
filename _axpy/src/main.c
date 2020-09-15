#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "utils.h" 

/*************************************************************************
*GET_TIME
*returns a long int representing the time
*************************************************************************/

#include <time.h>
#include <sys/time.h>

long long get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}
// Returns the number of seconds elapsed between the two specified times
float elapsed_time(long long start_time, long long end_time) {
        return (float) (end_time - start_time) / (1000 * 1000);
}
/*************************************************************************/

void axpy_intrinsics(double a, double *dx, double *dy, int n); 

// Ref version
void axpy_ref(double a, double *dx, double *dy, int n) {
   int i;
   for (i=0; i<n; i++) {
      dy[i] += a*dx[i];
   }
}

void init_vector(double *pv, long n, double value)
{
   for (int i=0; i<n; i++) pv[i]= value;
//   int gvl = __builtin_epi_vsetvl(n, __epi_e64, __epi_m1);
//   __epi_1xi64 v_value   = __builtin_epi_vbroadcast_1xi64(value, gvl);
//   for (int i=0; i<n; ) {
//    gvl = __builtin_epi_vsetvl(n - i, __epi_e64, __epi_m1);
//      __builtin_epi_vstore_1xf64(&dx[i], v_res, gvl);
//     i += gvl;
//   }
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

    printf ("doing reference axpy\n");
    start = get_time();
    axpy_ref(a, dx, dy, n); 
    end = get_time();
    printf("axpy_reference time: %f\n", elapsed_time(start, end));

    capture_ref_result(dy, dy_ref, n);
    init_vector(dx, n, 1.0);
    init_vector(dy, n, 2.0);

    printf ("doing vector axpy\n");
    start = get_time();
    axpy_intrinsics(a, dx, dy, n);
    end = get_time();
    printf("axpy_intrinsics time: %f\n", elapsed_time(start, end));
    
    printf ("done\n");
    test_result(dy, dy_ref, n);


    free(dx); free(dy); free(dy_ref);
    return 0;
}
