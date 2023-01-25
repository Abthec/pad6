#include "pad6bsl.h"

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
        FT_SetBitMode();
    }
}

void comRxHeader(void) 
{

}