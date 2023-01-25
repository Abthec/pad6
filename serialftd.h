#ifndef SERIALFTD_H
#define SERIALFTD_H

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

#define DEFAULT_BUF_SIZE 4096
#define WIN32_EV_ERR 0x0080

typdef struct {
    int index;
    char* description;
    int usb;
} Port;

typedef struct {
    FT_HANDLE ftHandle;
    int baudrate;
    FT_Status ftStatus;
    int byteSize;
    int parity;
    int stopBits;
    
    bool isOpen;
} SerialFTD;

void open_ft_device_id(int index);

void open(char* description);

void reconfigure_port(void);

void close(void);

char* read(int size=1);

int write(char* data);

void flushInput(void);

void flushOutput(void);

int getNumDevices(void);

#endif /* SERIALFTD_H */