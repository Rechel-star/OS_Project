#include <stdio.h>

int main() {

    int blockSize[20];
    int processSize[20];

    int allocation[20];

    int m, n;

    printf("Enter number of memory blocks: ");
    scanf("%d", &m);

    for(int i = 0; i < m; i++) {

        printf("Block %d size: ", i + 1);
        scanf("%d", &blockSize[i]);
    }

    printf("\nEnter number of processes: ");
    scanf("%d", &n);

    for(int i = 0; i < n; i++) {

        printf("Process %d size: ", i + 1);
        scanf("%d", &processSize[i]);

        allocation[i] = -1;
    }

    for(int i = 0; i < n; i++) {

        int bestIdx = -1;

        for(int j = 0; j < m; j++) {

            if(blockSize[j] >= processSize[i]) {

                if(bestIdx == -1 ||
                   blockSize[j] < blockSize[bestIdx]) {

                    bestIdx = j;
                }
            }
        }

        if(bestIdx != -1) {

            allocation[i] = bestIdx;

            blockSize[bestIdx] -= processSize[i];
        }
    }

    printf("\nProcess No.\tProcess Size\tBlock No.\n");

    for(int i = 0; i < n; i++) {

        printf("%d\t\t%d\t\t",
               i + 1,
               processSize[i]);

        if(allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }

    printf("\nRemaining Memory:\n");

    for(int i = 0; i < m; i++) {

        printf("Block %d: %d\n",
               i + 1,
               blockSize[i]);
    }

    return 0;
}
