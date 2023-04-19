/*

    Modificando el code para realizar el incisico b, para crear dos
vectores de al menos 100,000 elementos generados de forma aleatoria

imprima únicamente los primeros y últimos 10
elementos de cada vector (y el resultado) para validar. Incluya captura de
pantalla.

*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Allocate_vectors(double** x_pp, double** y_pp, double** z_pp, int n);
void Generate_vector(double a[], int n);
void Print_first_and_last_elements(double b[], int n, char title[]);
void Vector_sum(double x[], double y[], double z[], int n);

/*---------------------------------------------------------------------*/
int main(void) {
   int n = 100000;
   double *x, *y, *z;

    //añadiendo el apartado para medicion de tiempos
   clock_t start_time, end_time;
   double elapsed_time;

   Allocate_vectors(&x, &y, &z, n);
   
   srand(time(NULL));
   Generate_vector(x, n);
   Generate_vector(y, n);
   
   //medicion de time
   start_time = clock();

   Vector_sum(x, y, z, n);

   end_time = clock();
   elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
   printf("Time elapsed: %f seconds\n", elapsed_time);

   Print_first_and_last_elements(x, n, "Vector x");
   Print_first_and_last_elements(y, n, "Vector y");
   Print_first_and_last_elements(z, n, "The sum is");

   free(x);
   free(y);
   free(z);

   return 0;
}  /* main */


void Allocate_vectors(
      double**  x_pp  /* out */, 
      double**  y_pp  /* out */, 
      double**  z_pp  /* out */, 
      int       n     /* in  */) {
   *x_pp = malloc(n*sizeof(double));
   *y_pp = malloc(n*sizeof(double));
   *z_pp = malloc(n*sizeof(double));
   if (*x_pp == NULL || *y_pp == NULL || *z_pp == NULL) {
      fprintf(stderr, "Can't allocate vectors\n");
      exit(-1);
   }
}  

void Generate_vector(double a[], int n) {
   for (int i = 0; i < n; i++)
      a[i] = rand() % 100 + 1; // Genera números aleatorios entre 1 y 100
}  

void Print_first_and_last_elements(
      double  b[]     /* in */, 
      int     n       /* in */, 
      char    title[] /* in */) {
   printf("%s\n", title);
   printf("First 10 elements: ");
   for (int i = 0; i < 10; i++)
      printf("%f ", b[i]);
   printf("\nLast 10 elements: ");
   for (int i = n - 10; i < n; i++)
      printf("%f ", b[i]);
   printf("\n");
}  /* Print_first_and_last_elements */

void Vector_sum(
      double  x[]  /* in  */, 
      double  y[]  /* in  */, 
      double  z[]  /* out */, 
      int     n    /* in  */) {
   for (int i = 0; i < n; i++)
      z[i] = x[i] + y[i];
}  
