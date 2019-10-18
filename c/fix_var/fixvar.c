#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//#define FIXVAR_V2
#define FIXVAR_V3


#if defined(FIXVAR_V1)

#define _INTSIZEOF(n)   ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
void arg_test(int i, ...)
{
    int j=0;
    va_list arg_ptr;
    va_start(arg_ptr, i);
    printf("&i = %p\n", &i);                //打印参数i在堆栈中的地址
    printf("arg_ptr = %p\n", arg_ptr);      //打印va_start之后arg_ptr地址
                                            /*这时arg_ptr应该比参数i的地址高sizeof(int)个字节,即指向下一个参数的地址*/
    j=*((int *)arg_ptr);                    //对指针解引用
    printf("%d %d\n", i, j);
    j=va_arg(arg_ptr, int);
    printf("arg_ptr = %p\n", arg_ptr);      //打印va_arg后arg_ptr的地址
                                            /*这时arg_ptr应该比参数i的地址高sizeof(int)个字节,即指向下一个参数的地址,如果已经是最后一个参数，arg_ptr会为NULL*/ 
    va_end(arg_ptr);
    printf("%d %d\n", i, j);
}


int main(int argc,char *argv[])
{
    int int_size = _INTSIZEOF(int);
    printf("int_size=%d\n", int_size);
    arg_test(0, 4,4,5,6,7,8,9);
    return 0;
}
#elif defined(FIXVAR_V2)
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

void my_printf(const char* fmt, ... )
{
      va_list ap;
      va_start(ap,fmt); /* 用最后一个具有参数的类型的参数去初始化ap */
      for (;*fmt;++fmt)
      {
          /* 如果不是控制字符 */
         if (*fmt!='%')
         {
             putchar(*fmt); /* 直接输出 */
             continue;
         }
         /* 如果是控制字符，查看下一字符 */
         ++fmt;
         if ('\0'==*fmt) /* 如果是结束符 */
         {
             assert(0);  /* 这是一个错误 */
             break;
         }
         switch (*fmt)
         {
         case '%': /* 连续2个'%'输出1个'%' */
             putchar('%');
             break;
         case 'd': /* 按照int输出 */
             {
                 /* 下一个参数是int，取出 */
                 int i = va_arg(ap,int);
                 printf("%d",i);
             }
             break;
         case 'c': /* 按照字符输出 */
             {
                 /** 但是，下一个参数是char吗*/
                 /*  可以这样取出吗？ */
                 char c = va_arg(ap,char);
                 printf("%c",c);
             }
             break;
         }
     }
     va_end(ap);  /* 释放ap—— 必须！ 见相关链接*/
 }

int main(int argc,char *argv[])
{
    my_printf("%d %d",1,1);
    return 0;
}
#elif defined(FIXVAR_V3)
void cust_get_params(const char* fmt, ... )
{
      va_list ap;
      va_start(ap,fmt); 
      for (;*fmt;++fmt)
      {
         if (*fmt!='%')
         {
             continue;  //do noting
         }

         ++fmt;
         if ('\0'==*fmt) /* 如果是结束符 */
         {
             assert(0);  /* 这是一个错误 */
             break;
         }

         switch (*fmt)
         {
         case '%': 
             //putchar('%');   //we don`t need handle this case
             break;
         case 'd': /* 按照int输出 */
             {
                 int i = va_arg(ap,int);
                 printf("%d",i);
             }
             break;
         case 'c': /* 按照字符输出 */
             {
                 char c = va_arg(ap,char);
                 printf("%c",c);
             }
             break;
         }
     }
     va_end(ap);  
 }

int main(int argc,char *argv[])
{
    cust_get_params("%d %d",1,2,3,4);
    return 0;
}
#endif

