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
#include <unistd.h>
#include <string.h>
#include "serialftd.h"
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
#define ACTION_ERASE_CHECK 0x04

#define MAXDATA 224

#define RX_FRAME_IN 3
#define RX_FRAME_LENGTH 4

#define TX_FRAME_LENGTH 4

#define F1x "F1x family"
#define F2x "F2x family"
#define F4x "F4x family"

typedef struct {
    bool invertRST;
    bool invertTEST;
    bool swapResetTest;
    int testOnTX;
    int ignoreAnswer;
    int allowNAK;
    int protocolMode;
    int BSLMemAccessWarning;
    int slowMode;
    uint16_t _CBUS;
    int reqNo;
    int seqNo;
    bool usb;
} BSLlowlevel;

typedef struct {
    BSLlowlevel lowLevel;
    SerialFTD* serialFtd;
    Memory data;
    int byteCtr;
    int meraseCycles;
    int patchRequired;
    int bslVer;
    int maxData;
    int retransmitPasswd;
    int debug;
    char* passwd;
    char* cpu;
    bool patchLoaded;
} PAD6BootStrapLoader;

typedef struct {
    unsigned char *rxHeader;
    unsigned char *rxNum;
} RxHeader;

extern PAD6BootStrapLoader bslobj;
extern SerialFTD serialFtd;

unsigned short calcChecksum(unsigned char* data, int length);

void initLowLevel(void);

void comDone(void);

RxHeader* comRxHeader(void);

unsigned char* comRxFrame(unsigned char* rxNum);

unsigned char* comTxRx(unsigned char* cmd, unsigned char* dataOut, unsigned char* lengthChar, int length);

void setRSTpin(int level);

void setTESTpin(int level);

void bslReset(bool invokeBSL);

void bslSync(void);

unsigned char* bslTxRx(unsigned char* cmd, unsigned char* addr, unsigned char* length, unsigned char* blkout, int wait, bool sync);

void initBSLobj(void);

void setDebug(int debug);

void comInitK(Port port, int baud, bool usb);

void programData(Memory segments, int action);

void preparePatch(void);

void postPatch(void);

void verifyBlk(unsigned char* addr, unsigned char* blkout, unsigned char* strLength, int action);

void programBlk(unsigned char* addr, unsigned char* blkout, int action);

unsigned char* uploadData(unsigned char* startAddress, int size);

void txPasswd(unsigned char* passwd);

void actionMassErase(void);

void actionInfoErase(void);

void actionSegmentErase(unsigned char* address);

void actionDownWatchDogReset(Memory bslsegments, unsigned char* startAddress);

void actionProgram(void);

#endif /* PAD6BSL_H */