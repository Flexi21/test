#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
     int fd = open("/sys/bus/platform/devices/et_general.0/etgeneral",O_RDONLY);
     if (fd < 0)
         printf("open failed\n");
    char *test = "1";    
    ssize_t ret = write(fd, test, sizeof(test));
     if (ret < 0)
         printf("write failed\n");
    
}
