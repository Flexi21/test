#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <limits.h>  
  
int main(int argc, char *argv[])  
{  
    char strCurPath[PATH_MAX];  
  
    if(daemon(1, 1) < 0)  
    {  
        printf("error daemon...\n");  
        exit(1);  
    }  
    sleep(10);  
  
    if(getcwd(strCurPath, PATH_MAX) == NULL)  
    {  
        printf("error getcwd");  
        exit(1);  
    }  
    printf("%s\n", strCurPath);  
    return 0;  
}  

