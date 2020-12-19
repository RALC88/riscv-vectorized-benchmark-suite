#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <time.h>
#include <sys/time.h>

// RISC-V VECTOR Version by Cristóbal Ramírez Lazo, "Barcelona 2019"
#ifdef USE_RISCV_VECTOR
#include "../../common/vector_defines.h"
#endif


int main (int argc, char **argv)
{

//#ifdef USE_RISCV_VECTOR
    struct timeval tv1_0, tv2_0;
    struct timezone tz_0;
    double elapsed0=0.0;
    gettimeofday(&tv1_0, &tz_0);
//#endif

float float_vector[1024];
int int_vector[1024];

for(int i=0; i<)


_MMR_f32 xFloat_vector;
_MMR_f32 xInt_vector;
_MMR_f32 xResult;

//unsigned long int gvl = __builtin_epi_vsetvl(end, __epi_e32, __epi_m1);
unsigned long int  gvl = vsetvl_e32m1(end) //PLCT

xFloat_vector = _MM_LOAD_f32(float_vector,gvl);
xInt_vector = _MM_LOAD_i32(int_vector,gvl);


xRatexTime = _MM_MUL_f32(xRiskFreeRate, xTime,gvl);

_MM_STORE_f64(OptionPrice, xOptionPrice,gvl);

    return 0;
}
