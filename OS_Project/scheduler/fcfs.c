#include <stdio.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int waiting;
    int turnaround;
} Process;

int main() {
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];

    for(int i = 0; i < n; i++) {
        printf("\nProcess %d\n", i + 1);

        p[i].pid = i + 1;

        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);

        printf("Burst Time: ");
        scanf("%d", &p[i].burst);
    }

    int current_time = 0;
    float total_waiting = 0;
    float total_turnaround = 0;

    printf("\nExecution Order:\n");

    for(int i = 0; i < n; i++) {

        if(current_time < p[i].arrival) {
            current_time = p[i].arrival;
        }

        p[i].waiting = current_time - p[i].arrival;

        current_time += p[i].burst;

        p[i].turnaround = p[i].waiting + p[i].burst;

        total_waiting += p[i].waiting;
        total_turnaround += p[i].turnaround;

        printf("P%d ", p[i].pid);
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

    printf("\nAverage Waiting Time: %.2f\n",
           total_waiting / n);

    printf("Average Turnaround Time: %.2f\n",
           total_turnaround / n);

    return 0;
}
