// Version 1
// Sequential product without optimization

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void fill_random(int *matrix, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i * n + j] = rand() % 100;
}

void multiply(int *A, int *B, int *C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++)
                sum += A[i * n + k] * B[k * n + j];
            C[i * n + j] = sum;
        }
}

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    int n = atoi(argv[1]);
    if (n <= 0) return 1;

    int *A = malloc(n * n * sizeof(int));
    int *B = malloc(n * n * sizeof(int));
    int *C = malloc(n * n * sizeof(int));
    if (!A || !B || !C) return 1;

    srand(time(NULL));
    fill_random(A, n);
    fill_random(B, n);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    multiply(A, B, C, n);

    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed = (end.tv_sec - start.tv_sec)
                   + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("%.9f\n", elapsed);

    free(A); free(B); free(C);
    return 0;
}