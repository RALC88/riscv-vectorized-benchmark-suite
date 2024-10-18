//
// RISC-V VECTOR COSINE FUNCTION Version by Cristóbal Ramírez Lazo, "Barcelona 2019"
// This RISC-V Vector implementation is based on the original code presented by Julien Pommier

/* 
   Implementation of sin, cos, sincos, exp and log

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

inline _MMR_f64 __cos_1xf64(_MMR_f64 x , unsigned long int gvl) {

long int _ps_inv_sign_mask = ~0x8000000000000000;
double   _ps_cephes_FOPI   = 1.27323954473516; // 4 / M_PI
long int _pi32_1           = 1;
long int _pi32_inv1        = ~0x0000000000000001;
long int _pi32_2           = 2;
long int _pi32_4           = 4;
long int _Zero             = 0;
long int _largest          = 0xffffffffffffffff;

_MMR_i64   Zero            = _MM_SET_i64(0,gvl);

_MMR_f64   xmm2 =  _MM_SET_f64(0.0f,gvl);
_MMR_f64   xmm1;
_MMR_f64   xmm3;
_MMR_f64   y;

_MMR_i64   emm0;
_MMR_i64   emm2;

_MMR_MASK_i64 xMask;
  /* take the absolute value */
  x = _MM_CAST_f64_i64(_MM_AND_VX_i64(_MM_CAST_i64_f64(x), _ps_inv_sign_mask,gvl));

  /* scale by 4/Pi */
  y = _MM_MUL_VF_f64(x, _ps_cephes_FOPI,gvl);

  /* store the integer part of y in mm0 */
  emm2 = _MM_VFCVT_X_F_i64(y,gvl);

  /* j=(j+1) & (~1) (see the cephes sources) */
  emm2 = _MM_ADD_VX_i64(emm2,_pi32_1,gvl);
  emm2 = _MM_AND_VX_i64(emm2, _pi32_inv1,gvl);
  y = _MM_VFCVT_F_X_f64(emm2,gvl);

  emm2 = _MM_SUB_VX_i64(emm2, _pi32_2,gvl);

  /* get the swap sign flag */
  emm0 = _MM_XOR_VX_i64(emm2, _largest,gvl);
  emm0 = _MM_AND_VX_i64(emm0, _pi32_4,gvl);

  unsigned long int _61 = 61;
  emm0 = _MM_SLL_VX_i64(emm0, _61, gvl);

  /* get the polynom selection mask */
  emm2 = _MM_AND_VX_i64(emm2, _pi32_2 ,gvl);
  xMask= _MM_VMSEQ_VX_i64(emm2, _Zero,gvl);
  emm2 = _MM_MERGE_VX_i64(Zero, _largest, xMask,gvl);

  _MMR_f64 sign_bit =  _MM_CAST_f64_i64(emm0);
  _MMR_f64 poly_mask =  _MM_CAST_f64_i64(emm2);

  /* The magic pass: "Extended precision modular arithmetic"
     x = ((x - y * DP1) - y * DP2) - y * DP3; */

  double   _ps_minus_cephes_DP1    = -0.78515625;
  double   _ps_minus_cephes_DP2    = -2.4187564849853515625E-4;
  double   _ps_minus_cephes_DP3    = -3.77489497744594108E-8;

  xmm1 = _MM_MUL_VF_f64(y, _ps_minus_cephes_DP1,gvl);
  xmm2 = _MM_MUL_VF_f64(y, _ps_minus_cephes_DP2,gvl);
  xmm3 = _MM_MUL_VF_f64(y, _ps_minus_cephes_DP3,gvl);

  x = _MM_ADD_f64(x, xmm1,gvl);
  x = _MM_ADD_f64(x, xmm2,gvl);
  x = _MM_ADD_f64(x, xmm3,gvl);

  /* Evaluate the first polynom  (0 <= x <= Pi/4) */
  double   _ps_coscof_p0      = 2.443315711809948E-005;
  double   _ps_coscof_p1      = -1.388731625493765E-003;
  double   _ps_coscof_p2      = 4.166664568298827E-002;
  double   _ps_0p5            = 0.5;
  double   _One               = 1.0;

  _MMR_f64 z;
  _MMR_f64 tmp;

  z = _MM_MUL_f64(x,x,gvl);

  y = _MM_MUL_VF_f64(z, _ps_coscof_p0, gvl);
  y = _MM_ADD_VF_f64(y, _ps_coscof_p1,gvl);
  y = _MM_MUL_f64(y, z,gvl);
  y = _MM_ADD_VF_f64(y, _ps_coscof_p2,gvl);
  y = _MM_MUL_f64(y, z,gvl);
  y = _MM_MUL_f64(y, z,gvl);
  tmp = _MM_MUL_VF_f64(z, _ps_0p5,gvl);
  y = _MM_SUB_f64(y, tmp,gvl);
  y = _MM_ADD_VF_f64(y, _One ,gvl);

  /* Evaluate the second polynom  (Pi/4 <= x <= 0) */
  double   _ps_sincof_p0      = -1.9515295891E-4;
  double   _ps_sincof_p1      = 8.3321608736E-3;
  double   _ps_sincof_p2      = -1.6666654611E-1;

  _MMR_f64 y2;

  y2 = _MM_MUL_VF_f64(z ,_ps_sincof_p0, gvl);
  y2 = _MM_ADD_VF_f64(y2, _ps_sincof_p1 ,gvl);
  y2 = _MM_MUL_f64(y2, z ,gvl);
  y2 = _MM_ADD_VF_f64(y2, _ps_sincof_p2 ,gvl);
  y2 = _MM_MUL_f64(y2, z ,gvl);
  y2 = _MM_MUL_f64(y2, x ,gvl);
  y2 = _MM_ADD_f64(y2, x ,gvl);

  /* select the correct result from the two polynoms */
  xmm3 = poly_mask;
  y2 = _MM_CAST_f64_i64(_MM_AND_i64(_MM_CAST_i64_f64(xmm3), _MM_CAST_i64_f64(y2) , gvl));
  y = _MM_CAST_f64_i64(_MM_AND_i64(_MM_XOR_VX_i64(_MM_CAST_i64_f64(xmm3), _largest,gvl),_MM_CAST_i64_f64(y),gvl));
  y = _MM_ADD_f64(y, y2 ,gvl);
  /* update the sign */
  y = _MM_CAST_f64_i64(_MM_XOR_i64(_MM_CAST_i64_f64(y), _MM_CAST_i64_f64(sign_bit) , gvl));

  return y;
}

inline _MMR_f32 __cos_1xf32(_MMR_f32 x , unsigned long int gvl) {

int      _ps_inv_sign_mask = ~0x80000000;
float    _ps_cephes_FOPI   = 1.27323954473516; // 4 / M_PI
int      _pi32_1           = 0x00000001;
int      _pi32_inv1        = ~0x00000001;
int      _pi32_2           = 2;
int      _pi32_4           = 4;
int      _Zero             = 0;
int      _largest          = 0xffffffff;

_MMR_i32   Zero            = _MM_SET_i32(0,gvl);

_MMR_f32   xmm2 =  _MM_SET_f32(0.0f,gvl);
_MMR_f32   xmm1;
_MMR_f32   xmm3;
_MMR_f32   y;

_MMR_i32   emm0;
_MMR_i32   emm2;

_MMR_MASK_i32 xMask;
  /* take the absolute value */
  x = _MM_CAST_f32_i32(_MM_AND_VX_i32(_MM_CAST_i32_f32(x), _ps_inv_sign_mask,gvl));

  /* scale by 4/Pi */
  y = _MM_MUL_VF_f32(x, _ps_cephes_FOPI,gvl);

  /* store the integer part of y in mm0 */
  emm2 = _MM_VFCVT_X_F_i32(y,gvl);

  /* j=(j+1) & (~1) (see the cephes sources) */
  emm2 = _MM_ADD_VX_i32(emm2,_pi32_1,gvl);
  emm2 = _MM_AND_VX_i32(emm2, _pi32_inv1,gvl);
  y = _MM_VFCVT_F_X_f32(emm2,gvl);

  emm2 = _MM_SUB_VX_i32(emm2, _pi32_2,gvl);

  /* get the swap sign flag */
  emm0 = _MM_XOR_VX_i32(emm2, _largest, gvl);
  emm0 = _MM_AND_VX_i32(emm0, _pi32_4,gvl);

  unsigned int _29 = 29;
  emm0 = _MM_SLL_VX_i32(emm0, _29, gvl);

  /* get the polynom selection mask */
  emm2 = _MM_AND_VX_i32(emm2, _pi32_2 ,gvl);
  xMask= _MM_VMSEQ_VX_i32(emm2,_Zero,gvl);
  emm2 = _MM_MERGE_VX_i32(Zero,_largest, xMask,gvl);

  _MMR_f32 sign_bit =  _MM_CAST_f32_i32(emm0);
  _MMR_f32 poly_mask =  _MM_CAST_f32_i32(emm2);

  /* The magic pass: "Extended precision modular arithmetic"
     x = ((x - y * DP1) - y * DP2) - y * DP3; */

  float   _ps_minus_cephes_DP1    = -0.78515625;
  float   _ps_minus_cephes_DP2    = -2.4187564849853515625E-4;
  float   _ps_minus_cephes_DP3    = -3.77489497744594108E-8;

  xmm1 = _MM_MUL_VF_f32(y, _ps_minus_cephes_DP1,gvl);
  xmm2 = _MM_MUL_VF_f32(y, _ps_minus_cephes_DP2,gvl);
  xmm3 = _MM_MUL_VF_f32(y, _ps_minus_cephes_DP3,gvl);

  x = _MM_ADD_f32(x, xmm1,gvl);
  x = _MM_ADD_f32(x, xmm2,gvl);
  x = _MM_ADD_f32(x, xmm3,gvl);

  /* Evaluate the first polynom  (0 <= x <= Pi/4) */
  float   _ps_coscof_p0      = 2.443315711809948E-005;
  float   _ps_coscof_p1      = -1.388731625493765E-003;
  float   _ps_coscof_p2      = 4.166664568298827E-002;
  float   _ps_0p5            = 0.5f;
  float   _One               = 1.0f;

  _MMR_f32 z;
  _MMR_f32 tmp;

  z = _MM_MUL_f32(x,x,gvl);

  y = _MM_MUL_VF_f32(z, _ps_coscof_p0, gvl);
  y = _MM_ADD_VF_f32(y, _ps_coscof_p1,gvl);
  y = _MM_MUL_f32(y, z,gvl);
  y = _MM_ADD_VF_f32(y, _ps_coscof_p2,gvl);
  y = _MM_MUL_f32(y, z,gvl);
  y = _MM_MUL_f32(y, z,gvl);
  tmp = _MM_MUL_VF_f32(z, _ps_0p5,gvl);
  y = _MM_SUB_f32(y, tmp,gvl);
  y = _MM_ADD_VF_f32(y, _One, gvl);

  /* Evaluate the second polynom  (Pi/4 <= x <= 0) */
  float   _ps_sincof_p0      = -1.9515295891E-4;
  float   _ps_sincof_p1      = 8.3321608736E-3;
  float   _ps_sincof_p2      = -1.6666654611E-1;

  _MMR_f32  y2;

  y2 = _MM_MUL_VF_f32(z ,_ps_sincof_p0, gvl);
  y2 = _MM_ADD_VF_f32(y2, _ps_sincof_p1 ,gvl);
  y2 = _MM_MUL_f32(y2, z ,gvl);
  y2 = _MM_ADD_VF_f32(y2, _ps_sincof_p2 ,gvl);
  y2 = _MM_MUL_f32(y2, z ,gvl);
  y2 = _MM_MUL_f32(y2, x ,gvl);
  y2 = _MM_ADD_f32(y2, x ,gvl);

  /* select the correct result from the two polynoms */
  xmm3 = poly_mask;
  y2 = _MM_CAST_f32_i32(_MM_AND_i32(_MM_CAST_i32_f32(xmm3), _MM_CAST_i32_f32(y2) , gvl));
  y = _MM_CAST_f32_i32(_MM_AND_i32(_MM_XOR_VX_i32(_MM_CAST_i32_f32(xmm3), _largest, gvl),_MM_CAST_i32_f32(y),gvl));   
  y = _MM_ADD_f32(y, y2 ,gvl);
  /* update the sign */
  y = _MM_CAST_f32_i32(_MM_XOR_i32(_MM_CAST_i32_f32(y), _MM_CAST_i32_f32(sign_bit) , gvl));

  return y;
}
#endif