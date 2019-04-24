#include <stdio.h>
#include <string.h>
#include "hextobin.h"
#include "Cansendrecv.h"

void hextobin(char hexa[100], int count){
    char hex[100], bin[65] = "";
    //temp[20][20];
    int i = 0;
    char Hexcopy = hexa[count];
    size_t np = sizeof(hexa);
    char *tp[24][40] ={"",""};
	char *tpcpy[24][40]={"",""};
	memset(tp, 0, sizeof(tp)/sizeof(tp[0]));
    /* Input hexadecimal number from user */
   // printf("Enter any hexadecimal number: ");
    //gets(hex);

    /* Extract first digit and find binary of each hex digit */
    for(i=0; i< count; i++)
    {
        switch(hexa[i])
        {
            case '0':
                strcat(bin, "0000");
                sprintf(tp[i],"%s","0000");
                break;
            case '1':
                strcat(bin, "0001");
                sprintf(tp[i],"%s","0001");
                break;
            case '2':
                strcat(bin, "0010");
                sprintf(tp[i],"%s","0010");
                break;
            case '3':
                strcat(bin, "0011");
                sprintf(tp[i],"%s","0011");
                break;
            case '4':
                strcat(bin, "0100");
                sprintf(tp[i],"%s","0100");
                break;
            case '5':
                strcat(bin, "0101");
                sprintf(tp[i],"%s","0101");
                break;
            case '6':
                strcat(bin, "0110");
                sprintf(tp[i],"%s","0110");
                break;
            case '7':
                strcat(bin, "0111");
                sprintf(tp[i],"%s","0111");
                break;
            case '8':
                strcat(bin, "1000");
                sprintf(tp[i],"%s","1000");
                break;
            case '9':
                strcat(bin, "1001");
                sprintf(tp[i],"%s","1001");
                break;
            case 'a':
            case 'A':
                strcat(bin, "1010");
                sprintf(tp[i],"%s","1010");
                break;
            case 'b':
            case 'B':
                strcat(bin, "1011");
                sprintf(tp[i],"%s", "1011");
                break;
            case 'c':
            case 'C':
                strcat(bin, "1100");
                sprintf(tp[i],"%s","1100");
                break;
            case 'd':
            case 'D':
                strcat(bin, "1101");
                sprintf(tp[i],"%s", "1101");
                break;
            case 'e':
            case 'E':
                strcat(bin, "1110");
                sprintf(tp[i],"%s","1110");
                break;
            case 'f':
            case 'F':
                strcat(bin, "1111");
                sprintf(tp[i],"%s","1111");
                break;
            default:
                printf("Invalid hexadecimal input.");
        }
        printf("TP in switch [%d] = %s \n", i ,&tp[i]);
    }

    printf("Hexademial number = %s\n", hexa);
    printf("Binary number = %s \n", bin);
    //printf("bin %s", &bin[5]);
     // size_t n = sizeof(tp)/sizeof(tp[0]);
   // size_t m =sizeof(tpcpy);
	memset(tpcpy, 0, sizeof(tpcpy)/sizeof(tpcpy[0]));
	
	
	int inc2 =0;
	int j=0;
    for(j=0; j < count ; j++)
    {
		
		// printf("TP Value [%d] = %s \n", j ,&tp[j]);
			
			//strcat(tpcpy[i],tp[inc2]);
			
		    //strcat(tpcpy[i],tp[inc2+1]);
		//sprintf(tpcpy[i],"%s",strcat(tp[inc2], tp[inc2+1]));
		  //strcpy(tpcpy[i], strcat(tp[inc2], tp[inc2+1]));
			strcpy(tpcpy[j], strcat(tp[inc2], tp[inc2+1]));
			inc2+=2;
		
	
		printf("str cpy [%d] value is :%s \n", j, &tpcpy[j]);
	}
	
	
	inc2 = 0;
	printf("str cpy value is **************** :%s \n",&tpcpy[1]);
	//call can send function
	can_send_hex(tpcpy[][],j/2);
	
	//clear the memory
	 memset(tpcpy, 0, sizeof(tpcpy)/sizeof(tpcpy[0]));
	 memset(tp, 0, sizeof(tp)/sizeof(tp[0]));
   // return temp;
}
void can_send_hex(char  *canhexdata[24][40], int size){
	//memset(canhexdata, 0, size);
	
	for (int p=0;p<size; p++){
		printf(" Size of can is: %d  ready to send data is: %s \n ",p, canhexdata[p]);
		
	}
	can_send(1, 123,canhexdata);
	
}
