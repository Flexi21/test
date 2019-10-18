#include <stdio.h>

#include<stdio.h> 

    int file_size(char* filename)  
    {  
        FILE *fp=fopen(filename,"r");  
        if(!fp) 
        return -1;  
        
        fseek(fp,0L,SEEK_END);  
        int size=ftell(fp);  
        fclose(fp);  
          
        return size;  
    }  

#define MIC_NUM   6


void main()
{ 
    int i = 0;
    char tmp[12];
    FILE *fpMic1;
    FILE *fpMic2;
    FILE *fpMic3;
    FILE *fpMic4;
    FILE *fpMic5;
    FILE *fpMic6;

    //char *filename = "i2s_channel.pcm";
    char *filename = "i2s.pcm";

    FILE *fw=fopen(filename,"r");

    fpMic1 = fopen("1.pcm", "wb"); 
    fpMic2 = fopen("2.pcm", "wb"); 
    fpMic3 = fopen("3.pcm", "wb"); 
    fpMic4 = fopen("4.pcm", "wb"); 
    fpMic5 = fopen("5.pcm", "wb"); 
    fpMic6 = fopen("6.pcm", "wb"); 
 

    for(i = 0; i < file_size(filename)/(MIC_NUM*2); i++)
    {
        #if (MIC_NUM > 5)
        fseek(fw, i*12L, 0);
        #else
        fseek(fw, i*8L, 0);
        #endif
        fread(&tmp[0],1,(MIC_NUM*2),fw);
        

        if (fwrite(tmp, 1, 2, fpMic1) != 2) {
            printf("zwl");    
        }
        if (fwrite(&tmp[0]+2, 1, 2, fpMic2) != 2) {
            printf("zwl");    
        }
        if (fwrite(&tmp[0]+4, 1, 2, fpMic3) != 2) {
            printf("zwl");    
        }
        if (fwrite(&tmp[0]+6, 1, 2, fpMic4) != 2) {
            printf("zwl");    
        }
        #if (MIC_NUM > 5)
        if (fwrite(&tmp[0]+8, 1, 2, fpMic5) != 2) {
            printf("zwl");    
        }
        if (fwrite(&tmp[0]+10, 1, 2, fpMic6) != 2) {
            printf("zwl");    
        }
        #endif
    }
    fclose(fw); 
    fclose(fpMic1); 
    fclose(fpMic2); 
    fclose(fpMic3); 
    fclose(fpMic4); 
    #if (MIC_NUM > 5)    
    fclose(fpMic5); 
    fclose(fpMic6); 
    #endif
}

