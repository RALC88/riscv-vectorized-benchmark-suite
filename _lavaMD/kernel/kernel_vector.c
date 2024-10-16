#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************
* RISC-V Vectorized Version
* Author: Cristóbal Ramírez Lazo
* email: cristobal.ramirez@bsc.es
* Barcelona Supercomputing Center (2020)
*************************************************************************/

//========================================================================================================================================================================================================200
//	DEFINE/INCLUDE
//========================================================================================================================================================================================================200

//======================================================================================================================================================150
//	LIBRARIES
//======================================================================================================================================================150

//#include <omp.h>									// (in path known to compiler)			needed by openmp
#include <stdlib.h>									// (in path known to compiler)			needed by malloc
#include <stdio.h>									// (in path known to compiler)			needed by printf
#include <math.h>									// (in path known to compiler)			needed by exp

//======================================================================================================================================================150
//	MAIN FUNCTION HEADER
//======================================================================================================================================================150

#include "./../main.h"								// (in the main program folder)	needed to recognized input variables

//======================================================================================================================================================150
//	UTILITIES
//======================================================================================================================================================150

//#include "./../util/timer/timer.h"					// (in library path specified to compiler)	needed by timer
#include "../../common/riscv_util.h"
//======================================================================================================================================================150
//	KERNEL_CPU FUNCTION HEADER
//======================================================================================================================================================150

#include "kernel_vector.h"								// (in the current directory)

//========================================================================================================================================================================================================200
//	PLASMAKERNEL_GPU
//========================================================================================================================================================================================================200

void  kernel_cpu(	par_str par, 
					dim_str dim,
					box_str* box,
					FOUR_VECTOR* rv,
					fp* qv,
					FOUR_VECTOR* fv)
{

	//======================================================================================================================================================150
	//	Variables
	//======================================================================================================================================================150

	// timer
	long long time0;

	time0 = get_time();

	// timer
	long long time1;
	long long time2;
	long long time3;
	long long time4;

	// parameters
	fp alpha;
	fp a2;

	// counters
	int i, j, k, l;

	// home box
	long first_i;
	FOUR_VECTOR* rA;
	FOUR_VECTOR* fA;

	// neighbor box
	int pointer;
	long first_j; 
	FOUR_VECTOR* rB;
	fp* qB;

	// common
	fp r2; 
	fp u2;
	fp fs;
	fp vij;
	fp fxij,fyij,fzij;
	THREE_VECTOR d;

	time1 = get_time();

	//======================================================================================================================================================150
	//	MCPU SETUP
	//======================================================================================================================================================150

	//omp_set_num_threads(dim.cores_arg);

	time2 = get_time();

	//======================================================================================================================================================150
	//	INPUTS
	//======================================================================================================================================================150

	alpha = par.alpha;
	a2 = 2.0*alpha*alpha;

	time3 = get_time();

	//======================================================================================================================================================150
	//	PROCESS INTERACTIONS
	//======================================================================================================================================================150

	//#pragma omp	parallel for \
	//			private(i, j, k) \
	//			private(first_i, rA, fA) \
	//			private(pointer, first_j, rB, qB) \
	//			private(r2, u2, fs, vij, fxij, fyij, fzij, d)
	for(l=0; l<dim.number_boxes; l=l+1){

		//------------------------------------------------------------------------------------------100
		//	home box - box parameters
		//------------------------------------------------------------------------------------------100

		first_i = box[l].offset;												// offset to common arrays

		//------------------------------------------------------------------------------------------100
		//	home box - distance, force, charge and type parameters from common arrays
		//------------------------------------------------------------------------------------------100

		rA = &rv[first_i];
		fA = &fv[first_i];

		//------------------------------------------------------------------------------------------100
		//	Do for the # of (home+neighbor) boxes
		//------------------------------------------------------------------------------------------100
		for (k=0; k<(1+box[l].nn); k++) 
		{

			//----------------------------------------50
			//	neighbor box - get pointer to the right box
			//----------------------------------------50

			if(k==0){
				pointer = l;													// set first box to be processed to home box
			}
			else{
				pointer = box[l].nei[k-1].number;							// remaining boxes are neighbor boxes
			}

			//----------------------------------------50
			//	neighbor box - box parameters
			//----------------------------------------50

			first_j = box[pointer].offset; 

			//----------------------------------------50
			//	neighbor box - distance, force, charge and type parameters
			//----------------------------------------50

			rB = &rv[first_j];
			qB = &qv[first_j];

			//----------------------------------------50
			//	Do for the # of particles in home box
			//----------------------------------------50

			_MMR_f32 xr2, xDOT, xu2, xvij;
			_MMR_f32 xrB_v, xrB_x, xrB_y, xrB_z;
			_MMR_f32 xd_x, xd_y, xd_z;
			_MMR_f32 xfxij, xfyij, xfzij;
			_MMR_f32 xfs, xqB;
			_MMR_f32 xfA_v, xfA_x, xfA_y, xfA_z;
			_MMR_f32 xfA_1_v, xfA_1_x, xfA_1_y, xfA_1_z;

			for (i=0; i<NUMBER_PAR_PER_BOX; i=i+1){

				unsigned long int gvl = _MMR_VSETVL_E32M1(NUMBER_PAR_PER_BOX);

				xfA_v = _MM_SET_f32(0.0f,gvl);
				xfA_x = _MM_SET_f32(0.0f,gvl);
				xfA_y = _MM_SET_f32(0.0f,gvl);
				xfA_z = _MM_SET_f32(0.0f,gvl);
				xfA_1_v  = _MM_SET_f32(fA[i].v, 1);
				xfA_1_x  = _MM_SET_f32(fA[i].x, 1);
				xfA_1_y  = _MM_SET_f32(fA[i].y, 1);
				xfA_1_z  = _MM_SET_f32(fA[i].z, 1);

				// do for the # of particles in current (home or neighbor) box
				for (j=0; j<NUMBER_PAR_PER_BOX; j+=gvl){

					gvl = _MMR_VSETVL_E32M1(NUMBER_PAR_PER_BOX-j);

					// coefficients
					xrB_v = _MM_LOAD_STRIDE_f32(&rB[j].v,16,gvl);
					xrB_x = _MM_LOAD_STRIDE_f32(&rB[j].x,16,gvl);
					xrB_y = _MM_LOAD_STRIDE_f32(&rB[j].y,16,gvl);
					xrB_z = _MM_LOAD_STRIDE_f32(&rB[j].z,16,gvl);
					//r2 = rA[i].v + rB[j].v - DOT(rA[i],rB[j]); 
					xr2    = _MM_ADD_f32_VF(xrB_v, rA[i].v, gvl);
					xDOT   = _MM_MUL_f32_VF(xrB_x, rA[i].x, gvl);
					xDOT   = _MM_MACC_f32_VF(xDOT,rA[i].y,xrB_y,gvl);
					xDOT   = _MM_MACC_f32_VF(xDOT,rA[i].z,xrB_z,gvl);
					xr2    = _MM_SUB_f32(xr2, xDOT,gvl);
					//u2 = a2*r2;
					xu2    = _MM_MUL_f32_VF(xr2, a2, gvl);
					//vij= exp(-u2);
					xvij   = _MM_EXP_f32(_MM_VFSGNJN_f32(xu2,xu2,gvl),gvl);
					//fs = 2.*vij;
					xfs    = _MM_MUL_f32(_MM_SET_f32(2.0f,gvl), xvij,gvl);
					//d.x = rA[i].x  - rB[j].x; 
					xd_x   = _MM_SUB_f32(_MM_SET_f32(rA[i].x,gvl), xrB_x,gvl);
					//d.y = rA[i].y  - rB[j].y; 
					xd_y   = _MM_SUB_f32(_MM_SET_f32(rA[i].y,gvl), xrB_y,gvl);
					//d.z = rA[i].z  - rB[j].z; 
					xd_z   = _MM_SUB_f32(_MM_SET_f32(rA[i].z,gvl), xrB_z,gvl);
					//fxij=fs*d.x;
					xfxij  = _MM_MUL_f32(xfs, xd_x,gvl);
					//fyij=fs*d.y;
					xfyij  = _MM_MUL_f32(xfs, xd_y,gvl);
					//fzij=fs*d.z;
					xfzij  = _MM_MUL_f32(xfs, xd_z,gvl);

					// forces
					//fA[i].v +=  qB[j]*vij;
					//fA[i].x +=  qB[j]*fxij;
					//fA[i].y +=  qB[j]*fyij;
					//fA[i].z +=  qB[j]*fzij;
					xqB = _MM_LOAD_f32(&qB[j],gvl);
					xfA_v   = _MM_MACC_f32(xfA_v,xqB,xvij,gvl);
					xfA_x   = _MM_MACC_f32(xfA_x,xqB,xfxij,gvl);
					xfA_y   = _MM_MACC_f32(xfA_y,xqB,xfyij,gvl);
					xfA_z   = _MM_MACC_f32(xfA_z,xqB,xfzij,gvl);
				} // for j

				gvl = _MMR_VSETVL_E32M1(NUMBER_PAR_PER_BOX);

				xfA_1_v  = _MM_REDSUM_f32(xfA_v,xfA_1_v,gvl);
				xfA_1_x  = _MM_REDSUM_f32(xfA_x,xfA_1_x,gvl);
				xfA_1_y  = _MM_REDSUM_f32(xfA_y,xfA_1_y,gvl);
				xfA_1_z  = _MM_REDSUM_f32(xfA_z,xfA_1_z,gvl);

				fA[i].v = _MM_VGETFIRST_f32(xfA_1_v);
				fA[i].x = _MM_VGETFIRST_f32(xfA_1_x);
				fA[i].y = _MM_VGETFIRST_f32(xfA_1_y);
				fA[i].z = _MM_VGETFIRST_f32(xfA_1_z);
			} // for i

		} // for k

	} // for l
	time4 = get_time();

	//======================================================================================================================================================150
	//	DISPLAY TIMING
	//======================================================================================================================================================150

	printf("Time spent in different stages of CPU/MCPU KERNEL:\n");

	printf("%15.12f s, %15.12f : CPU/MCPU: VARIABLES\n",				(float) (time1-time0) / 1000000, (float) (time1-time0) / (float) (time4-time0) * 100);
	printf("%15.12f s, %15.12f : MCPU: SET DEVICE\n",					(float) (time2-time1) / 1000000, (float) (time2-time1) / (float) (time4-time0) * 100);
	printf("%15.12f s, %15.12f : CPU/MCPU: INPUTS\n", 				(float) (time3-time2) / 1000000, (float) (time3-time2) / (float) (time4-time0) * 100);
	printf("%15.12f s, %15.12f : CPU/MCPU: KERNEL\n",					(float) (time4-time3) / 1000000, (float) (time4-time3) / (float) (time4-time0) * 100);

	printf("Total time:\n");
	printf("%.12f s\n", 												(float) (time4-time0) / 1000000);
} // main

#ifdef __cplusplus
}
#endif
