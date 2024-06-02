#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

#define THREADS 4
#define N 3000

int primes[N];
int pflag[N];
int total = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;

int is_prime(int v) {
    if (v < 2) return 0;
    int bound = floor(sqrt((double)v)) + 1;
    for (int i = 2; i < bound; i++) {
        if (!pflag[i])
            continue;
        if (v % i == 0) {
            pflag[v] = 0;
            return 0;
        }
    }
    return 1;
}

void *work(void *arg) {
    int thread_id = *(int *)arg;
    int start = (N / THREADS) * thread_id;
    int end = start + (N / THREADS);

    pthread_mutex_lock(&mutex);
    while (!ready) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    for (int i = start; i < end; i++) {
        if (is_prime(i)) {
            pthread_mutex_lock(&mutex);
            primes[total] = i;
            total++;
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main() {
    pthread_t tids[THREADS];
    int thread_ids[THREADS];

    for (int i = 0; i < N; i++) {
        pflag[i] = 1;
    }

    for (int i = 0; i < THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&tids[i], NULL, work, (void *)&thread_ids[i]);
    }

    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);

    for (int i = 0; i < THREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    printf("Number of prime numbers between 2 and %d: %d\n", N, total);
    for (int i = 0; i < total; i++) {
        printf("%d\n", primes[i]);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}