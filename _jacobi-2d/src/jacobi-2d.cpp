/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* jacobi-2d.c: this file is part of PolyBench/C */

/*************************************************************************
* RISC-V Vectorized Version
* Author: Cristóbal Ramírez Lazo
* email: cristobal.ramirez@bsc.es
* Barcelona Supercomputing Center (2020)
*************************************************************************/

#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>


#ifdef USE_RISCV_VECTOR
#include "../../common/vector_defines.h"
#endif

using namespace std;
#define DATA_TYPE double
#define RESULT_PRINT
string outfilename;

/* Array initialization. */
static
void init_array (int n, DATA_TYPE **A, DATA_TYPE **B)
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      {
      A[i][j] = ((DATA_TYPE) i*(j+2) + 2) / n;
      B[i][j] = ((DATA_TYPE) i*(j+3) + 3) / n;
      }
}

#ifdef USE_RISCV_VECTOR
void kernel_jacobi_2d_vector(int tsteps,int n, DATA_TYPE **A,DATA_TYPE **B)
{
    _MMR_f64    xU;
    _MMR_f64    xUtmp;
    _MMR_f64    xUtmp2;
    _MMR_f64    xUleft;
    _MMR_f64    xUright;
    _MMR_f64    xUtop;
    _MMR_f64    xUbottom;
    _MMR_f64    xConstant;

    double diff, sum=0.0;
    double izq,der;
    int size_y = n-2;
    int size_x = n-2;

    unsigned long int gvl = _MMR_VSETVL_E64M1(size_y);

    xConstant = _MM_SET_f64(0.20f,gvl);

    for (int j=1; j<=size_x; j=j+gvl) 
    {
        gvl = _MMR_VSETVL_E64M1(size_y-j+1);

        xUtop = _MM_LOAD_f64(&A[0][j],gvl);
        xU = _MM_LOAD_f64(&A[1][j],gvl);
        xUbottom = _MM_LOAD_f64(&A[2][j],gvl);

        for (int i=1; i<=size_y; i++) 
        {
            if(i!=1)
            {
                xUtop = xU;
                xU =  xUbottom;
                xUbottom =  _MM_LOAD_f64(&A[i+1][j],gvl);
            }
            izq = A[i][j-1]; 
            der = A[i][j+gvl];
            xUleft = _MM_VSLIDE1UP_f64(xU,izq,gvl);
            xUright = _MM_VSLIDE1DOWN_f64(xU,der,gvl);
            xUtmp = _MM_ADD_f64(xU,xUleft,gvl);
            xUtmp = _MM_ADD_f64(xUtmp,xUright,gvl);
            xUtmp2 = _MM_ADD_f64(xUbottom,xUtop,gvl);
            xUtmp = _MM_ADD_f64(xUtmp,xUtmp2,gvl);
            xUtmp = _MM_MUL_f64(xUtmp,xConstant,gvl);
            _MM_STORE_f64(&B[i][j], xUtmp,gvl);
        }
    }
}
#endif

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_jacobi_2d(int tsteps,int n, DATA_TYPE **A,DATA_TYPE **B)
{
  int t, i, j;
#ifndef USE_RISCV_VECTOR
  for (t = 0; t < tsteps; t++)
    {
      for (i = 1; i < n - 1; i++)
       for (j = 1; j < n - 1; j++)
         B[i][j] = (0.2) * (A[i][j] + A[i][j-1] + A[i][1+j] + A[1+i][j] + A[i-1][j]);
      for (i = 1; i < n - 1; i++)
       for (j = 1; j < n - 1; j++)
         A[i][j] = (0.2) * (B[i][j] + B[i][j-1] + B[i][1+j] + B[1+i][j] + B[i-1][j]);
    }
#else
    for (t = 0; t < tsteps; t++)
    {
      kernel_jacobi_2d_vector(tsteps,n, A,B);
      kernel_jacobi_2d_vector(tsteps,n, B,A);
    }
#endif
}


long long get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000000) + tv.tv_usec;
}

// Returns the number of seconds elapsed between the two specified times
double elapsed_time(long long start_time, long long end_time) {
        return (double) (end_time - start_time) / (1000 * 1000);
}


void output_printfile(int n,DATA_TYPE **A,  string& outfile ) {
  ofstream myfile;
  myfile.open(outfile);
  assert(myfile.is_open());

  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      myfile << std::fixed << std::setprecision(4) <<A[i][j] <<" " ;
      if (j == n-1) myfile << A[i][j] <<"\n";
    }

    myfile.close();
}

int main(int argc, char** argv)
{
  if(argc!=4){
        printf("Usage: N TSTEPS output_file\n");
        exit(0);
    }

    int N = atoi(argv[1]);
    int TSTEPS = atoi(argv[2]);
    outfilename = argv[3];
  /* Retrieve problem size. */
  int n = N;
  int tsteps = TSTEPS;

  DATA_TYPE** A;
  DATA_TYPE** B;
  /* Variable declaration/allocation. */
  A = (DATA_TYPE **)malloc(n * sizeof(DATA_TYPE *));
  B = (DATA_TYPE **)malloc(n * sizeof(DATA_TYPE *));
  for (int i = 0; i < n; i++) {
    A[i] = (DATA_TYPE *)malloc(n * sizeof(DATA_TYPE));
    B[i] = (DATA_TYPE *)malloc(n * sizeof(DATA_TYPE));
  }

  /* Initialize array(s). */
  init_array(n,A,B);

  /* Start timer. */
  long long start = get_time();

  /* Run kernel. */
  kernel_jacobi_2d(tsteps, n, A, B);

  // stopping time
  long long end = get_time();
  printf("time: %lf\n", elapsed_time(start, end));
#ifdef RESULT_PRINT
  output_printfile(n,A, outfilename );
#endif  // RESULT_PRINT

  /* Be clean. */
  free(A[0]); free(A);
  free(B[0]); free(B);

  return 0;
}
