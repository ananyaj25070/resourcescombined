#include <stdio.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_BURSTS 10

// Process structure
struct Process {
    char id[10];
    int arrival;
    int bursts[MAX_BURSTS];
    int io[MAX_BURSTS];
    int num_bursts;
    
    // For simulation
    int current_burst;
    int remaining_time;
    int io_time;
    int state; // 0=not_arrived, 1=ready, 2=running, 3=io, 4=finished
    int ready_time;
    
    // Metrics
    int completion;
    int turnaround;
    int waiting;
    int response;
    int first_run;
};

struct Process processes[MAX_PROCESSES];
int num_processes = 0;

// Input function
void get_input() {
printf("Enter number of processes: ");
    scanf("%d", &num_processes);
    
    printf("\nInput format:\n");
    printf("Process_ID Arrival_Time Number_of_CPU_Bursts\n");
    printf("CPU_Burst_1 IO_Duration_1 CPU_Burst_2 IO_Duration_2 ... CPU_Burst_n\n");
    printf("Example: P1 0 3\n");
    printf("         4 2 3 3 2\n\n");
    
    for (int i = 0; i < num_processes; i++) {
        printf("--- Process %d ---\n", i + 1);
        printf("Enter Process_ID Arrival_Time Number_of_CPU_Bursts: ");
        scanf("%s %d %d", processes[i].id, &processes[i].arrival, &processes[i].num_bursts);
        
        printf("Enter CPU_Burst_1 IO_Duration_1 CPU_Burst_2 IO_Duration_2 ... CPU_Burst_n: ");
        for (int j = 0; j < processes[i].num_bursts; j++) {
            scanf("%d", &processes[i].bursts[j]);
            if (j < processes[i].num_bursts - 1) {
                scanf("%d", &processes[i].io[j]);
            }
        }
        
        // Initialize
        processes[i].current_burst = 0;
        processes[i].remaining_time = processes[i].bursts[0];
        processes[i].io_time = 0;
        processes[i].state = 0; // not arrived
        processes[i].ready_time = 0;
        processes[i].first_run = -1;
    }
}

// Find next FIFO process
int find_fifo() {
    int earliest = -1;
    int min_time = 999999;
    
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].state == 1 && processes[i].ready_time < min_time) {
            min_time = processes[i].ready_time;
            earliest = i;
        }
    }
    return earliest;
}

// FIFO simulation
void simulate() {
    int time = 0;
    int running = -1;
    int total_cpu_time = 0;
    
    printf("\n--- Starting FIFO Simulation ---\n");
    
    while (1) {
        // Check arrivals
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state == 0 && processes[i].arrival <= time) {
                processes[i].state = 1; // ready
                processes[i].ready_time = time;
                printf("Time %d: %s has arrived and is Ready\n", time, processes[i].id);
            }
        }
        
        // Update I/O processes
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state == 3) { // in I/O
                processes[i].io_time--;
                if (processes[i].io_time == 0) {
                    processes[i].current_burst++;
                    if (processes[i].current_burst < processes[i].num_bursts) {
                        processes[i].state = 1; // ready
                        processes[i].ready_time = time + 1;
                        processes[i].remaining_time = processes[i].bursts[processes[i].current_burst];
                        printf("Time %d: %s completes I/O and is Ready for CPU burst %d (%d units)\n", 
                               time + 1, processes[i].id, processes[i].current_burst + 1, processes[i].remaining_time);
                    } else {
                        processes[i].state = 4; // finished
                        processes[i].completion = time;
                        processes[i].turnaround = processes[i].completion - processes[i].arrival;
                        printf("Time %d: %s has finished all its bursts\n", time, processes[i].id);
                    }
                }
            }
        }
        
        // Schedule if CPU idle
        if (running == -1) {
            running = find_fifo();
            if (running != -1) {
                processes[running].state = 2; // running
                if (processes[running].first_run == -1) {
                    processes[running].first_run = time;
                    processes[running].response = time - processes[running].arrival;
                }
            }
        }
        
        // Execute running process
        if (running != -1) {
            processes[running].remaining_time--;
            total_cpu_time++;
            
            // Check if burst completed
            if (processes[running].remaining_time == 0) {
                if (processes[running].current_burst < processes[running].num_bursts - 1) {
                    // Go to I/O
                    processes[running].state = 3; // I/O
                    processes[running].io_time = processes[running].io[processes[running].current_burst];
                    printf("Time %d: %s finishes CPU burst %d, moves to I/O for %d units\n", 
                           time + 1, processes[running].id, processes[running].current_burst + 1, processes[running].io_time);
                } else {
                    // Process finished
                    processes[running].state = 4; // finished
                    processes[running].completion = time + 1;
                    processes[running].turnaround = processes[running].completion - processes[running].arrival;
                    printf("Time %d: %s has finished all its bursts\n", time + 1, processes[running].id);
                }
                running = -1;
            }
        }
        
        time++;
        
        // Check if all finished
        int all_done = 1;
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].state != 4) {
                all_done = 0;
                break;
            }
        }
        if (all_done) {
            printf("Time %d: All processes have completed. Simulation ending.\n", time);
            break;
        }
    }
    
    // Calculate waiting times
    for (int i = 0; i < num_processes; i++) {
        int total_cpu = 0, total_io = 0;
        for (int j = 0; j < processes[i].num_bursts; j++) {
            total_cpu += processes[i].bursts[j];
            if (j < processes[i].num_bursts - 1) {
                total_io += processes[i].io[j];
            }
        }
        processes[i].waiting = processes[i].turnaround - total_cpu - total_io;
    }
    
    printf("--- FIFO Simulation Complete ---\n");
    
    // Display results
    printf("\n================== FIFO RESULTS ==================\n");
    printf("Total Simulation Time: %d\n", time);
    printf("CPU Busy Time: %d\n", total_cpu_time);
    printf("CPU Utilization: %.2f%%\n\n", (double)total_cpu_time / time * 100);
        printf("Process metrics:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%s - Turnaround %d, Waiting %d, Response %d\n",
               processes[i].id, processes[i].turnaround, processes[i].waiting, processes[i].response);
    }
}

int main() {
    printf("=== FIFO CPU Scheduler ===\n");
    get_input();
    simulate();
    return 0;
}