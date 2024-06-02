#include <stdio.h>
#include <math.h>
#include <pthread.h>

#define THREADS 4
#define N 3000

int primes[N];
int pflag[N];
int total = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int is_prime(int v)
{
    int i;
    int bound = floor(sqrt((double)v)) + 1;
    for (i = 2; i < bound; i++) {
        if (!pflag[i])
            continue;
        if (v % i == 0) {
            pflag[v] = 0;
            return 0;
        }
    }
    return (v > 1);
}

void *work(void *arg)
{
    int thread_id = *(int *)arg;
    int start = (N / THREADS) * thread_id;
    int end = start + (N / THREADS);
    int i;

    for (i = start; i < end; i++) {
        if (is_prime(i)) {
            pthread_mutex_lock(&mutex);
            primes[total] = i;
            total++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main(int argn, char **argv)
{
    int i;
    pthread_t tids[THREADS];
    int thread_ids[THREADS];

    for (i = 0; i < N; i++) {
        pflag[i] = 1;
    }

    for (i = 0; i < THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&tids[i], NULL, work, (void *)&thread_ids[i]);
    }

    for (i = 0; i < THREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("Number of prime numbers between 2 and %d: %d\n", N, total);
    for (int i = 0; i < total; i++) {
        printf("%d\n", primes[i]);
    }

    return 0;
}