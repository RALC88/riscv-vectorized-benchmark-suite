//
// RISC-V VECTOR LOG FUNCTION Version by Cristóbal Ramírez Lazo, "Barcelona 2019"
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

inline _MMR_f64 __log_1xf64(_MMR_f64 x , unsigned long int gvl) {

_MMR_i64   _x_i; 
_MMR_i64   imm0;
_MMR_f64  e;
_MMR_MASK_i64 invalid_mask = _MM_VFLE_f64(x,_MM_SET_f64(0.0f,gvl),gvl);


double   _ps256_cephes_SQRTHF     = 0.707106781186547524;

double   _ps256_cephes_log_p0     = 7.0376836292E-2;
double   _ps256_cephes_log_p1     = -1.1514610310E-1;
double   _ps256_cephes_log_p2     = 1.1676998740E-1;
double   _ps256_cephes_log_p3     = -1.2420140846E-1;
double   _ps256_cephes_log_p4     = 1.4249322787E-1;
double   _ps256_cephes_log_p5     = -1.6668057665E-1;
double   _ps256_cephes_log_p6     = 2.0000714765E-1;
double   _ps256_cephes_log_p7     = -2.4999993993E-1;
double   _ps256_cephes_log_p8     = 3.3333331174E-1;

long int _256_min_norm_pos        = 0x0010000000000000;
double _ps256_min_norm_pos        = *(double*)&_256_min_norm_pos;

long int _256_inv_mant_mask       = ~0x7ff0000000000000;
int      _1023                    = 1023;

double   _zero                    = 0.0f;
double   _one                     = 1.0f;
double   _ps256_0p5               = 0.5f;
long int _256_0p5                 = *(long int*)&_ps256_0p5;

double   _ps256_cephes_log_q1     = -2.12194440e-4;
double   _ps256_cephes_log_q2     = 0.693359375;


  x = _MM_MAX_VF_f64(x, _ps256_min_norm_pos, gvl);  /* cut off denormalized stuff */
  imm0 = _MM_CAST_i64_u64(_MM_SRL_i64(_MM_CAST_u64_i64(_MM_CAST_i64_f64(x)), _MM_CAST_u64_i64(_MM_SET_i64(52,gvl)), gvl));
  /* keep only the fractional part */
  _x_i = _MM_AND_VX_i64(_MM_CAST_i64_f64(x), _256_inv_mant_mask, gvl);
  _x_i = _MM_OR_VX_i64(_x_i, _256_0p5, gvl);
  x= _MM_CAST_f64_i64(_x_i);
  imm0 = _MM_SUB_VX_i64(imm0 , _1023 , gvl);
  e = _MM_VFCVT_F_X_f64(imm0,gvl);
  e = _MM_ADD_VF_f64(e, _one ,gvl);

_MMR_MASK_i64 mask = _MM_VFLT_VF_f64(x, _ps256_cephes_SQRTHF, gvl);
_MMR_f64 tmp  = _MM_MERGE_f64(_MM_SET_f64(_zero, gvl),x, mask,gvl);

  x = _MM_SUB_VF_f64(x, _one, gvl);
  e = _MM_SUB_f64(e, _MM_MERGE_f64(_MM_SET_f64(_zero,gvl),_MM_SET_f64(_one,gvl), mask,gvl),gvl);
  x = _MM_ADD_f64(x, tmp,gvl);

_MMR_f64 z = _MM_MUL_f64(x,x,gvl);
_MMR_f64 y;

  y = _MM_MADD_f64(_MM_SET_f64(_ps256_cephes_log_p0,gvl), x, _MM_SET_f64(_ps256_cephes_log_p1,gvl),gvl);
  y = _MM_MADD_f64(y,x,_MM_SET_f64(_ps256_cephes_log_p2, gvl), gvl);
  y = _MM_MADD_f64(y,x,_MM_SET_f64(_ps256_cephes_log_p3, gvl), gvl);
  y = _MM_MADD_f64(y,x,_MM_SET_f64(_ps256_cephes_log_p4, gvl), gvl);
  y = _MM_MADD_f64(y,x,_MM_SET_f64(_ps256_cephes_log_p5, gvl), gvl);
  y = _MM_MADD_f64(y,x,_MM_SET_f64(_ps256_cephes_log_p6, gvl), gvl);
  y = _MM_MADD_f64(y,x,_MM_SET_f64(_ps256_cephes_log_p7, gvl), gvl);
  y = _MM_MADD_f64(y,x,_MM_SET_f64(_ps256_cephes_log_p8, gvl), gvl);
  y = _MM_MUL_f64(y, z,gvl);
  y = _MM_MACC_f64(y,e,_MM_SET_f64(_ps256_cephes_log_q1, gvl),gvl);
  tmp = _MM_MUL_VF_f64(z, _ps256_0p5 ,gvl);
  y = _MM_SUB_f64(y, tmp,gvl);
  tmp = _MM_MUL_VF_f64(e, _ps256_cephes_log_q2, gvl);
  x = _MM_ADD_f64(x, y,gvl);
  x = _MM_ADD_f64(x, tmp,gvl);
  x = _MM_MERGE_f64(x,_MM_CAST_f64_i64(_MM_SET_i64(0xffffffffffffffff,gvl)), invalid_mask,gvl);

  return x;
}

inline _MMR_f32 __log_2xf32(_MMR_f32 x , unsigned long int gvl) {

_MMR_i32   _x_i; 
_MMR_i32   imm0;
_MMR_f32  e;

_MMR_MASK_i32 invalid_mask = _MM_VFLE_f32(x,_MM_SET_f32(0.0f,gvl),gvl);

float   _ps256_cephes_SQRTHF     = 0.707106781186547524;

float   _ps256_cephes_log_p0     = 7.0376836292E-2;
float   _ps256_cephes_log_p1     = -1.1514610310E-1;
float   _ps256_cephes_log_p2     = 1.1676998740E-1;
float   _ps256_cephes_log_p3     = -1.2420140846E-1;
float   _ps256_cephes_log_p4     = 1.4249322787E-1;
float   _ps256_cephes_log_p5     = -1.6668057665E-1;
float   _ps256_cephes_log_p6     = 2.0000714765E-1;
float   _ps256_cephes_log_p7     = -2.4999993993E-1;
float   _ps256_cephes_log_p8     = 3.3333331174E-1;

int _256_min_norm_pos            = 0x00800000;
float _ps256_min_norm_pos        = *(float*)&_256_min_norm_pos;

int _256_inv_mant_mask           = ~0x7f800000;
int     _127                     = 127;
float   _zero                    = 0.0f;
float   _one                     = 1.0f;
float   _ps256_0p5               = 0.5f;
int     _256_0p5                 = *(int*)&_ps256_0p5;

float   _ps256_cephes_log_q1     = -2.12194440e-4;
float   _ps256_cephes_log_q2     = 0.693359375;

  x = _MM_MAX_VF_f32(x, _ps256_min_norm_pos, gvl);  /* cut off denormalized stuff */
  imm0 = _MM_CAST_i32_u32(_MM_SRL_i32(_MM_CAST_u32_i32(_MM_CAST_i32_f32(x)), _MM_CAST_u32_i32(_MM_SET_i32(23,gvl)), gvl));
  /* keep only the fractional part */
  _x_i = _MM_AND_VX_i32(_MM_CAST_i32_f32(x), _256_inv_mant_mask, gvl);
  _x_i = _MM_OR_VX_i32(_x_i, _256_0p5, gvl);
  x= _MM_CAST_f32_i32(_x_i);
  imm0 = _MM_SUB_VX_i32(imm0 ,_127 , gvl);
  e = _MM_VFCVT_F_X_f32(imm0,gvl);
  e = _MM_ADD_VF_f32(e, _one ,gvl);

_MMR_MASK_i32 mask = _MM_VFLT_VF_f32(x, _ps256_cephes_SQRTHF , gvl);
_MMR_f32 tmp  = _MM_MERGE_f32(_MM_SET_f32(_zero,gvl),x, mask,gvl);

  x = _MM_SUB_VF_f32(x, _one, gvl);
  e = _MM_SUB_f32(e, _MM_MERGE_f32(_MM_SET_f32(_zero,gvl),_MM_SET_f32(_one,gvl), mask,gvl),gvl);
  x = _MM_ADD_f32(x, tmp,gvl);

_MMR_f32 z = _MM_MUL_f32(x,x,gvl);
_MMR_f32 y;

  y = _MM_MADD_f32(_MM_SET_f32(_ps256_cephes_log_p0,gvl),x,_MM_SET_f32(_ps256_cephes_log_p1, gvl), gvl);
  y = _MM_MADD_f32(y,x,_MM_SET_f32(_ps256_cephes_log_p2, gvl), gvl);
  y = _MM_MADD_f32(y,x,_MM_SET_f32(_ps256_cephes_log_p3, gvl), gvl);
  y = _MM_MADD_f32(y,x,_MM_SET_f32(_ps256_cephes_log_p4, gvl), gvl);
  y = _MM_MADD_f32(y,x,_MM_SET_f32(_ps256_cephes_log_p5, gvl), gvl);
  y = _MM_MADD_f32(y,x,_MM_SET_f32(_ps256_cephes_log_p6, gvl), gvl);
  y = _MM_MADD_f32(y,x,_MM_SET_f32(_ps256_cephes_log_p7, gvl), gvl);
  y = _MM_MADD_f32(y,x,_MM_SET_f32(_ps256_cephes_log_p8, gvl), gvl);
  y = _MM_MUL_f32(y, z,gvl);
  y = _MM_MACC_f32(y,e,_MM_SET_f32(_ps256_cephes_log_q1, gvl), gvl);
  tmp = _MM_MUL_VF_f32(z, _ps256_0p5, gvl);
  y = _MM_SUB_f32(y, tmp,gvl);
  tmp = _MM_MUL_VF_f32(e, _ps256_cephes_log_q2, gvl);
  x = _MM_ADD_f32(x, y,gvl);
  x = _MM_ADD_f32(x, tmp,gvl);
  x = _MM_MERGE_f32(x,_MM_CAST_f32_i32(_MM_SET_i32(0xffffffff,gvl)), invalid_mask,gvl);

  return x;
}
#endif