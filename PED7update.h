#ifndef PED7UPDATE_H
#define PED7UPDATE_H

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
#include "serialftd.h"
#include "pad6bsl.h"

#define DEFUALT_ADDRESS 0xFFA0

#define CONNECTS_INFOMEM_D_ADDRESS 0x1000
#define CONNECTS_INFOMEM_C_ADDRESS 0x1040
#define CONNECTS_INFOMEM_B_ADDRESS 0x1080
#define CONNECTS_INFOMEM_A_ADDRESS 0x10C0


typedef struct {
    PAD6BootStrapLoader bslobj;
    SerialFtd serialftd;
    unsigned char* password;
    unsigned char* infomem;
    unsigned char* infoD;
    unsigned char* pcbType;
    int pcbVersion;
    int swVersion;
    int radioID;
    int debug;
} PrattleyDevice;

void initPrattleyDevice(void);

void close(void);

void softwareConnection(void);

void hardwareBSLConnection(void);

void repairInfomem(unsigned char* missingSerialNo);

void getInfo(void);

void openBlank(void);

void reboot(void);

void flash(void);

void access_PED7(void);

void USBDevices(void);

void serialDevices(void);

void displayInfomem(unsigned char* serialNum);

void autoUpdate(unsigned char* serialNum);

void autoUpdates(unsigned char* serialNum);

void mainWindow(void);

#endif /* PED7UPDATE_H */