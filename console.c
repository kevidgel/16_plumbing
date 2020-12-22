#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <signal.h>

static void sighandler(int signo)
{
    if (signo == SIGINT)
    {
        remove("ctp");
        remove("ptc");
        printf("\n");
        exit(0);   
    }
}

int main() 
{
    // signal 
    signal(SIGINT, sighandler);

    // pipes
    mkfifo("ctp", 0666);
    mkfifo("ptc", 0666);
    
    // open pipes
    int in = open("ctp", O_WRONLY); 
    if (in == -1) {
        printf("errno: %d\terror: %s\n", errno, strerror(errno));
        return -1;
    }

    int out = open("ptc", O_RDONLY);
    if (out == -1) {
        printf("errno: %d\terror: %s\n", errno, strerror(errno));
        return -1;
    } 
    
    // in, out 
    char userinput[100], processed[100];
    while(1)
    {

        // gets user input
        printf("Input   : " );
        fgets(userinput, sizeof(userinput), stdin);
        if(isspace(userinput[strlen(userinput) - 1])) userinput[strlen(userinput) - 1] = '\0';

        // sends to processor
        int w = write(in, &userinput, strlen(userinput) + 1); 
        if (w == -1) {
            printf("errno: %d\terror: %s\n", errno, strerror(errno));
            break;
        } 


        // reads from processor
        int r = read(out, &processed, sizeof(processed));
        if (r == -1) {
            printf("errno: %d\terror: %s\n", errno, strerror(errno));
            break;
        } 

        // prints processed
        printf("Response: %s\n", processed);
        
    }

    close(in);
    close(out);

    return 0;
}