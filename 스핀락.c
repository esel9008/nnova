#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    atomic_int lock;
} SpinLock;

void spinlock_init(SpinLock *s) {
    atomic_store(&s->lock, 0);
}

void spinlock_lock(SpinLock *s) {
    int expected;
    do {
        expected = 0;
    } while (!atomic_compare_exchange_weak(&s->lock, &expected, 1));
}

void spinlock_unlock(SpinLock *s) {
    atomic_store(&s->lock, 0);
}

SpinLock spin;

void* thread_func(void* arg) {
    spinlock_lock(&spin);
    printf("Thread %ld: critical section\n", (long)arg);
    sleep(1);
    spinlock_unlock(&spin);
    return NULL;
}

int main() {
    pthread_t threads[2];
    spinlock_init(&spin);

    for (long i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
