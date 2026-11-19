#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];          // Array to hold the file descriptors for the pipe
    int pid;
    char buffer[BUFFER_SIZE];
    const char* message = "Hello World!";
    
    // Create the anonymous pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork to create a parent-child process pair
    pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        close(pipefd[1]);   // Close the write end of the pipe (not needed in the child)
        
        // Read the message from the pipe
        read(pipefd[0], buffer, BUFFER_SIZE);
        printf("Child received: %s\n", buffer);

        close(pipefd[0]);   // Close the read end after reading
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        close(pipefd[0]);   // Close the read end of the pipe (not needed in the parent)

        // Write the message to the pipe
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);   // Close the write end after writing

        // Wait for the child process to finish
        wait(NULL);
    }

    return 0;
}
