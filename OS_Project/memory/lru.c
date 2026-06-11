#include <stdio.h>

int main() {

    int pages[50];
    int frames[10];
    int time[10];

    int n, capacity;

    int page_faults = 0;
    int counter = 0;

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
        time[i] = 0;
    }

    printf("\nPage Replacement Process:\n");

    for(int i = 0; i < n; i++) {

        int found = 0;

        for(int j = 0; j < capacity; j++) {

            if(frames[j] == pages[i]) {

                counter++;

                time[j] = counter;

                found = 1;

                break;
            }
        }

        if(!found) {

            int pos = -1;

            for(int j = 0; j < capacity; j++) {

                if(frames[j] == -1) {

                    pos = j;
                    break;
                }
            }

            if(pos == -1) {

                int lru = time[0];

                pos = 0;

                for(int j = 1; j < capacity; j++) {

                    if(time[j] < lru) {

                        lru = time[j];
                        pos = j;
                    }
                }
            }

            counter++;

            frames[pos] = pages[i];

            time[pos] = counter;

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
