#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

pthread_t test,test1;
pthread_mutex_t mutex;
pthread_cond_t cond;
int testflag = 1;

static void* uevent_thread_routine(__attribute__((unused))void *arg)
{
                printf("pthread 1\n");
        while(1){
                if(testflag == 1){
                     sleep(1);
                     printf(" 1\n");  
                     usleep(500000);
                     usleep(500000);
                     printf(" 11\n");
                }  
                else if(testflag == 2)
                     printf(" 2\n");  
                else if(testflag == 3)
                     printf(" 3\n");  
                else 
                     printf(" 4\n"); 
                sleep(5);
        }
}

static void* uevent_thread_routine1(__attribute__((unused))void *arg)
{
        printf("pthread 2\n");
        int tmp = 5;
        while(1)
        {
            sleep(100);
            testflag = tmp % 4;
            tmp++;
        }      
}

int main()
{
        int ret = 0;
	pthread_attr_t attr,attr1;

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	pthread_attr_init(&attr);
	pthread_attr_init(&attr1);

	ret = pthread_create(&test, &attr, uevent_thread_routine, NULL);
	if (ret != 0) 
	{
		printf("create uevt pthread failed.\n");
	}
       //pthread_detach(test,NULL);

	ret = pthread_create(&test1, &attr1, uevent_thread_routine1, NULL);
	if (ret != 0) 
	{
		printf("create uevt pthread failed.\n");
	}
        
        while(1);
        //pthread_join(test1,NULL);

        return 0;
}

#if 0
int pthread_create(pthread_t *tidp,const pthread_attr_t *attr,
(void*)(*start_rtn)(void*),void *arg);
#endif
