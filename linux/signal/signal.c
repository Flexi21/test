#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void handler(int signum)
{
    if(signum == SIGIO)
        printf("SIGIO   signal: %d\n", signum);
    else if(signum == SIGUSR1)
        printf("SIGUSR1   signal: %d\n", signum);
    else
        printf("error\n");
}

void main(void)
{
    signal(SIGIO, handler);
    signal(SIGUSR1, handler);

    printf("%d  %d\n", SIGIO, SIGUSR1);
    for(;;)
    {
        sleep(10000);
    }
}
