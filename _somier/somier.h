/*************************************************************************
* Somier - RISC-V Vectorized version
* Author: Jesus Labarta
* Barcelona Supercomputing Center
*************************************************************************/

extern double M;
extern double dt;
extern double spring_K;
extern double Xcenter[3];

extern void init_X (int n, double (*X)[n][n][n]);

extern void compute_forces(int n, double (*X)[n][n][n], double (*F)[n][n][n]);
extern void compute_forces_prevec(int n, double (*X)[n][n][n], double (*F)[n][n][n]);

extern void acceleration(int n, double (*A)[n][n][n], double (*F)[n][n][n], double M);
extern void velocities(int n, double (*V)[n][n][n], double (*A)[n][n][n], double dt);
extern void positions(int n, double (*X)[n][n][n], double (*V)[n][n][n], double dt);

extern void accel_intr(int n, double (*A)[n][n][n], double (*F)[n][n][n], double M);
extern void vel_intr(int n, double (*V)[n][n][n], double (*A)[n][n][n], double dt);
extern void pos_intr(int n, double (*X)[n][n][n], double (*V)[n][n][n], double dt);

extern void compute_stats(int n, double (*X)[n][n][n], double Xcenter[3]);
