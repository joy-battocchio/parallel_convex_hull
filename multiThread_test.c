#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#ifdef _OPENMP
#include <omp.h>
#endif


int main(int argc, char *argv[]) {
    int iter = atoi(argv[1]);
    int n_threads = 8;
    #ifdef _OPENMP
        float start = omp_get_wtime();
    #pragma omp parallel num_threads(n_threads)
        int sum = 0;
        {
            // int thread_num = omp_get_thread_num();
            // int real_thread_count = omp_get_num_threads();
            // int cpu_num = sched_getcpu();
            // printf("[DIVIDE] Thread n:%d of (%d) of %d CPU, is starting to work [in divide]\n", thread_num, real_thread_count, cpu_num);

            // printf("[lh size]: %d \n", lh_size);
            // printf("[rh size]: %d \n", rh_size);

    #pragma omp for nowait
            for (int i = 0; i < iter; i++)
            {
                sum=i;
            }

            //# pragma omp barrier
        float end = omp_get_wtime();
        printf("parallel section time: %f\n", end-start);
        }
    #else // no threads

        for (int i = 0; i < iter; i++)
        {
            //sum++;
        }
    #endif
    return 0;
}