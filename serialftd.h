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

enum PARITY {
    NO_PARITY,
    ODD_PARITY,
    EVEN_PARITY,
    MARK_PARITY,
    SPACE_PARITY
};

enum STOPBITS {
    STOPBITS_NONE,
    STOPBITS_ONE,
    STOPBITS_TWO
};

typedef struct {
    int index;
    char* description;
} Port;

typedef struct {
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;
    Port port;
    unsigned int baudrate;
    unsigned int byteSize;
    unsigned int parity;
    unsigned int stopBits;
    unsigned int orgTimeout;
    float timeout;
    bool xonxoff;
    bool rtscts;
    bool dsrdtr;
    bool isOpen;
    bool breakCondition;
} SerialFTD;

extern SerialFTD serialFtd;
extern Port port;

void open_ft_device_id(int index);

void open(char* description);

void reconfigure_port(void);

void close(void);

unsigned char* read(int size);

unsigned long write(unsigned char* data, int length);

void flushInput(void);

void flushOutput(void);

unsigned long getNumDevices(void);

#endif /* SERIALFTD_H */