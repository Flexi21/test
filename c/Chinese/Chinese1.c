#include<stdio.h>
#include<string.h>


unsigned char GB[1][32]= {
{0x00,0x00,0x0E,0xFC,0xEA,0x08,0xAA,0x08,0xAA,0xE8,0xAA,0xA8,0xAC,0xA8,0xAA,0xA8,
0xAA,0xA8,0xAA,0xA8,0xEA,0xE8,0xAA,0xA8,0x0C,0x08,0x08,0x08,0x08,0x28,0x08,0x10}//°¡0
};
int achinese[100] = {
#if 0
                     0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0
#endif
                     #if 0
                     0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0xB0,0xA0,
                     #else
                     0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,
                     #endif
                     #if 1
                     0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,
                     #else
                     0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,
                     #endif
                     0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,
                     0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,0xB0,0xA0,
                     0xB0,0xA1,0xB0,0xA1,0xB0,0xA1,0xB0,0xA1
                    };


struct recode_temp {
    int char_count;
    int recode_asc[100];
    int all_column;
    int cloumn_char[5];
    int lens;
};
/*
asdakjs

*/
int fchinese[100];
static int rchinese[100];
#if 0
#define ONE_COL_CHAR   20
static reconstruct_achinese(int *p, struct recode_temp recode)
{
        int i,char_count = 0;
        int *fp = fchinese;
        for(column = 0; column < recode.all_column; cloumn++){

            for(i = 0; i < 20; i++)
            {
                if ((*(p + i) < 95)&&(*(p + i) > 0))
                {
                    char_count++;
                } 
            }
    
            printf("\n char_count: %d , num: %d\n",char_count,char_count % 2);
    
            memcpy((fp + ONE_COL_CHAR*column - 1)), p, ONE_COL_CHAR - 1)
            if((char_count % 2) != 0){
                *(fp + column*ONE_COL_CHAR - 1) = *(p + column*ONE_COL_CHAR - 1)
            } else {
                *(fp + column*ONE_COL_CHAR - 1) = 0;
            }
        }
}
#endif

//获取每行要显示的数据存入buff
//每行的buff数量一定,如果行存在的ascall为奇数，则显示的数据量为19 + 一个填充数据
//如果为偶数，则显示数据为20个，两种显示方式都要计算重构后没一行的停止偏移地址。
/*
    1.前十个数据不存在ascall码，则显示数据偏移为10
    2.前十个显示数据有一个，则显示数据偏移 --> 1.第十一个数据不是ascall 则显示数据偏移为10，第十一个显示数据为ascall 则11    ：：：：检测范围等于（10 + n）
*/
#define Width 160
//int column[5];
#define ONE_ROW_CHAR 20
#define ONE_ROW_DATA 320
unsigned char RawData[ONE_ROW_DATA];
typedef unsigned char u8;
#if 1
#define GB2312_CH_H_BEGIN     0xB0
#define GB2312_CH_L_BEGIN     0x04
#define ONE_PAGE_GB2312       94
static int LCD_UnicodeSwitch(int unicode)
{
        unsigned char h_tmp,m_tmp,l_tmp,shift = 0;
        int re_unicode = 0;
        h_tmp = (unicode >> 8) & 0xFF;
        m_tmp = ((unicode >> 4) & 0x0F) - (GB2312_CH_L_BEGIN >> 4);
        l_tmp = unicode & 0x0F;

        unicode = (h_tmp - GB2312_CH_H_BEGIN)*ONE_PAGE_GB2312 
                                      + m_tmp *16 + l_tmp - 1 + shift;

       // printf("0x%.2X  0x%.2X  0x%.2X \n",h_tmp,m_tmp,l_tmp);
        printf("0x%.4X \n",unicode);
}

static void LCD_GetByteRawData(u8 num, u8 *data, int wsize ,int hsize,int row)
{
         unsigned char temp = 0;

         temp = GB[num][row];

          *data = temp;

         // printf("ff :0x%.2X \t", temp);  
}

static int recondata(int *pchinese, int row)
{
        int i = 0,j = 0, tmp = 0, shift = 0,tmp_shift = 0;
        unsigned char *ptmp = RawData;
        static count1 = 0;

        memset(RawData,0x00,ONE_ROW_DATA); 
//
       for(j = 0; j < 16; j++){                 
           tmp = 0;
           for(i = 0; i < ONE_ROW_CHAR; i++){

               if(*(pchinese +i) == 0){

                    while(tmp < 20)
                    {
                       tmp += 1;
                       printf("xxxxi:%d %d\n",i,tmp);                       
                       *(ptmp + j*20 + tmp - 1) = 0x00;
                    }
                    shift = i; 
                    printf("XXXXX ADD\n");
                    break;
               }

               if(((*(pchinese + i)) >= ' ')&&((*(pchinese + i)) < '~')){  
                   tmp += 1;
                   if(tmp <= ONE_ROW_CHAR){
                       LCD_GetByteRawData(0, (ptmp + j*20 + tmp - 1), 8, 16, j*2);
                   }
               } else {
                   tmp += 2;
                   if(tmp <= ONE_ROW_CHAR){
                       LCD_GetByteRawData(0, (ptmp + j*20 + tmp - 2), 16, 16, j*2);
                       LCD_GetByteRawData(0, (ptmp + j*20 + tmp - 1), 16, 16, j*2+1);
                   }
               }

            if (tmp > ONE_ROW_CHAR){
               shift = i; 
               break;
            }
            else if(tmp == ONE_ROW_CHAR){
               shift = i + 1; 
               break;
            } else {
              
            }
         }
      }
               count1++;


       printf("\n");   
       for(i=0 ; i< 16*20; i++){
            printf("ee :0x%.2X \t", *(ptmp+i));   
       }
      return shift;
}

//just need know how many data bytes
static int LCD_GetOneRowCenPos(int *pchinese)
{
        int i = 0,size = 0,show_pos = 0;
        for(i = 0; i < 20; i++){
 
            if((*(pchinese + i)) == 0){ //for the last row, we could not case weather the last char/chinese was attached
                break;
            }
            size += 8;
        }
        show_pos = (Width - size) / 2;
        printf("FUNC:%s onerowsize:%d pos:%d",__func__, size, show_pos);
        return show_pos;
}

#else
int recondata(int chinese[100] )
{
    int i = 0, tmp = 0, shift = 0;

    for(i = 0; i < 20; i++){

            if(chinese[i] == 0){ //for the last row, we could not case weather the last char/chinese was attached
                return i;
            }

        if((chinese[i] >= 0)&&(chinese[i] < 95)){
             tmp += 8;
              //printf("\n 0x%.4X",chinese[i]);
        } else {
             tmp += 16;
        }
        if (tmp > Width){
           shift = i; 
         //  printf("\n tmp > Width:%d %d\n",tmp,shift);
           break;
        }
       else if(tmp == Width){
           shift = i + 1; 
          // printf(" tmp == Width:%d %d\n",tmp,shift);
           break;
        }
    }
    return shift;    
}
#endif

int main()
{
        int i,lens;
        int char_count = 0;
        int shift = 0,old_shift = 0;
        int old_char = 1;
        int count = 0;
      

        for (lens = 0; lens < 100; lens++)
        {
#if 1
            if(!old_char){
               old_char = 1;
               continue;
            }        
#endif
 
            if (achinese[lens] != 0){
                     if((achinese[lens] >= 0)&&(achinese[lens] < 95)){
                         char_count++;
                         old_char = 1;
                         rchinese[count] = achinese[lens];
                     }
                     else {  
                         old_char = 0;
                         rchinese[count] = ((achinese[lens] << 8) | (achinese[lens+1] & 0xFF));
                     }
                     count++;
            } else {
                     //recode.lens = lens;
                     //printf("the show Chinese lens is :%d", recode.lens);
                     break;
            }
        }

        #if 1
        for(i = 0; i< 100; i++)
        {
            printf("re :0x%.4X \t", rchinese[i]);   
        }
        printf("re :%d \t", count);   
        #endif

        for(i = 0; i < 5; i++)
        {
            shift = recondata((rchinese + old_shift),i);
            old_shift += shift;
            printf("shift show %d old_shift:%d\n",shift,old_shift);

            if (old_shift == count){
                break;
            }
        }


        LCD_GetOneRowCenPos(achinese);

  //      if((old_shift - count) != 0)
        

}
