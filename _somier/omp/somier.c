#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>
#include "somier.h"


void init_X (int n, double (*X)[n][n][n])
{
   int i, j, k;

   Xcenter[0]=0, Xcenter[1]=0; Xcenter[2]=0;

   for (i = 0; i<n; i++)
      for (j = 0; j<n; j++)
         for (k = 0; k<n; k++) {
           X[0][i][j][k] = i;
           X[1][i][j][k] = j;
           X[2][i][j][k] = k; 

	   Xcenter[0] += X[0][i][j][k];
           Xcenter[1] += X[1][i][j][k];
           Xcenter[2] += X[2][i][j][k];
       }

    Xcenter[0] /= (n*n*n);
    Xcenter[1] /= (n*n*n);
    Xcenter[2] /= (n*n*n);



//   X[n/2][n/2][n/2][0] += 0.5; X[n/2][n/2][n/2][1] += 0.5; X[n/2][n/2][n/2][2] += 0.5; 
//   X[n/2][n/2][n/2][0] += 0.5; X[n/2][n/2][n/2][1] += 0.5; 
//   X[n/2][n/2][n/2][0] += 0.5;  
}

//make sure the boundary nodes are fixed

void boundary(int n, double (*X)[n][n][n], double (*V)[n][n][n])
{
   int i, j, k;
   i = 0;
   for (j = 0; j<n; j++) {
      for (k = 0; k<n; k++) {
         X[0][i][j][k] = i;   X[1][i][j][k] = j;   X[2][i][j][k] = k; 
         V[0][i][j][k] = 0.0; V[1][i][j][k] = 0.0; V[2][i][j][k] = 0.0; 
      }
   }
   j = 0;
   for (i = 0; i<n; i++) {
      for (k = 0; k<n; k++) {
         X[0][i][j][k] = i;   X[1][i][j][k] = j;   X[2][i][j][k] = k; 
         V[0][i][j][k] = 0.0; V[1][i][j][k] = 0.0; V[2][i][j][k] = 0.0; 
      }
   }
   k = 0;
   for (i = 0; i<n; i++) {
      for (j = 0; j<n; j++) {
         X[0][i][j][k] = i;   X[1][i][j][k] = j;   X[2][i][j][k] = k; 
         V[0][i][j][k] = 0.0; V[1][i][j][k] = 0.0; V[2][i][j][k] = 0.0; 
      }
   }
   k = n-1;
   for (i = 0; i<n; i++) {
      for (j = 0; j<n; j++) {
         X[0][i][j][k] = i;   X[1][i][j][k] = j;   X[2][i][j][k] = k; 
         V[0][i][j][k] = 0.0; V[1][i][j][k] = 0.0; V[2][i][j][k] = 0.0; 
      }
   }
   i = n-1;
   for (j = 0; j<n; j++) {
      for (k = 0; k<n; k++) {
         X[0][i][j][k] = i;   X[1][i][j][k] = j;   X[2][i][j][k] = k; 
         V[0][i][j][k] = 0.0; V[1][i][j][k] = 0.0; V[2][i][j][k] = 0.0; 
      }
   }
   j = n-1;
   for (i = 0; i<n; i++) {
      for (k = 0; k<n; k++) {
         X[0][i][j][k] = i;   X[1][i][j][k] = j;   X[2][i][j][k] = k; 
         V[0][i][j][k] = 0.0; V[1][i][j][k] = 0.0; V[2][i][j][k] = 0.0; 
      }
   }
}


inline void acceleration(int n, double (*A)[n][n][n], double (*F)[n][n][n], double M)
{
   int i, j, k;
//#dear compiler: please fuse next two loops if you can 
   for (i = 0; i<n; i++)
      for (j = 0; j<n; j++)
         for (k = 0; k<n; k++) {
            A[0][i][j][k]= F[0][i][j][k]/M;
            A[1][i][j][k]= F[1][i][j][k]/M;
            A[2][i][j][k]= F[2][i][j][k]/M;
	 }

}


inline void velocities(int n, double (*V)[n][n][n], double (*A)[n][n][n], double dt)
{
   int i, j, k;
//#dear compiler: please fuse next two loops if you can 
   for (i = 0; i<n; i++)
//      #pragma omp task
//      #pragma omp unroll
      for (j = 0; j<n; j++) {
	 #pragma omp simd
         for (k = 0; k<n; k++) {
               V[0][i][j][k] += A[0][i][j][k]*dt;
               V[1][i][j][k] += A[1][i][j][k]*dt;
               V[2][i][j][k] += A[2][i][j][k]*dt;
            }
     }
}

void positions(int n, double (*X)[n][n][n], double (*V)[n][n][n], double dt)
{
   int i, j, k;
//#dear compiler: please fuse next two loops if you can 
   for (i = 0; i<n; i++)
      for (j = 0; j<n; j++)
         for (k = 0; k<n; k++) {
               X[0][i][j][k] += V[0][i][j][k]*dt;
               X[1][i][j][k] += V[1][i][j][k]*dt;
               X[2][i][j][k] += V[2][i][j][k]*dt;
            }
}

void compute_stats(int n, double (*X)[n][n][n], double Xcenter[3])
{
   for (int i = 0; i<n; i++) {
      for (int j = 0; j<n; j++) {
         for (int k = 0; k<n; k++) {
             Xcenter[0] += X[0][i][j][k];
             Xcenter[1] += X[1][i][j][k];
             Xcenter[2] += X[2][i][j][k];
         }
      }
   }
   Xcenter[0] /= (n*n*n);
   Xcenter[1] /= (n*n*n);
   Xcenter[2] /= (n*n*n);
}

