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
#include "somier.h"

#include "timer.h"

#include "../common/riscv_util.h"

double Xcenter[3];

double dt=0.001;   // 0.1;
double spring_K=10.0;
double M=1.0;
int err;


int main(int argc, char *argv[])
{
   int nt;
   int ntsteps = 20;
   int N;

   if (argc !=3) {
       ntsteps = 5;
       N = 10;
       printf("Using default arguments:\nntsteps = %d\nN = %d\n", ntsteps, N);
   }
   else {
       ntsteps  = atoi(argv[1]);  //TODO: better parsing
       N       = atoi(argv[2]);  //TODO: better parsing
   }

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

   printf("Set initial speed\n");
//   V[2][N/2][N/2][N/2] = 0.1;
//   V[1][N/2][N/2][N/2] = 0.1;
//   V[0][N/2][N/2][N/2] = 0.1;  V[1][N/2][N/2][N/2] = 0.1;  V[2][N/2][N/2][N/2] = 0.1;
//   print_prv_header();
   V[0][N/2][N/2][N/2] = 0.1;  V[1][N/2][N/2][N/2] = 0.1;  V[2][N/2][N/2][N/2] = 0.1;

START_TIME();

// Start instruction and cycles count of the region of interest
//unsigned long cycles1, cycles2, instr2, instr1;
//instr1 = get_inst_count();
//cycles1 = get_cycles_count();
    
   for (nt=0; nt <ntsteps-1; nt++) {

      //if(nt%10 == 0) {
      if(nt%1 == 0) {
        print_state (N, X, Xcenter, nt);
      }

//      boundary(N, X, V);
//      printf ("\nCorrected Positions\n"); print_4D(N, "X", X); printf ("\n");
      Xcenter[0]=0, Xcenter[1]=0; Xcenter[2]=0;   //reset aggregate stats
      clear_4D(N, F);

#ifdef SEQ
      compute_forces(N, X, F);
      acceleration(N, A, F, M);
      velocities(N, V, A, dt);
      positions(N, X, V, dt);
#else
      compute_forces_prevec(N, X, F); // printf ("Computed forces\n"); print_4D(N, "F", F); printf ("\n");
      accel_intr  (N, A, F, M);       // printf ("Computed Accelerations\n"); print_4D(N, "A", A); printf ("\n");
      vel_intr  (N, V, A, dt);        // printf ("Computed Velocities\n"); print_4D(N, "V", V); printf ("\n");
//      vel_intr  (N, X, V, dt);        // printf ("Computed Positions\n"); print_4D(N, "X", X); printf ("\n");
      pos_intr  (N, X, V, dt);        // printf ("Computed Positions\n"); print_4D(N, "X", X); printf ("\n");
#endif

      compute_stats(N, X, Xcenter);
//      print_prv_record();
      
   }

// End instruction and cycles count of the region of interest
//instr2 = get_inst_count();
//cycles2 = get_cycles_count();
// Instruction and cycles count of the region of interest
//printf("-CSR   NUMBER OF EXEC CYCLES :%lu\n", cycles2 - cycles1);
//printf("-CSR   NUMBER OF INSTRUCTIONS EXECUTED :%lu\n", instr2 - instr1);

STOP_TIME();
#ifdef SEQ
printf("somier seq time   %f us \n\n", GET_TIME());
#else
printf("somier vec time   %f us \n\n", GET_TIME());
#endif

	printf ("\tV= %f, %f, %f\t\t X= %f, %f, %f\n",
		V[0][N/2][N/2][N/2], V[1][N/2][N/2][N/2], V[2][N/2][N/2][N/2],
	        X[0][N/2][N/2][N/2], X[1][N/2][N/2][N/2], X[2][N/2][N/2][N/2]);

    //asm("csrrw t0, 0xc00, t0");
  return 0;

}
