#include "head4fifo.h"

int main(int argc, char **argv)
{

        if (access(FIFO, F_OK))
        {
                mkfifo(FIFO, 0644);
        }

        int fifo = open(FIFO, O_RDONLY);

        char msg[50];
        bzero(msg, 50);

        read(fifo, msg, 50);

        printf("from FIFO: %s", msg);

        return 0;



}
