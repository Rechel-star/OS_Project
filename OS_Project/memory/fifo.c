#include <stdio.h>

int main() {

    int pages[50];
    int frames[10];

    int n, capacity;

    int page_faults = 0;

    int index = 0;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    printf("Enter page reference string:\n");

    for(int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Enter number of frames: ");
    scanf("%d", &capacity);

    for(int i = 0; i < capacity; i++) {
        frames[i] = -1;
    }

    printf("\nPage Replacement Process:\n");

    for(int i = 0; i < n; i++) {

        int found = 0;

        for(int j = 0; j < capacity; j++) {

            if(frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(!found) {

            frames[index] = pages[i];

            index = (index + 1) % capacity;

            page_faults++;

            printf("Page %d -> ", pages[i]);

            for(int j = 0; j < capacity; j++) {

                if(frames[j] != -1)
                    printf("%d ", frames[j]);
                else
                    printf("- ");
            }

            printf("(Fault)\n");

        } else {

            printf("Page %d -> Hit\n", pages[i]);
        }
    }

    float fault_rate =
        (float)page_faults / n * 100;

    printf("\nTotal Page Faults: %d\n",
           page_faults);

    printf("Page Fault Rate: %.2f%%\n",
           fault_rate);

    return 0;
}
