    #include <stdio.h>

    int foo(void)
    {
    	int *i = NULL;
    	*i = 0;
    }

    int main(void)
    {
    	foo();
    	return 0;
    }
