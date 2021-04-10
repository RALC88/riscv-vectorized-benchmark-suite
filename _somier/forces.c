#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>
#include "somier.h"

void force_contribution(int n, double (*X)[n][n][n], double (*F)[n][n][n],
                   int i, int j, int k, int neig_i, int neig_j, int neig_k)
{
   double dx, dy, dz, dl, spring_F, FX, FY,FZ;

   assert (i >= 1); assert (j >= 1); assert (k >= 1);
   assert (i <  n-1); assert (j <  n-1); assert (k <  n-1);
   assert (neig_i >= 0); assert (neig_j >= 0); assert (neig_k >= 0);
   assert (neig_i <  n); assert (neig_j <  n); assert (neig_k <  n);

   dx=X[0][neig_i][neig_j][neig_k]-X[0][i][j][k];
   dy=X[1][neig_i][neig_j][neig_k]-X[1][i][j][k];
   dz=X[2][neig_i][neig_j][neig_k]-X[2][i][j][k];
   dl = sqrt(dx*dx + dy*dy + dz*dz);
   spring_F = 0.25 * spring_K*(dl-1);
   FX = spring_F * dx/dl; 
   FY = spring_F * dy/dl;
   FZ = spring_F * dz/dl; 
   F[0][i][j][k] += FX;
   F[1][i][j][k] += FY;
   F[2][i][j][k] += FZ;
}

void compute_forces(int n, double (*X)[n][n][n], double (*F)[n][n][n])
{
   for (int i=1; i<n-1; i++) {
      for (int j=1; j<n-1; j++) {
         for (int k=1; k<n-1; k++) {
            force_contribution (n, X, F, i, j, k, i-1, j,   k);
            force_contribution (n, X, F, i, j, k, i+1, j,   k);
            force_contribution (n, X, F, i, j, k, i,   j-1, k);
            force_contribution (n, X, F, i, j, k, i,   j+1, k);
            force_contribution (n, X, F, i, j, k, i,   j,   k-1);
            force_contribution (n, X, F, i, j, k, i,   j,   k+1);
         }
      }
   }
}



#if 0
//COMPARAR !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void force_contribution(int n, double (*X)[n][n][n], double (*F)[n][n][n],
                   int i, int j, int k, int neig_i, int neig_j, int neig_k)
{
   double dx, dy, dz, dl, spring_F, FX, FY,FZ;
   double sign;

   assert (i >= 1); assert (j >= 1); assert (k >= 1);
   assert (i <  n-1); assert (j <  n-1); assert (k <  n-1);
   assert (neig_i >= 0); assert (neig_j >= 0); assert (neig_k >= 0);
   assert (neig_i <  n); assert (neig_j <  n); assert (neig_k <  n);

   dx=X[0][neig_i][neig_j][neig_k]-X[0][i][j][k];
   dy=X[1][neig_i][neig_j][neig_k]-X[1][i][j][k];
   dz=X[2][neig_i][neig_j][neig_k]-X[2][i][j][k];
   dl = sqrt(dx*dx + dy*dy + dz*dz);
   sign = -1.0;
   sign = (neig_i > i ? sign : 1.0);
   sign = (neig_j > j ? sign : 1.0);
   sign = (neig_k > k ? sign : 1.0);
   spring_F = 0.25 * spring_K*(dl-1);
   FX = sign * spring_F * dx/dl;
   FY = sign * spring_F * dy/dl;
   FZ = sign * spring_F * dz/dl;
   F[0][i][j][k] += FX;
   F[1][i][j][k] += FY;
   F[2][i][j][k] += FZ;
   //if (SI==n/2 && SJ==n/2 && SK==n/2)
       //printf ("contribution to forces: %d,%d,%d <- %d,%d,%d: %3.3f %3.3f %3.3f\n", i,j,k,neig_i,neig_j,neig_k, FX, FY, FZ);
}
#endif

