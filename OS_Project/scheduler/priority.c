#include <stdio.h>
#include "process.h"

void sortByPriority(Process p[], int n) {

    Process temp;

    for(int i = 0; i < n - 1; i++) {

        for(int j = 0; j < n - i - 1; j++) {

            if(p[j].priority > p[j + 1].priority) {

                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}

int main() {

    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];

    for(int i = 0; i < n; i++) {

        p[i].pid = i + 1;

        printf("\nProcess %d\n", i + 1);

        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);

        printf("Burst Time: ");
        scanf("%d", &p[i].burst);

        printf("Priority: ");
        scanf("%d", &p[i].priority);
    }

    sortByPriority(p, n);

    int current_time = 0;

    float total_waiting = 0;
    float total_turnaround = 0;

    printf("\nExecution Order:\n");

    for(int i = 0; i < n; i++) {

        if(current_time < p[i].arrival) {
            current_time = p[i].arrival;
        }

        p[i].waiting =
            current_time - p[i].arrival;

        current_time += p[i].burst;

        p[i].turnaround =
            p[i].waiting + p[i].burst;

        total_waiting += p[i].waiting;
        total_turnaround += p[i].turnaround;

        printf("P%d ", p[i].pid);
    }

    printf("\n\n");

    printf("PID\tPriority\tBurst\tWaiting\tTurnaround\n");

    for(int i = 0; i < n; i++) {

        printf("P%d\t%d\t\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].priority,
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
