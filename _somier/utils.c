#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>
//#include "utils.h"


void capture_ref_result(double *y, double* y_ref, int n)
{
   int i;
   //printf ("\nReference result: ");
   for (i=0; i<n; i++) {
      y_ref[i]=y[i];
      //printf (" %f", y[i]);
   }
   //printf ("\n\n");
}

void print_4D (int n, char*name, double (*y)[n][n][n])
{
   int i,j,k, dim;
   for (dim=0; dim<3; dim++) {
      for (i = 0; i<n; i++) {
         for (j = 0; j<n; j++) {
            printf("\n%s[%d][%d][%d][0..%d] ", name, dim, i, j, n-1);
            for (k = 0; k<n; k++) {
               printf("%.4f ", y[dim][i][j][k]);
            }
         }
      }
   }
   printf ("\n");
}

void capture_4D_ref (int n, double (*y)[n][n][n], double (*y_ref)[n][n][n])
{
   int i,j,k, dim;
   for (dim=0; dim<3; dim++) {
      for (i = 0; i<n; i++) {
         for (j = 0; j<n; j++) {
            for (k = 0; k<n; k++) {
               y_ref[dim][i][j][k] = y[dim][i][j][k];
            }
         }
      }
   }
}

void test_4D_result(int n, double (*y)[n][n][n], double (*y_ref)[n][n][n])
{
   int nerrs=0;
   int i,j,k, dim;

   for (dim=0; dim<3; dim++) {
      for (i = 0; i<n; i++) {
         for (j = 0; j<n; j++) {
            for (k = 0; k<n; k++) {
               double error = y[dim][i][j][k] - y_ref[dim][i][j][k];

               if (fabs(error) > 0.0000001)  {
                  printf("F[%d][%d][%d][%d]=%.16f != F_ref[%d][%d][%d][%d]=%.16f  INCORRECT RESULT (abs error=%.16f) !!!! \n ",
                      dim,i,j,k,y[dim][i][j][k], dim,i,j,k, y_ref[dim][i][j][k], error);
                  nerrs++;
               }
               if (nerrs == 100) break;
            }
         }
      }
   }
   if (nerrs == 0) printf ("Result ok !!!\n");
}

void clear_4D(int n, double (*X)[n][n][n])
{
   int i, j, k;
   for (i = 0; i<n; i++) 
      for (j = 0; j<n; j++)
         for (k = 0; k<n; k++) { 
            X[0][i][j][k] = 0.0; X[1][i][j][k] = 0.0; X[2][i][j][k] = 0.0;
         }
}

void print_prv_header() {
//   if ((output_fp = fopen("somier.prv", "w")) == NULL) {
//        err = errno;
//        fprintf(stderr, "%s: %s\n", "somier.prv", strerror(err));
//        exit(EXIT_FAILURE);
//   }
//   fprintf(output_fp,"#Paraver (27/06/2017 at 16:16):%d_ns:1(1):1:1:(1:1)\n", (int)(ntsteps*dt*1000));
//   i=j=k=N/2;
//   fprintf(output_fp, "2:%d:1:%d:1:%d:%d:%d:%d:%d:%d:%d\n", 1, 1, (int)((double)nt*dt*1000), 80000000, (int)(X[i][j][k][0]*1000), 80000001,(int)(X[i][j][k][1]*1000), 80000002, (int)(X[i][j][k][2]*1000));
//   fprintf(output_fp, "2:%d:1:%d:1:%d:%d:%d:%d:%d:%d:%d\n", 1, 1, (int)((double)nt*dt*1000), 90000000, (int)(V[i][j][k][0]*1000)+1000, 90000001, (int)(V[i][j][k][1]*1000)+1000, 90000002, (int)(V[i][j][k][2]*1000)+1000);
}

void print_prv_record() {
//      i=j=k=N/2;
//      fprintf(output_fp, "2:%d:1:%d:1:%d:%d:%d:%d:%d:%d:%d\n", 1, 1, (int)((double)nt*dt*1000), 80000000, (int)(X[i][j][k][0]*1000), 80000001,(int)(X[i][j][k][1]*1000), 80000002, (int)(X[i][j][k][2]*1000));
//      fprintf(output_fp, "2:%d:1:%d:1:%d:%d:%d:%d:%d:%d:%d\n", 1, 1, (int)((double)nt*dt*1000), 90000000, (int)(V[i][j][k][0]*1000)+1000, 90000001, (int)(V[i][j][k][1]*1000)+1000, 90000002, (int)(V[i][j][k][2]*1000)+1000);
}


void  print_state(int n, double (*X)[n][n][n], double Xcenter[3], int nt){
   printf ("t=%d\t", nt);
   printf ("XC= %f,%f,%f X[n/2-1] = %f,%f,%f  X[n/2] = %f,%f,%f V[n/2+1] = %f,%f,%f \n",
            Xcenter[0], Xcenter[1], Xcenter[2],
            X[0][n/2-1][n/2][n/2], X[1][n/2-1][n/2][n/2], X[2][n/2-1][n/2][n/2],
            X[0][n/2][n/2][n/2], X[1][n/2][n/2][n/2], X[2][n/2][n/2][n/2],
            X[0][n/2+1][n/2][n/2], X[1][n/2+1][n/2][n/2], X[2][n/2+1][n/2][n/2]);
//            F[0][n/2][n/2][n/2], F[1][n/2][n/2][n/2], F[2][n/2][n/2][n/2],
//            V[0][n/2][n/2][n/2], V[1][n/2][n/2][n/2], V[2][n/2][n/2][n/2]);
//        printf ("\n ");
}


