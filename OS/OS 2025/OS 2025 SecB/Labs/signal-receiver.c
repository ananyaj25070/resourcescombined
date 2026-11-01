#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Signal handler
void handle_sigusr1(int sig) {
    printf("Receiver: Got SIGUSR1 (%d) signal!\n", sig);
}

int main() {
    // Print PID so sender knows where to send signal
    printf("Receiver PID: %d\n", getpid());

    // Register signal handler
    signal(SIGUSR1, handle_sigusr1);

    // Keep process alive to catch signals
    while (1) {
        pause(); // sleep until a signal arrives
    }

    return 0;
}
