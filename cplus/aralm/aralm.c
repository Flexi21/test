#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void func()
{
    printf("this is func\n");
}

int main()
{

     signal(SIGALRM, func); //2s后要执行的函数ls
     alarm(2);//设置定时2s

     while (1);

     return 0;
}
