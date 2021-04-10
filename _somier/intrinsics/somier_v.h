#define print_register(VR, GVL, NAME) do { \
  double tmp[256]; \
  __builtin_epi_vstore_1xf64(&tmp[0], VR, GVL); \
  printf("%s: ", NAME);   \
  for (int ilocal=0; ilocal<GVL; ilocal++) printf (" %2.10f", tmp[ilocal]); \
  printf("\n");   \
} while(0)


#define print_var(ADDRESS, LOWER, UPPER, TEXT, COUNT, I, J) do { \
            printf( TEXT); \
            printf(" %d: %d %d", COUNT,I,J); \
            for (int vl=LOWER; vl<UPPER;vl++) { printf (" %2.10f",ADDRESS); } \
            printf ("\n"); \
}while(0)

