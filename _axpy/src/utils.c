#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <assert.h>
#include "utils.h"


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


void test_result(double *y, double *y_ref, long nrows)
{
   long row;
   int nerrs=0;
   /* Compute with the result to keep the compiler for marking the code as dead */
   for (row=0; row<nrows; row++) {
      double error = y[row] - y_ref[row];
      if (fabs(error) > 0.0000001)  {
         printf("y[%ld]=%.16f != y_ref[%ld]=%.16f  INCORRECT RESULT !!!! \n ", row, y[row], row, y_ref[row]);
         nerrs++;
         if (nerrs == 100) break;
      }
   }
   if (nerrs == 0) printf ("Result ok !!!\n");
}



