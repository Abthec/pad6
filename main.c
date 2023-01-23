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


uint8_t RawRxBuffer[2048];
char Buffer[16];


FT_STATUS ftStatus;
DWORD numDevs;
DWORD EventDWord;
DWORD TxBytes;
DWORD BytesWritten;
DWORD RxBytes;
DWORD BytesReceived;
DWORD devIndex = 0;

int main(void) {
    // ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
    // if (ftStatus == FT_OK) {
    //     printf("OK\n");
    // } else {
    //     printf("Not OK\n");
    // }
    // printf("%d", numDevs);

    ftStatus = FT_ListDevices((PVOID)devIndex, Buffer, FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
    if (FT_SUCCESS(ftStatus)) {
        printf("Success\n");
    } else {
        printf("Not Success\n");
    }
    printf("%lu", ftStatus);
}