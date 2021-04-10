#ifndef OUR_TIMER_H
#define OUR_TIMER_H


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


static double start_time;
static double end_time;

#define START_TIME() start_time = getusec_();
#define STOP_TIME() end_time = getusec_()
#define GET_TIME() (end_time - start_time) 


static char const *GET_CPUS(void) {
	if (getenv("OMP_NUM_THREADS") != NULL) {
		return getenv("OMP_NUM_THREADS");
	} else {
		return "Sequential";
	}
}


static double getusec_(void) {
	struct timeval time;
	gettimeofday(&time, 0);
	return ((double)time.tv_sec * (double)1e6 + (double)time.tv_usec);
}


#endif // OUR_TIMER_H
