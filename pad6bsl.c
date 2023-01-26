#include "pad6bsl.h"
#include "serialftd.h"

SerialFTD serialFtd;
Port port;

unsigned short calcChecksum(char* data, int length) 
{
    unsigned short checksum = 0;

    for (int i = 0; i < length/2; i++) {
        checksum = checksum ^ (data[i*2] | (data[i*2+1] << 8));
    }

    return 0xffff & (checksum ^ 0xffff);
}

void comDone(void)
{
    if (DEBUG == 1) {
        SetRSTpin(0);
        SetTESTpin(0);
    }
    if (bslobj.lowLevel.usb == 1) {
        FT_SetBitMode(serialFtd.ftHandle, 0x00, 0x20);
    }
    close();
}

RxHeader* comRxHeader(void) 
{
    RxHeader *header = (RxHeader*)malloc(sizeof(RxHeader));
    unsigned char *hdr = read(1);
    memcpy((*header).rxHeader, hdr+4, 4);
    memcpy((*header).rxNum, hdr, 4);

    if (bslobj.lowLevel.protocolMode == MODE_BSL) {
        bslobj.lowLevel.seqNo = 0;
        bslobj.lowLevel.reqNo = 0;
        memset((*header).rxNum, 0, sizeof(unsigned char)*4);
    }
    return header;
}

unsigned char* comRxFrame(unsigned char* rxNum)
{
    unsigned char* rxFrame = DATA_FRAME | *rxNum;
    unsigned char* rxFrameData = read(3);
    
}