#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_BURSTS 20
#define MAX_LOG 1000

// Process states
typedef enum {
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
} State;

// Process structure
typedef struct {
    char name[10];
    int bursts[MAX_BURSTS]; // Alternating CPU and I/O burst times
    int burst_count;        // Total number of bursts
    int burst_index;        // Current burst index
    int time_left;          // Time left in current burst
    State state;            // Current state
    int completion_time;    // When process finished
} Process;

Process processes[MAX_PROCESSES];
int process_count = 0;

// Log structure
char event_log[MAX_LOG][100];
int log_count = 0;

// Log an event with timestamp
void log_event(int time, const char *msg) {
    if (log_count < MAX_LOG) {
        snprintf(event_log[log_count++], 100, "Time %d: %s", time, msg);
    }
}

// Add a process to the system
void add_process(const char *name, int *bursts, int burst_count) {
    if (process_count < MAX_PROCESSES) {
        Process *p = &processes[process_count++];
        strcpy(p->name, name);
        memcpy(p->bursts, bursts, burst_count * sizeof(int));
        p->burst_count = burst_count;
        p->burst_index = 0;
        p->time_left = bursts[0];
        p->state = READY;
        p->completion_time = -1;
    }
}
// Find the next ready process (FCFS)
int find_next_ready() {
    for (int i = 0; i < process_count; i++) {
        if (processes[i].state == READY)
            return i;
    }
    return -1;
}

// Check if all processes are finished
int all_finished() {
    for (int i = 0; i < process_count; i++) {
        if (processes[i].state != FINISHED)
            return 0;
    }
    return 1;
}

int main() {
    int mode;
    printf("Select I/O mode (1 = overlapping, 0 = non-overlapping): ");
    scanf("%d", &mode);

    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    process_count = 0;
    for (int i = 0; i < n; i++) {
        char name[10];
        int bursts[MAX_BURSTS];
        int burst_count = 0;
        printf("Enter name for process %d: ", i + 1);
        scanf("%s", name);
        printf("Enter burst times for %s (space separated, end with -1): ", name);
        int val;
        while (burst_count < MAX_BURSTS) {
            scanf("%d", &val);
            if (val == -1) break;
            bursts[burst_count++] = val;
        }
        add_process(name, bursts, burst_count);
    }

    int time = 0;
    int running = -1; // Index of running process

    if (mode == 0) {
        // Strict non-overlapping: finish one process completely before starting the next.
        for (int i = 0; i < process_count; i++) {
            Process *p = &processes[i];
            log_event(time, "Scheduler starts process");
            while (p->state != FINISHED) {
                if (p->burst_index % 2 == 0) { // CPU Burst
                    char msg[100];
                    snprintf(msg, 100, "%s starts CPU burst (%d units)", p->name, p->time_left);
                    log_event(time, msg);
                    time += p->time_left;
                    p->time_left = 0;
                    snprintf(msg, 100, "%s finishes CPU burst", p->name);
                    log_event(time, msg);
                } else { // I/O Burst
                    char msg[100];
                    snprintf(msg, 100, "%s starts I/O burst (%d units), CPU is idle", p->name, p->time_left);
                    log_event(time, msg);
                    time += p->time_left;
                    p->time_left = 0;
                    snprintf(msg, 100, "%s finishes I/O burst", p->name);
                    log_event(time, msg);
                }

                p->burst_index++;
                if (p->burst_index < p->burst_count) {
                    p->time_left = p->bursts[p->burst_index];
                } else {
                    p->state = FINISHED;
                    p->completion_time = time;
                    char msg[100];
                    snprintf(msg, 100, "%s has finished all bursts at time %d", p->name, time);
                    log_event(time, msg);
                }
            }
        }
       } else {
        // Overlapping I/O mode
        while (!all_finished()) {
            // 1. Update blocked processes (I/O)
            for (int i = 0; i < process_count; i++) {
                Process *p = &processes[i];
                if (p->state == BLOCKED) {
                    p->time_left--;
                    if (p->time_left == 0) {
                        p->burst_index++;
                        if (p->burst_index < p->burst_count) {
                            p->time_left = p->bursts[p->burst_index];
                            p->state = READY;
                            char msg[50];
                            snprintf(msg, 50, "%s completed I/O, moved to Ready", p->name);
                            log_event(time, msg);
                        } else {
                             p->state = FINISHED;
                             // CORRECTED: A process finishes at the end of a time slice.
                             p->completion_time = time + 1;
                             char msg[50];
                             snprintf(msg, 50, "%s finished at time %d", p->name, time + 1);
                             log_event(time, msg);
                        }
                    }
                }
            }

            // 2. If CPU is free, schedule next ready process
            if (running == -1 || processes[running].state != RUNNING) {
                running = find_next_ready();
                if (running != -1) {
                    processes[running].state = RUNNING;
                    char msg[50];
                    snprintf(msg, 50, "%s scheduled on CPU", processes[running].name);
                    log_event(time, msg);
                }
            }

            // 3. Run the process on CPU
          if (running != -1 && processes[running].state == RUNNING) {
                Process *p = &processes[running];
                p->time_left--;
                if (p->time_left == 0) {
                    p->burst_index++;
                    if (p->burst_index < p->burst_count) {
                        // Next burst is I/O
                        p->time_left = p->bursts[p->burst_index];
                        p->state = BLOCKED;
                        char msg[50];
                        snprintf(msg, 50, "%s completed CPU burst, moved to Blocked for I/O", p->name);
                        log_event(time, msg);
                        running = -1;
                    } else {
                        // Process finished
                        p->state = FINISHED;
                        // CORRECTED: A process finishes at the end of a time slice.
                        p->completion_time = time + 1;
                        char msg[50];
                        snprintf(msg, 50, "%s finished at time %d", p->name, time + 1);
                        log_event(time, msg);
                        running = -1;
                    }
                }
            }
            time++;
        }
    }

    // Print event log
    printf("\n--- Event Log ---\n");
    for (int i = 0; i < log_count; i++) {
        printf("%s\n", event_log[i]);
    }

    // Print completion times
    printf("\n--- Completion Times ---\n");
    for (int i = 0; i < process_count; i++) {
        printf("%s: %d\n", processes[i].name, processes[i].completion_time);
    }
    printf("\nTotal simulation time: %d\n", time);

    return 0;
}