#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <receiver-pid>\n", argv[0]);
        exit(1);
    }

    pid_t pid = atoi(argv[1]);

    if (kill(pid, SIGUSR1) == -1) {
        perror("kill");
        exit(1);
    }

    printf("Sender: Sent SIGUSR1 to process %d\n", pid);
    return 0;
}
