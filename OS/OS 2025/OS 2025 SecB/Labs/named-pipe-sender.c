#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>


int main() {
    int fd, n;
    char message[20] = "Hello World!";

    fd = open("fifo-pipe",O_WRONLY);
    write(fd, "Hello World!", strlen(message));
    printf("Sender Process %d sent the data\n",getpid());
    close(fd);
}