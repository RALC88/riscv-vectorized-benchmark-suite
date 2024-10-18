// 
// RISC-V VECTOR EXP FUNCTION Version by Cristóbal Ramírez Lazo, "Barcelona 2019"
// This RISC-V Vector implementation is based on the original code presented by Julien Pommier

/* 
   AVX implementation of sin, cos, sincos, exp and log

   Based on "sse_mathfun.h", by Julien Pommier
   http://gruntthepeon.free.fr/ssemath/

   Copyright (C) 2012 Giovanni Garberoglio
   Interdisciplinary Laboratory for Computational Science (LISC)
   Fondazione Bruno Kessler and University of Trento
   via Sommarive, 18
   I-38123 Trento (Italy)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  (this is the zlib license)
*/
#ifdef USE_RISCV_VECTOR

#include <riscv_vector.h>

inline _MMR_f64 __exp_1xf64(_MMR_f64 x ,unsigned long int gvl) {

_MMR_f64   tmp;
_MMR_f64   tmp2;
_MMR_f64   tmp4;
_MMR_f64   fx;
_MMR_f64   z;
_MMR_f64   y;
_MMR_MASK_i64  mask;
_MMR_i64  imm0;
_MMR_i64  tmp3;

double exp_hi = 88.3762626647949;
double exp_low = -88.3762626647949;

double   cephes_LOG2EF = 1.44269504088896341;
double   cephes_exp_C1 = 0.693359375;
double   cephes_exp_C2 = -2.12194440e-4;

double   cephes_exp_p0 = 1.9875691500E-4;
double   cephes_exp_p1 = 1.3981999507E-3;
double   cephes_exp_p2 = 8.3334519073E-3;
double   cephes_exp_p3 = 4.1665795894E-2;
double   cephes_exp_p4 = 1.6666665459E-1;
double   cephes_exp_p5 = 5.0000001201E-1;

double One = 1.0;


        x     = _MM_MIN_VF_f64(x, exp_hi, gvl);
        x     = _MM_MAX_VF_f64(x, exp_low, gvl);

        fx    = _MM_SET_f64(0.5,gvl);
        fx    = _MM_MACC_VF_f64(fx, cephes_LOG2EF, x, gvl);

        tmp3  = _MM_VFCVT_X_F_i64(fx,gvl);
        tmp   = _MM_VFCVT_F_X_f64(tmp3,gvl);

        mask  = _MM_VFLT_f64(fx,tmp,gvl); 
        tmp2  = _MM_MERGE_VF_f64(_MM_SET_f64(0.0,gvl), One, mask,gvl);
        fx    = _MM_SUB_f64(tmp,tmp2,gvl);
        tmp   = _MM_MUL_VF_f64(fx, cephes_exp_C1, gvl);
        z     = _MM_MUL_VF_f64(fx, cephes_exp_C2,gvl);
        x     = _MM_SUB_f64(x,tmp,gvl);
        x     = _MM_SUB_f64(x,z,gvl);

        z     = _MM_MUL_f64(x,x,gvl);
        y     = _MM_SET_f64(cephes_exp_p0,gvl);
        y     = _MM_MADD_f64(y,x,_MM_SET_f64(cephes_exp_p1,gvl),gvl);
        y     = _MM_MADD_f64(y,x,_MM_SET_f64(cephes_exp_p2,gvl),gvl);
        y     = _MM_MADD_f64(y,x,_MM_SET_f64(cephes_exp_p3,gvl),gvl);
        y     = _MM_MADD_f64(y,x,_MM_SET_f64(cephes_exp_p4,gvl),gvl);
        y     = _MM_MADD_f64(y,x,_MM_SET_f64(cephes_exp_p5,gvl),gvl);
        y     = _MM_MADD_f64(y,z,x,gvl);
        y     = _MM_ADD_VF_f64(y, One, gvl);

        long int _1023 = 1023;
        unsigned long int _52 = 52;
        imm0  = _MM_VFCVT_X_F_i64(fx,gvl);
        imm0  = _MM_ADD_VX_i64(imm0, _1023, gvl); 
        imm0  = _MM_SLL_VX_i64(imm0, _52, gvl);

        tmp4 = _MM_CAST_f64_i64(imm0);
        y     = _MM_MUL_f64(y, tmp4,gvl);
        return y;
}

inline _MMR_f32 __exp_2xf32(_MMR_f32 x ,unsigned long int gvl) {

_MMR_f32   tmp;
_MMR_f32   tmp2;
_MMR_f32   tmp4;
_MMR_f32   fx;
_MMR_f32   z;
_MMR_f32   y;
_MMR_MASK_i32  mask;
_MMR_i32  imm0;
_MMR_i32  tmp3;

float exp_hi = 88.3762626647949;
float exp_low = -88.3762626647949;

float   cephes_LOG2EF = 1.44269504088896341;
float   cephes_exp_C1 = 0.693359375;
float   cephes_exp_C2 = -2.12194440e-4;

float   cephes_exp_p0 = 1.9875691500E-4;
float   cephes_exp_p1 = 1.3981999507E-3;
float   cephes_exp_p2 = 8.3334519073E-3;
float   cephes_exp_p3 = 4.1665795894E-2;
float   cephes_exp_p4 = 1.6666665459E-1;
float   cephes_exp_p5 = 5.0000001201E-1;

float One = 1.0;

        x     = _MM_MIN_VF_f32(x, exp_hi, gvl);
        x     = _MM_MAX_VF_f32(x, exp_low, gvl);

        fx    = _MM_SET_f32(0.5,gvl);
        fx    = _MM_MACC_VF_f32(fx, cephes_LOG2EF, x, gvl);

        tmp3  = _MM_VFCVT_X_F_i32(fx,gvl);
        tmp   = _MM_VFCVT_F_X_f32(tmp3,gvl);

        mask  = _MM_VFLT_f32(fx,tmp,gvl); 
        tmp2  = _MM_MERGE_VF_f32( _MM_SET_f32(0.0,gvl), One, mask,gvl);
        fx    = _MM_SUB_f32(tmp,tmp2,gvl);
        tmp   = _MM_MUL_VF_f32(fx, cephes_exp_C1, gvl);
        z     = _MM_MUL_VF_f32(fx, cephes_exp_C2, gvl);
        x     = _MM_SUB_f32(x,tmp,gvl);
        x     = _MM_SUB_f32(x,z,gvl);

        z     = _MM_MUL_f32(x,x,gvl);
        y     = _MM_SET_f32(cephes_exp_p0,gvl);
        y     = _MM_MADD_f32(y,x,_MM_SET_f32(cephes_exp_p1, gvl),gvl);
        y     = _MM_MADD_f32(y,x,_MM_SET_f32(cephes_exp_p2, gvl),gvl);
        y     = _MM_MADD_f32(y,x,_MM_SET_f32(cephes_exp_p3, gvl),gvl);
        y     = _MM_MADD_f32(y,x,_MM_SET_f32(cephes_exp_p4, gvl),gvl);
        y     = _MM_MADD_f32(y,x,_MM_SET_f32(cephes_exp_p5, gvl),gvl);
        y     = _MM_MADD_f32(y,z,x,gvl);
        y     = _MM_ADD_VF_f32(y, One, gvl);

        int _0x7f = 0x7f;
        unsigned int _23 = 23;
        imm0  = _MM_VFCVT_X_F_i32(fx,gvl);
        imm0  = _MM_ADD_VX_i32(imm0, _0x7f, gvl); 
        imm0  = _MM_SLL_VX_i32(imm0, _23, gvl);

        tmp4 = _MM_CAST_f32_i32(imm0);
        y     = _MM_MUL_f32(y, tmp4,gvl);
        return y;
}
#endif