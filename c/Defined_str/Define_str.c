#include <stdio.h>
#include <string.h>



//#define ET_KEY_PARAM_INIT(name)    #(a##name)     
#if 0                                     
        \
                           strcat(name,"x") == NULL ? name : NULL
#endif

struct a{
int b;
int c;
};

struct a *pa;

struct a cust[2];

int main(void)
{
    pa = cust;
   
   (*(pa + 0)).b = 1;
   
    printf("%d %d",(*(pa + 1)).b,cust[1].b);
    
    return 0;
};
