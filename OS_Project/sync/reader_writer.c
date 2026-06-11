#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t rw_mutex;         // 用于保护共享数据的读写（写者互斥，读写互斥）
pthread_mutex_t mutex;  // 用于保护 reader_count 变量的原子操作
int reader_count = 0;   // 记录当前正在读取的读者数量
int shared_data = 0;    // 模拟的共享数据

// 读者线程
void* reader(void* arg) {
    int id = *(int*)arg;

    for(int i = 0; i < 3; i++) {
        // 进入区：保护对 reader_count 的修改
        pthread_mutex_lock(&mutex);
        reader_count++;
        // 如果是第一个读者，需要向写者申请阻塞，防止写者在此期间写入
        if (reader_count == 1) {
            sem_wait(&rw_mutex);
        }
        pthread_mutex_unlock(&mutex);

        // 临界区：执行读取操作（允许多个读者同时进入）
        printf("Reader %d is reading data: %d\n", id, shared_data);
        sleep(1);

        // 退出区：保护对 reader_count 的修改
        pthread_mutex_lock(&mutex);
        reader_count--;
        // 如果是最后一个读者，释放对写者的阻塞
        if (reader_count == 0) {
            sem_post(&rw_mutex);
        }
        pthread_mutex_unlock(&mutex);
        
        sleep(1);
    }
    return NULL;
}

// 写者线程
void* writer(void* arg) {
    int id = *(int*)arg;

    for(int i = 0; i < 3; i++) {
        // 申请写锁，确保互斥
        sem_wait(&rw_mutex);
        
        // 临界区：执行写入操作
        shared_data++;
        printf("Writer %d is writing data: %d\n", id, shared_data);
        sleep(2);
        
        // 释放写锁
        sem_post(&rw_mutex);
        
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t r[5], w[2];
    int r_ids[5] = {1, 2, 3, 4, 5};
    int w_ids[2] = {1, 2};

    sem_init(&rw_mutex, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    // 并发创建读者和写者线程
    for(int i = 0; i < 2; i++) pthread_create(&w[i], NULL, writer, &w_ids[i]);
    for(int i = 0; i < 5; i++) pthread_create(&r[i], NULL, reader, &r_ids[i]);

    for(int i = 0; i < 2; i++) pthread_join(w[i], NULL);
    for(int i = 0; i < 5; i++) pthread_join(r[i], NULL);

    sem_destroy(&rw_mutex);
    pthread_mutex_destroy(&mutex);

    return 0;
}
