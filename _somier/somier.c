/*************************************************************************
* Somier - RISC-V Vectorized version
* Author: Jesus Labarta
* Barcelona Supercomputing Center
*************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>


#include "utils.h"

double ref[1000][3];
double Xcenter[3];

double dt=0.001;   // 0.1;
double M=1.0;
double spring_K=10.0;
int err;

#if 0
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
#endif


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

void force_contribution(int n, double (*X)[n][n][n], double (*F)[n][n][n],
                   int i, int j, int k, int neig_i, int neig_j, int neig_k);
#if 0
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

void compute_force_new(int n, double (*X)[n][n][n], double (*F)[n][n][n])
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


int main(int argc, char *argv[])
{
   int nt, i, j, k;
   int ntsteps = 20;
   int N;
//   FILE *output_fp;

   ntsteps  = atoi(argv[1]);  //TODO: better parsing
   N        = atoi(argv[2]);  //TODO: better parsing
   printf ("Problem size = %d, steps = %d\n", N, ntsteps);

   double (*X)[N][N][N];
   double (*V)[N][N][N];
   double (*A)[N][N][N];
   double (*F)[N][N][N];
   double (*F_ref)[N][N][N];

   X      = malloc(3*sizeof (*X));
   V      = malloc(3*sizeof (*V));
   A      = malloc(3*sizeof (*A));
   F      = malloc(3*sizeof (*F));
   F_ref  = malloc (3*sizeof (*F_ref));

   clear_4D(N, F);
   clear_4D(N, A);
   clear_4D(N, V);
   clear_4D(N, F_ref);
   init_X(N, X);
//   printf ("\nInitial Positions\n"); print_4D(N, "X", X); printf ("\n\n");
   printf("Set initial speed\n");
   V[0][N/2][N/2][N/2] = 0.1;  V[1][N/2][N/2][N/2] = 0.1;  V[2][N/2][N/2][N/2] = 0.1;  

//   if ((output_fp = fopen("somier.prv", "w")) == NULL) {
//        err = errno;
//        fprintf(stderr, "%s: %s\n", "somier.prv", strerror(err));
//        exit(EXIT_FAILURE);
//   }
//   fprintf(output_fp,"#Paraver (27/06/2017 at 16:16):%d_ns:1(1):1:1:(1:1)\n", (int)(ntsteps*dt*1000));
//   i=j=k=N/2; 
//   fprintf(output_fp, "2:%d:1:%d:1:%d:%d:%d:%d:%d:%d:%d\n", 1, 1, (int)((double)nt*dt*1000), 80000000, (int)(X[i][j][k][0]*1000), 80000001,(int)(X[i][j][k][1]*1000), 80000002, (int)(X[i][j][k][2]*1000));
//   fprintf(output_fp, "2:%d:1:%d:1:%d:%d:%d:%d:%d:%d:%d\n", 1, 1, (int)((double)nt*dt*1000), 90000000, (int)(V[i][j][k][0]*1000)+1000, 90000001, (int)(V[i][j][k][1]*1000)+1000, 90000002, (int)(V[i][j][k][2]*1000)+1000);


   for (nt=0; nt <ntsteps-1; nt++) {

      if(nt%10 == 0) {
//      capture_ref_result();
        printf ("t=%d ", nt);
        printf ("XC= %f,%f,%f X[N/2-1] = %f,%f,%f  X[N/2] = %f,%f,%f V[N/2+1] = %f,%f,%f \n", 
                         Xcenter[0], Xcenter[1], Xcenter[2], 
			 X[0][N/2-1][N/2][N/2], X[1][N/2-1][N/2][N/2], X[2][N/2-1][N/2][N/2], 
			 X[0][N/2][N/2][N/2], X[1][N/2][N/2][N/2], X[2][N/2][N/2][N/2], 
			 X[0][N/2+1][N/2][N/2], X[1][N/2+1][N/2][N/2], X[2][N/2+1][N/2][N/2]); 
//			 F[0][N/2][N/2][N/2], F[1][N/2][N/2][N/2], F[2][N/2][N/2][N/2], 
//			 V[0][N/2][N/2][N/2], V[1][N/2][N/2][N/2], V[2][N/2][N/2][N/2]); 
//        printf ("\n ");
      }

//      boundary(N, X, V);
//      printf ("\nCorrected Positions\n"); print_4D(N, "X", X); printf ("\n");
      Xcenter[0]=0, Xcenter[1]=0; Xcenter[2]=0;   //reset aggregate stats



/*	if(V[N/2][N/2][N/2][2] > 0.7999) { 
		printf ("t=%d\t", nt);
		printf ("V= %f,%f,%f X= %f,%f,%f XC= %f,%f,%f\n", V[N/2][N/2][N/2][0], V[N/2][N/2][N/2][1], V[N/2][N/2][N/2][2], X[N/2][N/2][N/2][0], X[N/2][N/2][N/2][1], X[N/2][N/2][N/2][2], Xcenter[0], Xcenter[1], Xcenter[2]);

	}*/

      clear_4D(N, F);
      compute_force_new(N, X, F);
//      capture_4D_ref(N, F, F_ref);
//      test_4D_result(N, F, F_ref) ;
//      printf ("Computed forces\n"); print_4D(N, "F", F); printf ("\n");

      for (i = 0; i<N; i++)
         for (j = 0; j<N; j++)
            for (k = 0; k<N; k++) {
               A[0][i][j][k]= F[0][i][j][k]/M;
               A[1][i][j][k]= F[1][i][j][k]/M;
               A[2][i][j][k]= F[2][i][j][k]/M;
            }
//      printf ("Computed Accelerations\n"); print_4D(N, "A", A); printf ("\n");

      for (i = 0; i<N; i++)
         for (j = 0; j<N; j++)
            for (k = 0; k<N; k++) {
               V[0][i][j][k] += A[0][i][j][k]*dt;
               V[1][i][j][k] += A[1][i][j][k]*dt;
               V[2][i][j][k] += A[2][i][j][k]*dt;
            }
//      printf ("Computed Velocities\n"); print_4D(N, "V", V); printf ("\n");

      for (i = 0; i<N; i++)
         for (j = 0; j<N; j++)
            for (k = 0; k<N; k++) {
               X[0][i][j][k] += V[0][i][j][k]*dt;
               X[1][i][j][k] += V[1][i][j][k]*dt;
               X[2][i][j][k] += V[2][i][j][k]*dt;
            }
//      printf ("Computed Positions\n"); print_4D(N, "X", X); printf ("\n");

// compute statistics
      for (i = 0; i<N; i++)
         for (j = 0; j<N; j++)
            for (k = 0; k<N; k++) {
		Xcenter[0] += X[0][i][j][k];
                Xcenter[1] += X[1][i][j][k];
                Xcenter[2] += X[2][i][j][k];
            }

      Xcenter[0] /= (N*N*N);
      Xcenter[1] /= (N*N*N);
      Xcenter[2] /= (N*N*N);
		


//      printf ("X+1= %3.3f,%3.3f,%3.3f\n", X[N/2][N/2][N/2][0], X[N/2][N/2][N/2][1], X[N/2][N/2][N/2][2]); 
//      printf ("V= %3.3f,%3.3f,%3.3f\t X+1= %3.3f,%3.3f,%3.3f\n", V[N/2][N/2][N/2][0], V[N/2][N/2][N/2][1], V[N/2][N/2][N/2][2], X[N/2][N/2][N/2][0], X[N/2][N/2][N/2][1], X[N/2][N/2][N/2][2]); 
//      i=j=k=N/2; 
//      fprintf(output_fp, "2:%d:1:%d:1:%d:%d:%d:%d:%d:%d:%d\n", 1, 1, (int)((double)nt*dt*1000), 80000000, (int)(X[i][j][k][0]*1000), 80000001,(int)(X[i][j][k][1]*1000), 80000002, (int)(X[i][j][k][2]*1000));
//      fprintf(output_fp, "2:%d:1:%d:1:%d:%d:%d:%d:%d:%d:%d\n", 1, 1, (int)((double)nt*dt*1000), 90000000, (int)(V[i][j][k][0]*1000)+1000, 90000001, (int)(V[i][j][k][1]*1000)+1000, 90000002, (int)(V[i][j][k][2]*1000)+1000);

   }
	printf ("\tV= %f, %f, %f\t\t X= %f, %f, %f\n",
		V[0][N/2][N/2][N/2], V[1][N/2][N/2][N/2], V[2][N/2][N/2][N/2], 
	        X[0][N/2][N/2][N/2], X[1][N/2][N/2][N/2], X[2][N/2][N/2][N/2]);
}
