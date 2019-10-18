//#include <unist.h>
#include <string.h>
#include <wait.h>
#include <stdio.h>

#define MAX_LINE 80

int main()
{
	int testPipe[2], ret;
	char buf[MAX_LINE + 1];
	const char *testbuf = "主程序发送的数据";

	if (pipe(testPipe) == 0) {
		if (fork() == 0) {
			ret = read(testPipe[0], buf, MAX_LINE);
			buf[ret] = 0;
			printf("子程序读到的数据为：%s", buf);
			close(testPipe[0]);
		}else {
			ret = write(testPipe[1], testbuf, strlen(testbuf));
			ret = wait(NULL);
			close(testPipe[1]);
		}
	}

	return 0;
}
