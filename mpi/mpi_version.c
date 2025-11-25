//Version 2
//Using MPI for paralelism

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void fill_random(int *matrix, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i * n + j] = rand() % 100;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int N;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        MPI_Finalize();
        return 1;
    }

    N = atoi(argv[1]);
    if (N <= 0) {
        MPI_Finalize();
        return 1;
    }

    int rows_per_proc = N / size;
    int extra = N % size;
    int local_rows = (rank < extra) ? rows_per_proc + 1 : rows_per_proc;

    int *A = NULL;
    int *B = malloc(N * N * sizeof(int));
    int *C_local = malloc(local_rows * N * sizeof(int));
    int *A_local = malloc(local_rows * N * sizeof(int));

    if (rank == 0) {
        A = malloc(N * N * sizeof(int));
        srand(time(NULL));
        fill_random(A, N);
        fill_random(B, N);
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    int *sendcounts = NULL;
    int *displs = NULL;

    if (rank == 0) {
        sendcounts = malloc(size * sizeof(int));
        displs = malloc(size * sizeof(int));

        int offset = 0;
        for (int i = 0; i < size; i++) {
            int r = (i < extra) ? rows_per_proc + 1 : rows_per_proc;
            sendcounts[i] = r * N;
            displs[i] = offset;
            offset += r * N;
        }
    }

    MPI_Scatterv(
        A, sendcounts, displs, MPI_INT,
        A_local, local_rows * N, MPI_INT,
        0, MPI_COMM_WORLD
    );

    struct timespec start, end;
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A_local[i * N + k] * B[k * N + j];
            }
            C_local[i * N + j] = sum;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        clock_gettime(CLOCK_MONOTONIC, &end);

        double elapsed =
            (end.tv_sec - start.tv_sec) +
            (end.tv_nsec - start.tv_nsec) / 1e9;

        printf("%.9f\n", elapsed);
    }

    if (rank == 0) {
        free(A);
        free(sendcounts);
        free(displs);
    }

    free(B);
    free(A_local);
    free(C_local);

    MPI_Finalize();
    return 0;
}