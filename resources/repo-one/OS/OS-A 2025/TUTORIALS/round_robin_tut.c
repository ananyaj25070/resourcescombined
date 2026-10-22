#include <stdio.h>
#define MAX_PROCESSES 10

// This is the process struct
typedef struct {
    int id;         // Process ID
    int burst;      // Burst time
    int remaining;  // Remaining burst time
    int completion; // Completion time
    int waiting;    // Waiting time
} Process;


void roundRobinScheduling(Process processes[], int n, int timeQuantum) {
    int currentTime = 0;
    int completed = 0;

    // Loop until all processes are completed
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            // Check if the process has remaining burst time
            if (processes[i].remaining > 0) {
                // Execute the process for a time quantum or its remaining burst time
                int executionTime = (processes[i].remaining < timeQuantum) ? processes[i].remaining : timeQuantum;
                processes[i].remaining -= executionTime;
                currentTime += executionTime;

                // Print the process execution for demonstration
                printf("Time %d - %d: P%d\n", currentTime - executionTime, currentTime, processes[i].id);

                // If the process is completed
                if (processes[i].remaining == 0) {
                    processes[i].completion = currentTime;
                    completed++;
                }
            }
        }
    }

    // Calculate waiting time for each process
    for (int i = 0; i < n; i++) {
        processes[i].waiting = processes[i].completion - processes[i].burst;
    }

    // Display process details
    printf("\nProcess\tBurst\tCompletion\tWaiting\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t\t%d\n", processes[i].id, processes[i].burst, processes[i].completion, processes[i].waiting);
    }

    // Calculate and display average waiting and completion times
    float totalWaitingTime = 0;
    float totalCompletionTime = 0;
    for (int i = 0; i < n; i++) {
        totalWaitingTime += processes[i].waiting;
        totalCompletionTime += processes[i].completion;
    }
    printf("\nAverage Waiting Time: %.2f\n", totalWaitingTime / n);
    printf("Average Completion Time: %.2f\n", totalCompletionTime / n);
}

int main() {
    int n, timeQuantum;
    Process processes[MAX_PROCESSES];

    // Input number of processes and time quantum
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the time quantum: ");
    scanf("%d", &timeQuantum);

    // Input burst time for each process
    for (int i = 0; i < n; i++) {
        printf("Enter burst time for process P%d: ", i + 1);
        scanf("%d", &processes[i].burst);
        processes[i].id = i + 1;
        processes[i].remaining = processes[i].burst; // Set remaining time equal to burst time initially
    }

    // Run Round Robin scheduling
    roundRobinScheduling(processes, n, timeQuantum);

    return 0;
}
