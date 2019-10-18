#include <stdio.h>
enum oled_num {
        ZERO = 0,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NIGHT,
};

char num_data[][16] = {
{0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x08, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x20, 0x20, 0x10, 0x08, 0x04, 0x42, 0x7E, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x20, 0x18, 0x20, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x20, 0x30, 0x28, 0x24, 0x24, 0x22, 0x22, 0x7E, 0x20, 0x20, 0x78, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x02, 0x1A, 0x26, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x38, 0x24, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x7E, 0x22, 0x22, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00},
{0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x64, 0x58, 0x40, 0x40, 0x24, 0x1C, 0x00, 0x00},
};

unsigned char data_one[64] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x07, 0x60, 0x0C,
  0x30, 0x18, 0x18, 0x30, 0x18, 0x30, 0x18, 0x20, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60,
  0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x18, 0x20, 0x18, 0x30, 0x18, 0x30,
  0x30, 0x18, 0x60, 0x0C, 0xC0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
  };//'"0",0

unsigned int data_one1[32];


int readbit(num, bit) {

    return (num >> bit) & 1;

}

void data_trans(unsigned char data[64])
{
   int i;
   for(i = 0;i < 32; i++){
       data_one1[i] = (int)(data[i*2] | data[i*2+1]<<8);
   }
#if 0
   for(i = 0;i < 32; i++){
       printf("0x%x\t",data_one1[i]);
       if((i%5 ==0)&&(i!=0))
       printf("\n");
   }
#endif
}


unsigned char ToOrd(unsigned char ch)
{
	if(ch<32)
	{
		ch=95;
	}
	else if((ch>=32)&&(ch<=47)) //(32~47)空格~/
	{
		ch=(ch-32)+10+62;
	}
	else if((ch>=48)&&(ch<=57))//(48~57)0~9
	{
		ch=ch-48;
	}
	else if((ch>=58)&&(ch<=64))//(58~64):~@
	{
		ch=(ch-58)+10+62+16;
	}
	else if((ch>=65)&&(ch<=126))//(65~126)A~~
	{
		ch=(ch-65)+10;
	}
	else if(ch>126)
	{		
		ch=95;
	}

	return ch;
}


//0x18 0001 1000
#if 1
#define WHITE         0xffff
#define BLACK         0x0000
char temp_data[32*16*2];
char soc_data[32*16*2];
#else
int temp_buff[16*32];
static void display_num(enum oled_num num, int color, int width, int height)
{
        int row = 0, column = 0;
   
        for(row = 0; row < row; row++)
        {
             for(column = 0; column < column; column++)
             {
                if(readbit(data_one1[row], column - 1)){
                       temp_buff[row * width *column] = 1;
                }
                else{
                       temp_buff[row * width *column] = 0;
                }
             }
        }

        for(row = 0; i < row; row++)
        {
             for(column = 0; column < column; column++)
             {
                if(temp_buff[row * width  column]){
                        printf("*");
                }
                else{
                        printf(".");
                }
            }
            printf("\n");   
        }
}
#endif

#define MAX_SIZE 10  // 存储元素的最大个数
#define ERROE -1
#define bool int
#define True 1
#define Flase 0
// element_type 代表任意基本数据类型
#define element_type int

struct stack{
    element_type data[MAX_SIZE];
    int top;
};

// 入栈
void push(struct stack *s, element_type item)
{
    if (s->top < MAX_SIZE-1){
        s->data[(s->top)+1] = item;
        s->top++;
    }
}

// 出栈
element_type pop(struct stack *s)
{
    element_type n = NULL;
    if (s->top != -1){
        n = s->data[(s->top--)];
    }
    return n;
}

// 判断堆栈是否为空
bool is_empty(struct stack *s)
{
    bool flag = Flase;
    if (s->top == -1){
        flag = True;
    }
    
    return flag;
}

// 判断堆栈是否已满
bool is_full(struct stack *s, int maxsize)
{
    bool flag = Flase;
    if (s->top == maxsize-1){
        flag = True;
    }
    
    return flag;
}

#define Height        160
#define Width         80
#define ONE_FB_SIZE   160*80*2
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#define ALIGN_TO(x, n)  	(((x) + ((n) - 1)) & ~((n) - 1))

int pow101(int a,int y)
{
        int x = 1;
        if(y > 10)
        return -1;

        for(y; y > 0; y--)
        {
           x = x*10;
        }
        x = a / x % 10;
        return x;
} 


int test(int num,unsigned char len)
{
        int nlen = 0,temp = 0;
        for(nlen = 0; nlen < len; nlen++)
        {
	    temp = pow101(num ,len - nlen - 1);
            printf("%d \t",temp);
        }
}

#if 0
void LCD_ShowString(u8 *string, x,u16 y,u16 width,u16 height,u8 size)
{         
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
#endif
//void push(stack *s, element_type item)
#define BIT_REG_CTRL0_TRY_SNK           (0x01 << 5)
#define BIT_REG_CTRL0_RLE_DRP           (0x02 << 1)
#define WUSB3801_DRP_ACC                (BIT_REG_CTRL0_RLE_DRP)
#define WUSB3801_DRP_PREFER_SNK_ACC     (WUSB3801_DRP_ACC | BIT_REG_CTRL0_TRY_SNK)   

struct test {
    char *string;
    int  a;
};

struct test c = {
   "123",1
};
 
struct test b;
struct test *d = &c;
static int LCD_CalNumLens(int num)
{
       int sum = 0;
       while(num)
       {
           num /= 10;
           sum++;
       }
       return sum;
}

static int aw36432_get_bit(int val, int shift)
{
        return ((val >> shift) & 0x1);
}

static int aw36432_set_bit(int val, int shift)
{
        return (val | (0x1 << shift));
}

static int aw36432_clear_bit(int val, int shift)
{
        return (val & (~(0x1 << shift)));
}
int main(void)
{

        printf("get:0x%d set:0x%x clear:0x%x",aw36432_get_bit(0xFEFF,8),
                             aw36432_set_bit(0x0,8),aw36432_clear_bit(0xFFFF,8));     
        return 0;
}



























