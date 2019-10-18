#include <stdio.h>
#include<fcntl.h>

int main(void)
{
    int fd = 0,short data = 0;
    fd = open("/home/zwenlong/test/c/fd/hello",O_RDONLY);
    printf("fd:%d\n",fd);
    test(&data);
    printf("fd:%d data%d\n",fd,data);
    close(fd);
    return 0;
}


void test(void *point)
{
    *point = 1;
    printf("test! \n");

}
