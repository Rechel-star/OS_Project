#include <stdio.h>
#include "process.h"

int main() {

    int n, quantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];

    for(int i = 0; i < n; i++) {

        p[i].pid = i + 1;

        printf("\nProcess %d\n", i + 1);

        printf("Burst Time: ");
        scanf("%d", &p[i].burst);

        p[i].remaining = p[i].burst;
    }

    printf("\nEnter Time Quantum: ");
    scanf("%d", &quantum);

    int current_time = 0;
    int completed = 0;

    float total_waiting = 0;
    float total_turnaround = 0;

    printf("\nExecution Order:\n");

    while(completed < n) {

        for(int i = 0; i < n; i++) {

            if(p[i].remaining > 0) {

                printf("P%d ", p[i].pid);

                if(p[i].remaining > quantum) {

                    current_time += quantum;
                    p[i].remaining -= quantum;

                } else {

                    current_time += p[i].remaining;

                    p[i].waiting =
                        current_time - p[i].burst;

                    p[i].turnaround = current_time;

                    total_waiting += p[i].waiting;
                    total_turnaround += p[i].turnaround;

                    p[i].remaining = 0;

                    completed++;
                }
            }
        }
    }

    printf("\n\n");

    printf("PID\tBurst\tWaiting\tTurnaround\n");

    for(int i = 0; i < n; i++) {

        printf("P%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].burst,
               p[i].waiting,
               p[i].turnaround);
    }

    printf("\nAverage Waiting Time: %.2f\n",
           total_waiting / n);

    printf("Average Turnaround Time: %.2f\n",
           total_turnaround / n);

    return 0;
}
