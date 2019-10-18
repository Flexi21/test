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
   char *buf = "namefifotest!";
   unlink(TEST_FIFO);

  //mkfifo(FIFO_NAME, S_IFIFO|0666);
   mkfifo(TEST_FIFO,S_IFIFO|0666);

   int fd = open(TEST_FIFO, O_WRONLY);

   int ret = write(fd, buf, sizeof(buf)+10);

   close(fd);

   unlink(TEST_FIFO);

   return 0;
}
