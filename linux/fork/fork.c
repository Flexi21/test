#include <stdio.h>
#include <unistd.h>

       //pid_t fork(void);
void main(void)
{
        int i = 0;
        int ret = 1,ret2 = 2;
        ret = fork();
        printf("ret %d\n",ret);
        if (0 == ret) {

                ret2 = fork();
                printf("ret2 %d\n",ret2);

                for (i = 0; i < 10; i++)
                {
                        sleep(1);
                        printf("son %d \n",i);
                }
        } else {
                for (i = 0; i < 10; i++)
                {
                        sleep(1);
                        printf("parent %d\n",i);
                }
        }
}
