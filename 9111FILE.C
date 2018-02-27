#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "dask.h"

U16 channel=3;  //4 channels
U16 range=AD_B_5_V;
char *file_name="9111d";
BOOLEAN concat=FALSE;
U32 read_count=1024;
F64 sample_rate=10000;

main()
{
    I16 card, err, card_num;

    printf("This program inputs %d data from CH-0 to CH-%d of PCI-9111 in %d Hz, and\nstore data to file '%s.dat'.\nPlease press any key to start the operation.\n", read_count, channel, (int)sample_rate, file_name);
	printf("Please input a card number: ");
    scanf(" %d", &card_num);
    //getch();
    if ((card=Register_Card (PCI_9111DG, card_num)) <0 ) {
        printf("Register_Card error=%d", card);
        exit(1);
    }
    err = AI_9111_Config(card, TRIG_INT_PACER, 0, 1024);
    if (err!=0) {
       printf("AI_9111_Config error=%d", err);
       exit(1);
    }
    err = AI_AsyncDblBufferMode(card, 0);
    if (err!=0) {
       printf("AI_DblBufferMode error=%d", err);
       exit(1);
    }
    err = AI_ContScanChannelsToFile(card, channel, range, file_name, read_count, sample_rate, SYNCH_OP);
    if (err!=0) {
       printf("AI_ContReadChannel error=%d", err);
       exit(1);
    }
    Release_Card(card);
    printf("\n\nThe input data is already stored in file '%s.dat'.\n", file_name);
    printf("\nPress ENTER to exit the program. "); getch();
}
