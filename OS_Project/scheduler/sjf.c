#include <stdio.h>
#include "process.h"

int main() {
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    int completed[n]; // 标记进程是否已经执行完毕：0为未完成，1为已完成

    for(int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        completed[i] = 0; 

        printf("\nProcess %d\n", i + 1);
        
        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);
        
        printf("Burst Time: ");
        scanf("%d", &p[i].burst);
    }

    int current_time = 0;
    int completed_count = 0;
    
    float total_waiting = 0;
    float total_turnaround = 0;

    printf("\nExecution Order:\n");

    // 当还有进程未完成时循环
    while(completed_count < n) {
        int shortest_idx = -1;
        int min_burst = 999999; // 初始化一个极大值

        // 在所有已经到达且未完成的进程中，寻找运行时间最短的
        for(int i = 0; i < n; i++) {
            if(completed[i] == 0 && p[i].arrival <= current_time) {
                if(p[i].burst < min_burst) {
                    min_burst = p[i].burst;
                    shortest_idx = i;
                }
            }
        }

        if(shortest_idx == -1) {
            // 如果当前时间点没有任何进程到达，CPU 空转，时间推进
            current_time++;
        } else {
            // 找到了符合条件的进程，开始执行
            p[shortest_idx].waiting = current_time - p[shortest_idx].arrival;
            
            // 运行进程，时间推进
            current_time += p[shortest_idx].burst;
            
            p[shortest_idx].turnaround = p[shortest_idx].waiting + p[shortest_idx].burst;

            total_waiting += p[shortest_idx].waiting;
            total_turnaround += p[shortest_idx].turnaround;

            completed[shortest_idx] = 1; // 标记为已完成
            completed_count++;

            printf("P%d ", p[shortest_idx].pid);
        }
    }

    printf("\n\n");
    printf("PID\tArrival\tBurst\tWaiting\tTurnaround\n");

    for(int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].waiting,
               p[i].turnaround);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);

    return 0;
}
