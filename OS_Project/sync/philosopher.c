#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // 哲学家数量

pthread_mutex_t chopsticks[N]; // 5把互斥锁代表5根筷子
sem_t room; // 限制最多允许 N-1 个哲学家同时去拿筷子

// 哲学家线程
void* philosopher(void* arg) {
    int id = *(int*)arg;
    int left = id;
    int right = (id + 1) % N;

    for(int i = 0; i < 3; i++) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(1);

        // 关键死锁预防：申请进入进餐队列的名额
        sem_wait(&room); 
        
        // 获取左右两根筷子
        pthread_mutex_lock(&chopsticks[left]);
        printf("Philosopher %d picked up left chopstick %d.\n", id, left);
        pthread_mutex_lock(&chopsticks[right]);
        printf("Philosopher %d picked up right chopstick %d.\n", id, right);

        // 临界区：进餐
        printf("Philosopher %d is eating.\n", id);
        sleep(2);

        // 放下筷子
        pthread_mutex_unlock(&chopsticks[left]);
        pthread_mutex_unlock(&chopsticks[right]);
        
        // 归还进餐名额
        sem_post(&room); 
    }
    return NULL;
}

int main() {
    pthread_t philo[N];
    int ids[N];

    // 初始化 5 把筷子互斥锁
    for(int i = 0; i < N; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
    }
    // 初始化容量为 4 的信号量，这是破坏死锁环路的核心
    sem_init(&room, 0, N - 1); 

    // 创建哲学家并发线程
    for(int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philo[i], NULL, philosopher, &ids[i]);
    }

    for(int i = 0; i < N; i++) {
        pthread_join(philo[i], NULL);
    }

    for(int i = 0; i < N; i++) {
        pthread_mutex_destroy(&chopsticks[i]);
    }
    sem_destroy(&room);

    return 0;
}
