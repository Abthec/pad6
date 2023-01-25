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
// #include "pad6bsl.h"

// BSLlowlevel lowLevel = {0, 0, 0, 0, 0, 0, MODE_BSL, 0, 0, 0x70, false};
// PAD6BootStrapLoader bslobj = {lowLevel, 0, 1, 0, 0, 0, NULL, NULL, MAXDATA, NULL, 1};

FT_DEVICE_LIST_INFO_NODE *devInfo;
bool FTDI_open_flag;

char RawRxBuffer[256];
char RawTXBuffer[256];
char Buffer[16];

FT_STATUS ftStatus;
FT_HANDLE ftHandle;
FT_DEVICE ftDevice;
FTDCB ftDCB;
DWORD numDevs;
DWORD EventDWord;
DWORD TxBytes;
DWORD BytesWritten;
DWORD RxBytes;
DWORD BytesReceived;
DWORD devIndex = 0;

char serialNumber[16];
char description[64];
LPVOID pvDummy = NULL;

FT_HANDLE deviceHandle;
DWORD deviceFlags;
DWORD deviceType;
DWORD deviceID;
DWORD deviceLocalID;
char deviceSerialNumber[16];
char deviceDescription[64];

char description[] = "Prattley Auto Drafter 6";

int CBUS = 0x70;

int main(void) {

    BOOL commState;
    uint8_t FTWrite_Check;
    int char_tx_count = 0;
    unsigned char data = 0x80;
    int tx_data_size = sizeof(data);

    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    if (ftStatus == FT_OK) {
        printf("NumDevs = %lu\n", numDevs);
        if (numDevs > 1) {
            printf("Multiple devices connected not yet implemented please remove and retry with only 1 device\n");
        } else if (numDevs == 1) {
            ftStatus = FT_GetDeviceInfoDetail(0, 
                                              &deviceFlags,
                                              &deviceType,
                                              &deviceID,
                                              &deviceLocalID,
                                              &deviceSerialNumber,
                                              &deviceDescription,
                                              &deviceHandle);
            printf("Index = %lu, Flags = 0x%lu, Type = 0x%lu, ID = 0x%lu, LocalID = 0x%lu, Serial Number = %s, Description = %s\n", 
                   0, deviceFlags, deviceType, deviceID, deviceLocalID, deviceSerialNumber, deviceDescription);
        }
    } else {
        printf("ERROR creating device info list\n");
    }

    ftStatus = FT_OpenEx(deviceDescription, FT_OPEN_BY_DESCRIPTION, &ftHandle);
    if (ftStatus == FT_OK) {
        printf("Open Success\n");
        printf("Device Handle = 0x%p\n", ftHandle);
    } else {
        printf("Open Failed\n");
    }

    commState = FT_W32_SetupComm(ftHandle, 4096, 4096);
    if (commState) {
        printf("CommSetup Good\n");
    } else {
        printf("CommSetup Bad\n");
    }

    ftStatus = FT_SetBitMode(ftHandle, 0x70, 0x20);
    ftStatus = FT_SetBitMode(ftHandle, 0x74, 0x20);
    // // FT_SetRts(ftHandle);
    // // FT_ClrDtr(ftHandle);
    FT_W32_PurgeComm(ftHandle, FT_PURGE_RX)
    FT_W32_PurgeComm(ftHandle, FT_PURGE_TX)

    FT_SetTimeouts(ftHandle,5000,0);

    // // commState = FT_W32_GetCommState(ftHandle, &ftDCB);
    // // if (commState == true) {
    // //     printf("CommState = true")
    // // } else {
    // //     printf("CommState = false")
    // // }

    FTWrite_Check = FT_Write(ftHandle, &data, sizeof(data), &BytesWritten);

    printf("Bytes Written = %lu\n", BytesWritten);

    ftStatus = FT_GetStatus(ftHandle, &RxBytes, &TxBytes, &EventDWord);

    printf("Bytes in TxBuffer = %lu\n", TxBytes);
    printf("Bytes in RxBuffer = %lu\n", RxBytes);

    // // RawRxBuffer[RxBytes+1] = '\0';    

    // // ftStatus = FT_Read(ftHandle, RawRxBuffer, 1, &BytesReceived);
    // // if (ftStatus == FT_OK) {
    // //     if (BytesReceived > 0) {
    // //         printf("%s\n", RawRxBuffer);
    // //     }        
    // //     printf("Bytes Recieved = %lu\n", BytesReceived);
    // // } else {
    // //     printf("Unsuccessful RX\n");
    // //     printf("%lu\n", ftStatus);
    // // }
    
    ftStatus = FT_Close(ftHandle);
    if (ftStatus == FT_OK) {
        printf("Closed\n");
    } else {
        printf("Unsuccessful Close\n");
        printf("%lu\n", ftStatus);
    }
}