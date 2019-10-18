#include "head4fifo.h"

int main(int argc, char **argv)
{
        if(access(FIFO, F_OK)) {
                mkfifo(FIFO, 0644);
        }

        int fifo = open(FIFO, O_WRONLY);

        char *msg = "my fifo  test I am jack\n";

        int n = write(fifo, msg, strlen(msg));

        printf("%d bytes have been sended.\n", n);
        return 0;

}
