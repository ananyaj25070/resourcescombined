#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>   


int main() {
    int status;
    status = mkfifo("fifo-pipe",0775); 
    printf("named pipe created with status %d\n", status);
}