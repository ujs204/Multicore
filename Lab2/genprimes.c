#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
//gcc -O3 -std=c99 -Wall -fopenmp -o genprime genprimes.c -lm

void runSequential(int N){
    bool *prime = (bool*)malloc((N+1) * sizeof(bool));

    for(int iter = 2; iter <= N; iter++){
        prime[iter] = true;
    }
    for(int iter = 2; iter <= sqrt(N); iter++){
        if(prime[iter]){
            for(int j = pow(iter,2); j <= N; j = j + iter){
                prime[j] = false;
            }
        }
    }

    char filename[20];
    snprintf(filename, sizeof(filename), "%d.txt", N);
    FILE *output = fopen(filename, "w");

    int r = 1;
    for(int iter = 2; iter <= N; iter++){
        if(prime[iter]){
            fprintf(output, "%d %d\n", r, iter);
            r++;
        }
    }

    fclose(output);
    free(prime);
    //return 0;

}

void runParallel(int N, int t){
    double tstart = 0.0;
    double ttaken;
    bool *prime = (bool*)malloc((N+1) * sizeof(bool));

    tstart = omp_get_wtime();
    int limit = (int) sqrt(N);
    memset(prime, 1, (N + 1) * sizeof(bool));
    prime[0] = false;
    prime[1] = false;
    
    for(int iter = 2; pow(iter,2) <= limit; iter++){
        if(prime[iter]){
            for(int j = pow(iter,2); j <= limit; j = j + iter){
                prime[j] = false;
            }
        }
    }

    omp_set_num_threads(t);
    #pragma omp parallel for schedule(dynamic) num_threads(t)
    for(int iter = 2; iter <= limit; iter++){
        if(prime[iter]){
            for(int j = pow(iter,2); j <= N; j = j + iter){
                prime[j] = false;
            }
        }
    }

    ttaken = omp_get_wtime() - tstart;
    printf("Time for main section of program: %f\n", ttaken);

    char filename[20];
    snprintf(filename, sizeof(filename), "%d.txt", N);
    FILE *output = fopen(filename, "w");

    int r = 1;
    for(int iter = 2; iter <= N; iter++){
        if(prime[iter]){
            fprintf(output, "%d %d\n", r, iter);
            r++;
        }
    }

    fclose(output);
    free(prime);
    //return 0;

}


int main(int argc, char *argv[]){
    int N = atoi(argv[1]);
    int t = atoi(argv[2]);
    //runSequential(N);
    runParallel(N,t);
    return 0;

}