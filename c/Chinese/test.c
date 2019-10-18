#include<stdio.h>
#include<string.h>

#define UNICODE_TEST
#if defined(UNICODE_TEST)
#define GB2312_CH_H_BEGIN     0x81
#define GB2312_CH_L_BEGIN     0x40
#define ONE_PAGE_GB2312       192
static int LCD_UnicodeSwitch(int unicode)
{
        unsigned char h_tmp,m_tmp,l_tmp,shift = 0;
        int re_unicode = 0;
        h_tmp = (unicode >> 8) & 0xFF;
        m_tmp = ((unicode >> 4) & 0x0F) - (GB2312_CH_L_BEGIN >> 4);
        l_tmp = unicode & 0x0F;

        unicode = (h_tmp - GB2312_CH_H_BEGIN)*ONE_PAGE_GB2312
                                      + m_tmp *16 + l_tmp + shift;

        printf("0x%.2X  0x%.2X  0x%.2X \n",h_tmp,m_tmp,l_tmp);
        printf("unicode:%d \n",unicode);
}
#endif

int test[2] = {1,2};
int test1[2];
int main()
{
    #if defined(UNICODE_TEST)
    LCD_UnicodeSwitch(0xB0A1);
    #endif
}
