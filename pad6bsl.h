
#ifndef PAD6BSL_H
#define PAD6BSL_H

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
#include "memory.h"

#define MODE_SSP 0
#define MODE_BSL 1

#define BSL_SYNC 0x80
#define BSL_TXPWORD 0x10
#define BSL_TXBLK 0x12
#define BSL_RXBLK 0x14
#define BSL_ERASE 0x16
#define BSL_MERAS 0x18
#define BSL_CHANGEBAUD 0x20
#define BSL_LOADPC 0x1A
#define BSL_TXVERSION 0x1E

#define BSL_CRITICAL_ADDR 0x0A00

// Header Definitions
#define CMD_FAILED 0x70
#define DATA_FRAME 0x80
#define DATA_ACK 0x90
#define DATA_NAK 0xA0

#define QUERY_POLL 0xB0
#define QUERY_RESPONS 0x50

#define OPEN_CONNECTION 0xC0
#define ACK_CONNECTION 0x40

#define DEFAULT_TIMEOUT 1
#define DEFAULT_PROLONG 10
#define MAX_FRAME_SIZE 256
#define MAX_DATA_BYTES 250
#define MAX_DATA_WORDS 125

#define MAX_FRAME_COUNT 16

#define ACTION_PROGRAM 0x01
#define ACTION_VERIFY 0x02
#define ACTION_ERASE_CHECL 0x03

#define MAXDATA 224

#define F1x "F1x family"
#define F2x "F2x family"
#define F4x "F4x family"

typedef struct{
    int invertRST;
    int invertTEST;
    int swapResetTest;
    int testOnTX;
    int ignoreAnswer;
    int allowNAK;
    int protocolMode;
    int BSLMemAccessWarning;
    int slowMode;
    int _CBUS;
    bool usb;
} BSLlowlevel;

typedef struct {
    BSLlowlevel lowLevel;
    int byteCtr;
    int meraseCycles;
    int patchRequired;
    int patchLoaded;
    int bslVer;
    char* passwd;
    char* data;
    int maxData;
    char* cpu;
    int retransmitPasswd;
} PAD6BootStrapLoader;


extern PAD6BootStrapLoader bslobj;

unsigned short calcChecksum(char* data, int length);

// BSL* initBSL(int aTimeout, int aProlongFactor);

void comRxHeader(void);

void comDone(void);

unsigned char* comRxFrame(int rxNum);

// void comTxHeader(unsigned char txHeader); unused?

int comTxRx(unsigned char cmd, unsigned char* dataOut, int length);

void SetRSTpin(int level);

void SetTESTpin(int level);

void bslReset(int invokeBSL);

void bslSync(int wait);

void bslTxRx(unsigned int cmd, unsigned long addr, unsigned int length=0, char* blkout=NULL, int wait=0, bool sync);

void setDebug(int debug);

void comInitK(char* port, int baud, int usb);

void programData(Memory segments, int action);

void preparePatch(void);

void postPatch(void);

void verifyBlk(unsigned long addr, char* blkout, int action);

void programBlk(unsigned long addr, char* blkout, int action);

// void programBlock(unsigned long addr, char* data); unused?

char* uploadData(unsigned long startAddress, int size, int wait);

void txPasswd(char* passwd, int wait);

// void actionUnlockInfoA(void); unused?

void actionMassErase(void);

// void actionMainErase(bool sync=true);

void actionInfoErase(void);

void actionSegmentErase(unsigned long address);

// makeActionSegmentErase(); not sure what this actually does

// void actionStartBSL(int usepatch, int adjsp, Memory replacementBSL, int forseBSL, int mayuseBSL, int bslreset); unused?

// actionGetDeviceID(); unused?

// actionDownloadBSL(); only used inside another unused function?

void actionDownWatchDogReset(Memory bslsegments, unsigned int startAddress=NULL);

// void actionEraseCheck(void); unused?

void actionProgram(void);

// void actionVerify(void); unused?

// void actionReset(void); unused?

// void actionRun(unsigned int address=0x220); unused?

// void actionChangeBaudRate(int baudrate=9600, bool sync=true); only used in another unused function?

// char* actionReadBSLVersion(bool sync=true); unused?

#endif /* PAD6BSL_H */