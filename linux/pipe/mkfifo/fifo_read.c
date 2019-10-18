#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define TEST_FIFO "testfifo"

int main(void)
{
   char buf1[20];
   char *buf = "namefifotest!";

   int fd = open(TEST_FIFO, O_RDONLY);

   if (fd > 0) {
        int ret = read(fd, buf1, sizeof(buf)+10);

        printf("fifo read:%s \n",buf1);
   }

   close(fd);

   unlink(TEST_FIFO);

   return 0;
}
