/*************************************************************************
*GET_TIME
*returns a long int representing the time
*************************************************************************/

#include <time.h>
#include <sys/time.h>

static long long get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000000) + tv.tv_usec;
}

// Returns the number of seconds elapsed between the two specified times
static float elapsed_time(long long start_time, long long end_time) {
        return (float) (end_time - start_time) / (1000 * 1000);
}

/*************************************************************************
*Instruction Count
*returns the number of instructions executed
*************************************************************************/
static unsigned long get_inst_count()
{
    unsigned long instr;
    asm volatile ("rdinstret %[instr]"
                : [instr]"=r"(instr));
    return instr;
}

static unsigned long get_cycles_count()
{
    unsigned long cycles;
    asm volatile ("rdcycle %[cycles]"
                : [cycles]"=r"(cycles));
    return cycles;
}

/*************************************************************************/