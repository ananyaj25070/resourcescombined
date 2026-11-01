#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// Helper function to print policy name
const char* policy_name(int policy) {
    switch (policy) {
        case SCHED_OTHER: return "SCHED_OTHER";
        case SCHED_FIFO:  return "SCHED_FIFO";
        case SCHED_RR:    return "SCHED_RR";
#ifdef SCHED_BATCH
        case SCHED_BATCH: return "SCHED_BATCH";
#endif
#ifdef SCHED_IDLE
        case SCHED_IDLE:  return "SCHED_IDLE";
#endif
#ifdef SCHED_DEADLINE
        case SCHED_DEADLINE: return "SCHED_DEADLINE";
#endif
        default: return "UNKNOWN";
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;
    struct sched_param param;
    int policy;

    if (argc < 2) {
        pid = getpid();  // Default: which is current process
        printf("No PID given, using self (PID=%d)\n", pid);
    } else {
        pid = atoi(argv[1]);  // Use given PID to test
        printf("Using PID=%d\n", pid);
    }

    // 1. Get current policy
    policy = sched_getscheduler(pid);
    if (policy == -1) {
        perror("sched_getscheduler");
        return 1;
    }
    printf("Current policy: %s\n", policy_name(policy));

    // 2. Get current priority
    if (sched_getparam(pid, &param) == -1) {
        perror("sched_getparam");
        return 1;
    }
    printf("Current priority: %d\n", param.sched_priority);

    // 3. Try to change priority
    param.sched_priority = 5; // new priority
    if (sched_setparam(pid, &param) == -1) {
        printf("sched_setparam failed: %s\n", strerror(errno)); // if the process is not in fifo,rr policy then it will give error
    } else {
        printf("Priority changed to: %d\n", param.sched_priority); // process is in fifo,rr policy then it will change
    }

    // 4. Try to change policy (requires root)
    param.sched_priority = 1; // Minimal priority for FIFO/RR (Range is 1-99)
    if (sched_setscheduler(pid, SCHED_FIFO, &param) == -1) {
        printf("sched_setscheduler failed: %s\n", strerror(errno));
    } else {
        printf("Policy changed to: SCHED_FIFO\n");
    }

    // 5. Check again
    policy = sched_getscheduler(pid);
    if (sched_getparam(pid, &param) == -1) {
        perror("sched_getparam");
        return 1;
    }
    printf("After changes → Policy: %s, Priority: %d\n",
           policy_name(policy), param.sched_priority);

    return 0;
}

//
// Compile with: gcc sched_setscheduler.c -o sched_setscheduler
// Run with: sudo ./sched_setscheduler [PID]
// Note: Changing scheduling policy to FIFO.