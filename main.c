#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h> 
#include <windows.h>
#include <windef.h>
#include <winnt.h>
#include <winbase.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "ftd2xx.h"

FT_DEVICE_LIST_INFO_NODE *devInfo;
bool FTDI_open_flag;


char RawRxBuffer[256];
char RawTXBuffer[256];
char Buffer[16];


FT_STATUS ftStatus;
FT_HANDLE ftHandle;
FT_DEVICE ftDevice;
DWORD numDevs;
DWORD EventDWord;
DWORD TxBytes;
DWORD BytesWritten;
DWORD RxBytes;
DWORD BytesReceived;
DWORD devIndex = 0;

DWORD deviceID;
char serialNumber[16];
char description[64];
LPVOID pvDummy = NULL;

int CBUS = 0x70;

int main(void) {

    uint8_t FTWrite_Check;
    int char_tx_count = 0;
    char data = 'K';
    char buf[1];
    memcpy(buf, &data, sizeof(data));
    int tx_data_size = sizeof(data);

    ftStatus = FT_Open(0, &ftHandle);
    if (ftStatus == FT_OK) {
        printf("Open Success\n");
        printf("%p\n", ftHandle);
    } else {
        printf("Open Failed\n");
    }

    ftStatus = FT_SetBitMode(ftHandle, 0x70, 0x20);
    ftStatus = FT_SetBitMode(ftHandle, 0x74, 0x20);
    // FT_SetRts(ftHandle);
    // FT_ClrDtr(ftHandle);
    FT_W32_PurgeComm(ftHandle, PURGE_RXCLEAR|PURGE_RXABORT);
    FT_W32_PurgeComm(ftHandle, PURGE_TXCLEAR|PURGE_TXABORT);

    FT_SetTimeouts(ftHandle,5000,0);

    // while(char_tx_count != tx_data_size) {
    //     FTWrite_Check = FT_Write(ftHandle, &data[char_tx_count], (DWORD)sizeof(data[char_tx_count]), &BytesWritten);
    //     if (FTWrite_Check != FT_OK) {
    //         printf("Bad Write\n");
    //         printf("%lu\n", FTWrite_Check);
    //     }
    //     printf("%C\n", data[char_tx_count]);
    //     char_tx_count++;
    // }

    // printf("CharTxCount = %lu\n", char_tx_count);

    FTWrite_Check = FT_Write(ftHandle, buf, (DWORD)sizeof(data), &BytesWritten);

    printf("Bytes Written = %lu\n", BytesWritten);

    ftStatus = FT_GetStatus(ftHandle, &RxBytes, &TxBytes, &EventDWord);

    printf("Bytes in TxBuffer = %lu\n", TxBytes);
    printf("Bytes in RxBuffer = %lu\n", RxBytes);

    RawRxBuffer[RxBytes+1] = '\0';    

    ftStatus = FT_Read(ftHandle, RawRxBuffer, 1, &BytesReceived);
    if (ftStatus == FT_OK) {
        if (BytesReceived > 0) {
            printf("%s\n", RawRxBuffer);
        }        
        printf("Bytes Recieved = %lu\n", BytesReceived);
    } else {
        printf("Unsuccessful RX\n");
        printf("%lu\n", ftStatus);
    }
    

    ftStatus = FT_Close(ftHandle);
    if (ftStatus == FT_OK) {
        printf("Closed\n");
    } else {
        printf("Unsuccessful Close\n");
        printf("%lu\n", ftStatus);
    }
}