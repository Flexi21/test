#include <stdio.h>
#include <string.h>
static int cust_index = 0;
struct flashinfo {
        char name[20];
	unsigned char  raw_cid[4];
};

static struct flashinfo cust_finfo[] = {
        { //H9TQ27ADFTMCUR_KUM
            "Hynix_16G+2G",
            {
              0x90014A68,
              0x42386150,
              0x3E,
              0x0
            }
        },
        { //H9TQ27ADFTMCUR_KUM
            "Hynix_16G+2G",
            {
              0x90014A68,
              0x42386150,
              0x3E,
              0x0
            }
        },
        {
        },
        { //H9TQ27ADFTMCUR_KUM
            "Hynix_16G+2G",
            {
              0x90014A68,
              0x42386150,
              0x3E,
              0x0
            }
        },
};

static char name[20];
int main(void)
{
   while(cust_finfo[cust_index].name[0] != '\0')
   {
       printf("cust:%d \n",cust_index);
       cust_index++;
   }

   if (strlen(name) ==0) {
       printf("zwl 1\n");
   }

    if (name[0] == '\0') {
       printf("zwl 2\n");
   }
}
