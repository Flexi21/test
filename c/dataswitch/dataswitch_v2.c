#include <stdio.h>
#include<stdlib.h>
#include<string.h>

unsigned char asc_1616[10 * 16 * 16 / 8] = 
{
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,
0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,};



static int LCD_StringDataCollect(u8 *_string, u8 *data, int ccolor,
                                         int bcolor, int wsize ,int hsize)
{
        u8 *pdata = data;

        while( *_string != '\0')
        {  
                if((*_string < ' ')||(*_string > '~'))
                {
                        pr_err(TAG "fun:%s the string is not exist at ascll ' ' to '~'! \n",__func__);
                        return -1;
                }

                LCD_OneDataCollect(*_string, pdata, ccolor, bcolor, wsize, hsize);
                _string++;     
                pdata += 2*wsize*hsize; 
        }
        return 0;
}


static void LCD_ONeColumeDataSwitch(char *data, int wsize, int hsize)
{
        int i = 0;
        int nchar,char_size,datasize;
        char *tmp = malloc((sizeof(asc_1616)/sizeof(unsigned char)));
        char *ptmp = data;
        memcpy(tmp, data, (sizeof(asc_1616)/sizeof(unsigned char)));
       // printf("%d",sizeof(unsigned char));
     //   printf("\n");
        #if 0
        for(i= 0;i<320;i++)
        {
            printf("0x%0.2X,",*(tmp+i));
            if(((i+1)%32 == 0)&&(i!=0))
            printf("\n");
        }
        #endif
        for(char_size = 0;char_size < hsize; char_size++)//16
        {
            for(nchar = 0; nchar < 160/wsize; nchar++)//10
            {
                for(datasize = 0; datasize < wsize/8; datasize++)//2         ->=2*10*16 = 320
                {
                    *ptmp = *(tmp + (nchar*wsize + char_size)*wsize/8 + datasize);// char_size*wsize/8
                     ptmp++;
                }
            }
        }    
}

int main(void)
{
    int i,j = 0;
    int tmp = 0;
    LCD_ONeColumeDataSwitch(asc_1616,16,16);
    for(i= 0;i<320;i++)
    {
            printf("0x%0.2X,",asc_1616[i]);
            if(((i+1)%20 == 0)&&(i!=0))
            printf("\n");
    }
    #if 0
    for(j = 0;j < 32;j++)
    {
        for(i = 0;i < 10;i++)
        {
           printf("0x%0.2x,",asc_1616[tmp]);
           tmp++;
           if((tmp+1)%10 == 0)
           printf("\n");
        }
    }
    #endif
    return 0;
}
