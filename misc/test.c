#define X
#ifdef  XXXXX
#include <string.h>
#include <stdio.h>
char string[] ="A, string, of, tokens, and, some, more, tokens";
//char seps[]   =" ,\t\n";
char seps[]   =",";
char *token;
int main( void )
{
   token = strtok( string, seps );

   printf( "%s\nzwl :\n", string );

while( token != NULL )
   {

      printf( " %s\n", token );
      printf( " ZWL\n");// token );

      token = strtok( NULL, seps );

   }
return 0;
}
#elif defined(X)
#include <stdlib.h>
#include <stdio.h>
 
int main()
{
    char a[] = "-100";
    char b[] = "123";
    int c;
    c = atoi(a) + atoi(b);
    printf("c=%d\n", c);
    return 0;
}
#elif defined(XX)
#include <stdio.h>
#include <string.h>
//#include <alloc.h>
int main(void)
{       
        char *dup_str,*string = "abcde";
        dup_str =strdup(string);
        printf("%s\n", dup_str);
        //free(dup_str);
         return 0;
}


#else
#include <string.h>
#include <stdio.h>
char string[] ="A, string, of, tokens, and, some, more, tokens";
char seps[]   =",";
char *token, *s;
int main( void )
{
   //printf( "%s\n\nTokens:\n", string );
/* Establish string and get the first token: */
   s=string;
   token = strsep( &s, seps );
   printf( "%s\n\nzwl:\n", token );
while( token != NULL )
   {
/* While there are tokens in "string" */
      printf( " %s\n", token );
/* Get next token: */
      token = strsep( &s, seps );
   }
return 0;
}
#endif
