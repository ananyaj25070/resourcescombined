#include <stdio.h>
#include <limits.h>

#define MAX_PROCESSES 10

// This is the struct representing a process.
typedef struct {
    int id;         // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int remaining;  // Remaining burst time
    int completion; // Completion time
    int start;      // Start time
    int finished;   // Flag to check if the process is finished
} Process;

// Function to find the process with the shortest remaining time
int findShortestRemainingTime(Process processes[], int n, int currentTime) {
    int minTime = INT_MAX;
    int index = -1;

    for (int i = 0; i < n; i++) {
        if (processes[i].arrival <= currentTime && processes[i].remaining > 0 && processes[i].remaining < minTime) {
            minTime = processes[i].remaining;
            index = i;
        }
    }
    return index;
}

void stcfScheduling(Process processes[], int n) {
    int completed = 0;
    int currentTime = 0;
    int lastIndex = -1;
    int current_idx=-1;

    while (completed < n) {
        int index = findShortestRemainingTime(processes, n, currentTime);

        if (index == -1) {
            currentTime++; 
            printf("came here");
            if (lastIndex!=-1) {
                printf("The process running is %d",processes[lastIndex].id);
            }
            
            continue;
        }

        // current_idx=index;

        // Start the process if not already started
        if (processes[index].remaining == processes[index].burst) {
            processes[index].start = currentTime;
        }

        // Execute the process with the shortest remaining time
        processes[index].remaining--;
        currentTime++;
        
        // If the process is completed
        if (processes[index].remaining == 0) {
            processes[index].completion = currentTime;
            processes[index].finished = 1;
            completed++;
        }

        // Print the process execution for demonstration
        if (lastIndex != index) {
            printf("Time %d - %d: P%d\n", currentTime - 1, currentTime, processes[index].id);
            lastIndex = index;
        } else {
            if (lastIndex!=-1) {
                printf("The process running is %d \n",processes[lastIndex].id);
            }
        }
    }

    // Display process details
    printf("\nProcess\tArrival\tBurst\tStart\tCompletion\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\n", processes[i].id, processes[i].arrival, processes[i].burst, processes[i].start, processes[i].completion);
    }
    printf("\nTotal time taken for all processes to complete: %d\n", currentTime);
}

int main() {
    int n;
    Process processes[MAX_PROCESSES];

    // Input number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Input arrival time and burst time for each process
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for process P%d: ", i + 1);
        scanf("%d%d", &processes[i].arrival, &processes[i].burst);
        processes[i].id = i + 1;
        processes[i].remaining = processes[i].burst; // Set remaining time equal to burst time initially
        processes[i].finished = 0; // Process is not yet finished
    }

    // Run STCF scheduling
    stcfScheduling(processes, n);

    return 0;
}
