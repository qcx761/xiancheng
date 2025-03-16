//局部变量
#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg) {
    int local_counter = 0;  // 局部变量
    for (int i = 0; i < 5; i++) {
        local_counter++;
        printf("Thread %ld: local_counter = %d\n", pthread_self(), local_counter);
    }
    return NULL;
}

int main() {
    pthread_t threads[2];
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}

//key
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_key_t key;

void my_destructor(void *ptr) {
    free(ptr);
}

void* thread_function(void* arg) {
    char *data = malloc(20);
    snprintf(data, 20, "Data from Thread %ld", pthread_self());
    pthread_setspecific(key, data);
    printf("Thread %ld: %s\n", pthread_self(), (char*)pthread_getspecific(key));
    return NULL;
}

int main() {
    pthread_t threads[2];
    pthread_key_create(&key, my_destructor);
    for (int i = 0; i < 2; i++) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_key_delete(key);
    return 0;
}