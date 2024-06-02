#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS 4
#define N 3000

int primes[N];
int pflag[N];
int total = 0;
sem_t sem; // 세마포어 선언

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
    int start;
    int end;
    int i;
    start = (N / THREADS) * (*(int *)arg);
    end = start + N / THREADS;
    for (i = start; i < end; i++) {
        if (is_prime(i)) {
            sem_wait(&sem); // 세마포어 잠금
            primes[total] = i;
            total++;
            sem_post(&sem); // 세마포어 해제
        }
    }
    return NULL;
}

int main(int argn, char **argv)
{
    int i;
    pthread_t tids[THREADS];
    int thread_ids[THREADS];

    // pflag 배열 초기화
    for (i = 0; i < N; i++) {
        pflag[i] = 1;
    }

    // 세마포어 초기화
    sem_init(&sem, 0, 1);

    // 스레드 생성
    for (i = 0; i < THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&tids[i], NULL, work, (void *)&thread_ids[i]);
    }

    // 스레드 종료 대기
    for (i = 0; i < THREADS; i++) {
        pthread_join(tids[i], NULL);
    }

    // 세마포어 해제
    sem_destroy(&sem);

    // 결과 출력
    printf("Number of prime numbers between 2 and %d: %d\n", N, total);
    for (i = 0; i < total; i++) {
        printf("%d\n", primes[i]);
    }

    return 0;
}