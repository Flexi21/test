#include <stdio.h>

static unsigned char bq25601_bit_shift(unsigned char reg_bit)
{
        unsigned char shift = 0;
        for (shift = 0; shift < 8; shift++)
        {
            if ((reg_bit >> shift) && 0x01)
                return shift;
        }

        return 0;
}

int main(void)
{
        printf("0x%x \n",bq25601_bit_shift(0x60));
}
