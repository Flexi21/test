#include <stdio.h>


int test_array(char ary[255],int count)
{
   int i = 0;
   for(i = 0; i < count; i++)
   {
     ary[i] = i;
      //printf("zwl ");
   }


}

int main(void)
{
  int i = 0;
  char test[255] = {0}, count = 10;
  test_array(test,count);
   for(i = 0; i < count; i++)
   {
     printf("%d\t",test[i]);
   }
  
  return 0;
}
