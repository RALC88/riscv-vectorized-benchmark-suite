extern void capture_ref_result(double *y, double* y_ref, int n);
extern void print_4D (int n, char*name, double (*y)[n][n][n]);
extern void capture_4D_ref (int n, double (*y)[n][n][n], double (*y_ref)[n][n][n]);
extern void test_4D_result(int n, double (*y)[n][n][n], double (*y_ref)[n][n][n]);
extern void clear_4D(int n, double (*X)[n][n][n]);
extern void init_X (int n, double (*X)[n][n][n]);
extern void boundary(int n, double (*X)[n][n][n], double (*V)[n][n][n]);
extern void force_contribution(int n, double (*X)[n][n][n], double (*F)[n][n][n],
                   int i, int j, int k, int neig_i, int neig_j, int neig_k);
extern void compute_force_new(int n, double (*X)[n][n][n], double (*F)[n][n][n]);
extern void  print_state(int n, double (*X)[n][n][n], double Xcenter[3], int nt);

extern void print_prv_header();
extern void print_prv_record();
