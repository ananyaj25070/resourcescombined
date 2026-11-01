#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>


int main() {

    int fd, bytesRead;
    char message[20];
    fd = open("fifo-pipe",O_RDONLY);
    bytesRead = read(fd, message, sizeof(message)-1);

    if (bytesRead > 0) {
        message[bytesRead] = '\0'; //null terminate the string
    }

    printf("Reader process %d started\n",getpid());
    printf("Data received by receiver %d is: %s\n",getpid(), message);
    close(fd);
}