#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
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

char *process(char *str)
{
    char *out = malloc((strlen(str)) * sizeof(char)); 
    int i = 0; 
    while(*str)
    {
        if(*str <= 'z' && *str >= 'a') out[i] = (((*str - 'a') + 13) % 26) + 'a';
        else if(*str <= 'Z' && *str >= 'A') out[i] = (((*str - 'A') + 13) % 26) + 'A';
        else out[i] = *str;  
        i++;
        str++;
    } 
    out[i] = '\0';
    return out;
}

int main()
{   
    // signal 
    signal(SIGINT, sighandler);

    // pipes
    mkfifo("ctp", 0666);
    mkfifo("ptc", 0666);

    int in = open("ctp", O_RDONLY); 
    if (in == -1) {
        printf("errno: %d\terror: %s\n", errno, strerror(errno));
        return -1; 
    }

    int out = open("ptc", O_WRONLY);
    if (out == -1) {
        printf("errno: %d\terror: %s\n", errno, strerror(errno));
        return -1;
    } 

    char userinput[100];
    while(1)
    {
        // reads from console
        int r = read(in, userinput, sizeof(userinput));
        if (r == -1) {
            printf("errno: %d\terror: char %s\n", errno, strerror(errno));
            break;
        } 

        char *processed = process(userinput);

        // sends to console
        int w = write(out, processed, strlen(processed) + 1); 
        if (w == -1) {
            printf("errno: %d\terror: %s\n", errno, strerror(errno));
            break;
        }

        free(processed);
        
    }

    close(in);
    close(out);

    return 0;
}