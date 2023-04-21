#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void Check_for_error(int local_ok, char fname[], char message[],
                     MPI_Comm comm);
void Allocate_vectors(double **local_x_pp, double **local_y_pp,
                      double **local_z_pp, double **local_scalar_mult1_pp, double **local_scalar_mult2_pp, int local_n, MPI_Comm comm);
void Generate_vector(double local_a[], int local_n, int n,
                     char vec_name[], int my_rank, MPI_Comm comm);
void Print_first_and_last_elements(double local_b[], int local_n, int n,
                                   char title[], int my_rank, MPI_Comm comm);
void Parallel_vector_sum(double local_x[], double local_y[],
                         double local_z[], int local_n);
void Parallel_dot_product(double local_x[], double local_y[],
                          double* local_dot_p, int local_n, MPI_Comm comm);

void Parallel_scalar_mult(double local_x[],
                          double local_scalar_mult[], int scalar, int local_n);

int main(void)
{
    int n, local_n;
    int comm_sz, my_rank;
    double *local_dot_p;

    double *local_x, *local_y, *local_z, *local_scalar_mult1, *local_scalar_mult2;
    int scalar = rand() % 100 + 1;
    local_dot_p = malloc(sizeof(int));
    MPI_Comm comm;

    MPI_Init(NULL, NULL);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &comm_sz);
    MPI_Comm_rank(comm, &my_rank);

    n = 5; // Valor actualizado de n
    local_n = n / comm_sz;

    Allocate_vectors(&local_x, &local_y, &local_z, &local_scalar_mult1, &local_scalar_mult2, local_n, comm);

    srand(time(NULL) + my_rank);
    Generate_vector(local_x, local_n, n, "x", my_rank, comm);
    Generate_vector(local_y, local_n, n, "y", my_rank, comm);

    // Añadimos esto para medir el tiempo
    double start_time, end_time, elapsed_time;
    if (my_rank == 0)
    {
        start_time = MPI_Wtime();
    }

    Parallel_vector_sum(local_x, local_y, local_z, local_n);
    Parallel_dot_product(local_x, local_y, local_dot_p, local_n, comm);


    Parallel_scalar_mult(local_x, local_scalar_mult1, scalar, local_n);
    Parallel_scalar_mult(local_y, local_scalar_mult2, scalar, local_n);

    // Añadimos esto para medir el tiempo
    if (my_rank == 0)
    {
        end_time = MPI_Wtime();
        elapsed_time = end_time - start_time;
        printf("Time elapsed: %f seconds\n", elapsed_time);
    }

    Print_first_and_last_elements(local_x, local_n, n, "Vector x", my_rank, comm);
    Print_first_and_last_elements(local_y, local_n, n, "Vector y", my_rank, comm);
    Print_first_and_last_elements(local_z, local_n, n, "The sum is", my_rank, comm);

    if (my_rank == 0)
    {
        printf("The scalar is %d \n", scalar);
    }
    Print_first_and_last_elements(local_scalar_mult1, local_n, n, "The scalar multiplication of x is", my_rank, comm);
    Print_first_and_last_elements(local_scalar_mult2, local_n, n, "The scalar multiplication of y is", my_rank, comm);

    if (my_rank == 0)
    {
        printf("The dot product is %f \n", *local_dot_p);
    }


    free(local_x);
    free(local_y);
    free(local_z);
    free(local_dot_p);
    free(local_scalar_mult1);
    free(local_scalar_mult2);

    MPI_Finalize();

    return 0;
}

void Check_for_error(int local_ok, char fname[], char message[],
                     MPI_Comm comm)
{
    int ok;

    MPI_Allreduce(&local_ok, &ok, 1, MPI_INT, MPI_MIN, comm);
    if (ok == 0)
    {
        int my_rank;
        MPI_Comm_rank(comm, &my_rank);
        if (my_rank == 0)
        {
            fprintf(stderr, "Proc %d > In %s, %s\n", my_rank, fname,
                    message);
            fflush(stderr);
        }
        MPI_Finalize();
        exit(-1);
    }
}

void Allocate_vectors(double **local_x_pp, double **local_y_pp,
                      double **local_z_pp, double **local_scalar_mult1_pp, double **local_scalar_mult2_pp, int local_n, MPI_Comm comm)
{
    int local_ok = 1;
    char *fname = "Allocate_vectors";

    *local_x_pp = malloc(local_n * sizeof(double));
    *local_y_pp = malloc(local_n * sizeof(double));
    *local_z_pp = malloc(local_n * sizeof(double));
    *local_scalar_mult1_pp = malloc(local_n * sizeof(double));
    *local_scalar_mult2_pp = malloc(local_n * sizeof(double));

    if (*local_x_pp == NULL || *local_y_pp == NULL ||
        *local_z_pp == NULL || *local_scalar_mult1_pp == NULL || *local_scalar_mult2_pp == NULL)
        local_ok = 0;
    Check_for_error(local_ok, fname, "Can't allocate local vector(s)",
                    comm);
}

void Generate_vector(double local_a[], int local_n, int n,
                     char vec_name[], int my_rank, MPI_Comm comm)
{
    double *a = NULL;
    int i;

    if (my_rank == 0)
    {
        a = malloc(n * sizeof(double));
        for (i = 0; i < n; i++)
            a[i] = rand() % 100 + 1;
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
        free(a);
    }
    else
    {
        MPI_Scatter(a, local_n, MPI_DOUBLE, local_a, local_n, MPI_DOUBLE, 0, comm);
    }
}

void Print_first_and_last_elements(
    double local_b[] /* in */,
    int local_n /* in */,
    int n /* in */,
    char title[] /* in */,
    int my_rank /* in */,
    MPI_Comm comm /* in */)
{

    double *full_b = NULL;
    int i;

    if (my_rank == 0)
    {
        full_b = malloc(n * sizeof(double));
    }

    MPI_Gather(local_b, local_n, MPI_DOUBLE, full_b, local_n, MPI_DOUBLE, 0, comm);

    if (my_rank == 0)
    {
        printf("%s\n", title);
        printf("Elements: ");
        for (i = 0; i < n; i++)
            printf("%f ", full_b[i]);
        printf("\n");

        free(full_b);
    }

}

void Parallel_vector_sum(double local_x[], double local_y[],
                         double local_z[], int local_n)
{
    int i;

    for (i = 0; i < local_n; i++)
    {
        local_z[i] = local_x[i] + local_y[i];
    }
}

void Parallel_dot_product(double local_x[], double local_y[],
                          double* local_dot_p, int local_n, MPI_Comm comm)
{
    int i;
    double local_sum = 0;

    for (i = 0; i < local_n; i++)
    {
        local_sum += local_x[i] * local_y[i];
    }

    MPI_Reduce(&local_sum, local_dot_p, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
}



void Parallel_scalar_mult(double local_x[],
                          double local_scalar_mult[], int scalar, int local_n)
{
    int i;

    for (i = 0; i < local_n; i++)
    {
        local_scalar_mult[i] = scalar * local_x[i];
    }
}