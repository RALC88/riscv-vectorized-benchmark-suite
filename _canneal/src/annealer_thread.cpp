// annealer_thread.cpp
//
// Created by Daniel Schwartz-Narbonne on 14/04/07.
//
// Copyright 2007-2008 Princeton University
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

#ifdef ENABLE_THREADS
#include <pthread.h>
#endif

#include <cassert>
#include "annealer_thread.h"
#include "location_t.h"
#include "annealer_types.h"
#include "netlist_elem.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include "rng.h"
#include <stdio.h>
using std::cout;
using std::endl;

#include <stdlib.h>

// RISC-V VECTOR Version by Cristóbal Ramírez Lazo, "Barcelona 2019"
#ifdef USE_RISCV_VECTOR
#include <riscv_vector.h>
#include "../../common/vector_defines.h"
#endif

//*****************************************************************************************
//
//*****************************************************************************************
void annealer_thread::Run()
{
    int accepted_good_moves=0;
    int accepted_bad_moves=-1;
    double T = _start_temp;
    Rng rng; //store of randomness

    long a_id;
    long b_id;
    netlist_elem* a = _netlist->get_random_element(&a_id, NO_MATCHING_ELEMENT, &rng);
    netlist_elem* b = _netlist->get_random_element(&b_id, NO_MATCHING_ELEMENT, &rng);

    int temp_steps_completed=0;

    #ifdef USE_RISCV_VECTOR
    unsigned long int gvl   = __riscv_vsetvlmax_e32m1();
    mask = (int*)malloc(gvl*sizeof(int));
    for(int i=0 ; i<=gvl ; i=i+1) { mask[i]=0x55555555; }
    #endif // !USE_RISCV_VECTOR

    while(keep_going(temp_steps_completed, accepted_good_moves, accepted_bad_moves)){
        T = T / 1.5;
        accepted_good_moves = 0;
        accepted_bad_moves = 0;

        for (int i = 0; i < _moves_per_thread_temp; i++){
            a = b;
            a_id = b_id;
            b = _netlist->get_random_element(&b_id, a_id, &rng);
    #ifdef USE_RISCV_VECTOR
            routing_cost_t delta_cost = calculate_delta_routing_cost_vector(a,b/*,xMask*/);
    #else // !USE_RISCV_VECTOR
            routing_cost_t delta_cost = calculate_delta_routing_cost(a,b);
    #endif // !USE_RISCV_VECTOR

            move_decision_t is_good_move = accept_move(delta_cost, T, &rng);

            //make the move, and update stats:
            if (is_good_move == move_decision_accepted_bad){
                accepted_bad_moves++;
                _netlist->swap_locations(a,b);
            } else if (is_good_move == move_decision_accepted_good){
                accepted_good_moves++;
                _netlist->swap_locations(a,b);
            } else if (is_good_move == move_decision_rejected){
                //no need to do anything for a rejected move
            }
        }

        temp_steps_completed++;
#ifdef ENABLE_THREADS
        pthread_barrier_wait(&_barrier);
#endif
    }
#ifdef USE_RISCV_VECTOR
    free(mask);
#endif // !USE_RISCV_VECTOR
}

//*****************************************************************************************
//
//*****************************************************************************************
annealer_thread::move_decision_t annealer_thread::accept_move(routing_cost_t delta_cost, double T, Rng* rng)
{
    //always accept moves that lower the cost function
    if (delta_cost < 0){
        return move_decision_accepted_good;
    }else {
        double random_value = rng->drand();
        double boltzman = exp(- delta_cost/T);
        if (boltzman > random_value){
            return move_decision_accepted_bad;
        } else {
            return move_decision_rejected;
        }
    }
}

//*****************************************************************************************
//  If get turns out to be expensive, I can reduce the # by passing it into the swap cost fcn
//*****************************************************************************************
#ifdef USE_RISCV_VECTOR
routing_cost_t annealer_thread::calculate_delta_routing_cost_vector(netlist_elem* a, netlist_elem* b/*, __epi_2xi1  xMask2*/)
{
    routing_cost_t delta_cost=0.0;

    int a_fan_size = a->fanin.size() + a->fanout.size();
    int b_fan_size = b->fanin.size() + b->fanout.size();
    location_t* a_loc = a->present_loc.Get();
    location_t* b_loc = b->present_loc.Get();


    if((a_fan_size > 0) | (b_fan_size > 0))
    {
        int max_vl = (a_fan_size > b_fan_size) ? a_fan_size*2 : b_fan_size*2;
        unsigned long int gvl   = __riscv_vsetvl_e32m1(max_vl);
        // Get the MVL allowed by the hardware
        //unsigned long int gvl   = __builtin_epi_vsetvlmax(__epi_e32, __epi_m1);
        //Create a mask with size of MVL
        //int* mask;
        //mask = (int*)malloc(gvl*sizeof(int));
        //for(int i=0 ; i<=gvl ; i=i+2) { mask[i]=1;  mask[i+1]=0; }
        _MMR_MASK_i32  xMask = _MM_CAST_i1_i32(_MM_LOAD_i32((int *)&mask[0],gvl));
        _MMR_i32 xAFanin_loc     = _MM_MERGE_i32(_MM_SET_i32(a_loc->y,gvl),_MM_SET_i32(a_loc->x,gvl),xMask,gvl);
        _MMR_i32 xBFanin_loc     = _MM_MERGE_i32(_MM_SET_i32(b_loc->y,gvl),_MM_SET_i32(b_loc->x,gvl),xMask,gvl);

        if(a_fan_size > 0) {
            delta_cost = a->swap_cost_vector(xAFanin_loc,xBFanin_loc,a_fan_size);
        }
        if(b_fan_size > 0) {
            delta_cost = delta_cost + b->swap_cost_vector(xBFanin_loc,xAFanin_loc,b_fan_size);
        }
    }

    return delta_cost;
}

#else // !USE_RISCV_VECTOR
//*****************************************************************************************
//  If get turns out to be expensive, I can reduce the # by passing it into the swap cost fcn
//*****************************************************************************************
routing_cost_t annealer_thread::calculate_delta_routing_cost(netlist_elem* a, netlist_elem* b)
{
    routing_cost_t delta_cost=0.0;

    location_t* a_loc = a->present_loc.Get();
    location_t* b_loc = b->present_loc.Get();

    delta_cost = a->swap_cost(a_loc, b_loc);
    delta_cost = delta_cost + b->swap_cost(b_loc, a_loc);

    return delta_cost;
}
#endif // !USE_RISCV_VECTOR
//*****************************************************************************************
//  Check whether design has converged or maximum number of steps has reached
//*****************************************************************************************
bool annealer_thread::keep_going(int temp_steps_completed, int accepted_good_moves, int accepted_bad_moves)
{
    bool rv;

    if(_number_temp_steps == -1) {
        //run until design converges
        rv = _keep_going_global_flag && (accepted_good_moves > accepted_bad_moves);
        if(!rv) _keep_going_global_flag = false; // signal we have converged
    } else {
        //run a fixed amount of steps
        rv = temp_steps_completed < _number_temp_steps;
    }

    return rv;
}

