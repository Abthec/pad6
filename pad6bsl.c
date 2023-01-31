#include "pad6bsl.h"

unsigned short calcChecksum(unsigned char* data, int length) 
{
    unsigned short checksum = 0;

    for (int i = 0; i < length/2; i++) {
        checksum = checksum ^ (data[i*2] | (data[i*2+1] << 8));
    }

    return 0xffff & (checksum ^ 0xffff);
}

void initLowLevel(void)
{
    bslobj.lowLevel.invertRST = 0;
    bslobj.lowLevel.invertTEST = 0;
    bslobj.lowLevel.swapResetTest = 0;
    bslobj.lowLevel.testOnTX = 0;
    bslobj.lowLevel.ignoreAnswer = 0;
    bslobj.lowLevel.allowNAK = 0;
    bslobj.lowLevel.protocolMode = MODE_BSL;
    bslobj.lowLevel.BSLMemAccessWarning = 0;
    bslobj.lowLevel.slowMode = 0;

    *(bslobj.lowLevel._CBUS) = 0x70;
    bslobj.lowLevel.usb = false;
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
    RxHeader *header;
    (*header).rxHeader = malloc(sizeof(unsigned char));
    (*header).rxNum = malloc(sizeof(unsigned char));

    unsigned char* hdr;
    hdr = read(1);

    memcpy((*header).rxHeader, hdr, sizeof(unsigned char));
    memcpy((*header).rxNum, hdr, sizeof(unsigned char));

    *((*header).rxHeader) |= 0xF0;
    *((*header).rxNum) |= 0x0F;

    if (bslobj.lowLevel.protocolMode == MODE_BSL) {
        bslobj.lowLevel.seqNo = 0;
        bslobj.lowLevel.reqNo = 0;
        memset((*header).rxNum, 0, sizeof(unsigned char));
    }

    free(hdr);

    return header;
}

unsigned char* comRxFrame(unsigned char* rxNum)
{
    int rxLengthCrc;
    unsigned char rxFrameTemp = DATA_FRAME | (*rxNum);
    unsigned char*  rxFrameData = read(RX_FRAME_IN);
    unsigned char* rxFrame = malloc(RX_FRAME_LENGTH * sizeof(unsigned char));

    memcpy(rxFrame, &rxFrameTemp, 1);
    memcpy(rxFrame+1, rxFrameData, RX_FRAME_IN);

    if (rxFrame[1] == '0' && rxFrame[2] == rxFrame[3]) {
        rxLengthCrc = rxFrame[2] + 2;
    }

    free(rxFrameData);

    unsigned char* rxFrameData2 = read(rxLengthCrc);

    rxFrame = realloc(rxFrame, (RX_FRAME_LENGTH+rxLengthCrc) * sizeof(unsigned char));

    memcpy(rxFrame+RX_FRAME_LENGTH, rxFrameData2, rxLengthCrc);

    unsigned short checksum = calcChecksum(rxFrame, rxFrame[2] + 4);

    if ((rxFrame[(rxFrame[2] + 4)] == (0xFF & checksum)) && 
         rxFrame[(rxFrame[2] + 5)] == (0xFF & checksum >> 8)) {
        return rxFrame;
    } else {
        return "";
    }
}

unsigned char* comTxRx(unsigned char* cmd, unsigned char* dataOut, unsigned char* lengthChar, int length)
{
    unsigned char* txFrame;
    unsigned char* rxFrame;
    unsigned short checksum;
    unsigned short checksumPt1;
    unsigned short checksumPt2;
    unsigned short accessAddr;
    unsigned char dataSeq = DATA_FRAME | (bslobj.lowLevel.seqNo);
    int txFramePos = 0;
    int lengthLen;
    RxHeader* header;

    if (length % 2 != 0) {
        if (bslobj.lowLevel.protocolMode == MODE_BSL) {
            dataOut[length] = 0xFF;
            dataOut[length+1] = '\0';
        } else {
            dataOut[length] = 0;
            dataOut[length+1] = '\0';
        }
        length += 1;
    }

    txFrame = malloc(sizeof(unsigned char) * TX_FRAME_LENGTH);
    memcpy(txFrame, &dataSeq, 1);
    memcpy(txFrame+1, cmd, 1);
    memcpy(txFrame+2, lengthChar, 1);
    memcpy(txFrame+3, lengthChar, 1);

    bslobj.lowLevel.reqNo = (bslobj.lowLevel.seqNo + 1) % MAX_FRAME_COUNT;

    txFrame = realloc(txFrame, sizeof(unsigned char) * (40+length));

    memcpy(txFrame+4, dataOut, length);

    checksum = calcChecksum(txFrame, length+4);
    checksumPt1 = checksum & 0xFF;
    checksumPt2 = (checksum >> 8) & 0xFF;

    memcpy(txFrame+4+length, &checksumPt1, 1);
    memcpy(txFrame+5+length, &checksumPt2, 1);

    accessAddr = (0x0212 + (checksum^0xFFFF)) & 0xFFFE;

    flushInput();

    write(txFrame, length+5);

    header = comRxHeader();

    if (*(*header).rxHeader == DATA_ACK) {
        if (*(*header).rxNum == bslobj.lowLevel.reqNo) {
            bslobj.lowLevel.seqNo = bslobj.lowLevel.reqNo;
            return "";
        }
    } else if ((*header).rxHeader == DATA_NAK) {
        if (bslobj.lowLevel.allowNAK) {
            if (*(*header).rxNum == bslobj.lowLevel.reqNo) {
                bslobj.lowLevel.seqNo = bslobj.lowLevel.reqNo;
                return "";
            }
        }
    } else if (*(*header).rxHeader == DATA_FRAME) {
        if (*(*header).rxNum == bslobj.lowLevel.reqNo) {
            rxFrame = comRxFrame((*header).rxNum);
            return rxFrame;
        }
    } else if (*(*header).rxHeader == CMD_FAILED) {
        return "";
    } else {
        return "";
    }
}

void setRSTpin(int level) 
{
    if (bslobj.lowLevel.invertRST) {
        if (level == 0) {
            level = 1;
        } else {
            level = 0;
        }
    }
    if (bslobj.lowLevel.usb) {
        unsigned char mask = bslobj.lowLevel._CBUS;
        if (level > 0) {
            mask |= 0x22;
        } else {
            mask &= ~0x02;
        }
        FT_SetBitMode(serialFtd.ftHandle, mask, 0x20);
    }
    sleep(0.010);
}

void setTESTpin(int level)
{
    
    if (bslobj.lowLevel.invertTEST) {
        if (level == 0) {
            level = 1;
        } else {
            level = 0;
        }
    }
    if (bslobj.lowLevel.usb) {
        unsigned char mask = bslobj.lowLevel._CBUS;
        if (level > 0) {
            mask |= 0x44;
        } else {
            mask &= ~0x44;
        }
        FT_SetBitMode(serialFtd.ftHandle, mask, 0x20);
    }
    sleep(0.010);
}

void bslReset(bool invokeBSL)
{
    setRSTpin(0);
    setTESTpin(1);

    sleep(0.250);

    setRSTpin(0);

    if (invokeBSL) {
        setTESTpin(1);
        setTESTpin(0);
        setTESTpin(1);
        setTESTpin(0);
        setRSTpin(1);
        if (bslobj.lowLevel.testOnTX) {
            serialFtd.breakCondition = false;
        } else {
            setTESTpin(1);
        }
    }
    sleep(0.250);
    flushInput();
}

void bslSync(int wait) 
{
    int loopcnt = 3;

    unsigned char* c;

    while (loopcnt > 0) {
        loopcnt--;
        flushInput();

        write(BSL_SYNC, 1);

        c = read(1);

        if (*c == DATA_ACK) {
            // "success"
        } else {
            // "failure"
        }
    }
}

unsigned char* bslTxRx(unsigned char* cmd, unsigned char* addr, unsigned char* length, unsigned char* blkout, int wait, bool sync)
{   

    unsigned char filler = 0xFF;
    unsigned char* dataOut;
    unsigned char* rxFrame;

    if (*cmd == BSL_TXBLK) {

        unsigned char* blkout2 = malloc(sizeof(unsigned char) * ((*length)+1));
        
        if ((addr % 2) != 0) {
            (*addr)--;
            memcpy(blkout2, &filler, 1);
            memcpy(blkout2+1, blkout, length);
            (*length)++;
        }

        if ((length % 2) != 0) {
            blkout2 = realloc(blkout2, (*length)+1);
            memcpy(blkout2+(*length), %filler, 1);
            length++;
        }

        memcpy(blkout, blkout2, *length);

        free(blkout2);

    } else if (*cmd == BSL_RXBLK) {
        if ((*(addr) % 2) != 0) {
            (*addr)--;
            (*length)+=8;
        }
        if ((*(length) % 2) != 0) {
            (*addr)--;
            (*length)++;
        }
    }

    memcpy(dataOut, addr, 1);
    memcpy(dataOut+1, length, 1);

    if (blkout) {
       memcpy(dataOut+2, blkout, length);
    }
    
    if (sync) {
        bslSync();
    }

    (*length) += 2;

    rxFrame = comTxRx(cmd, dataOut, length, *length);

    if (*cmd == BSL_RXBLK) {
        return rxFrame+4;
    } else {
        return rxFrame;
    }

}

void setDebug(int debug)
{
    bslobj.DEBUG = debug;
}

void comInitK(Port port, int baud, bool usb)
{
    bslobj.lowLevel.seqNo = 0;
    bslobj.lowLevel.reqNo = 0;

    bslobj.lowLevel.usb = usb;
    bslobj.serialFtd.port = port;

    if (usb) {
        if (port.description) {
            bslobj.serialFtd.baudrate = baud;
            bslobj.serialFtd.parity = PARITY_NONE;
            bslobj.serialFtd.timeout = 0.2;
        }
    } else if (!usb) {
        if (port.index) {
            bslobj.serialFtd.baudrate = baud;
            bslobj.serialFtd.parity = PARITY_NONE;
            bslobj.serialFtd.timeout = 0.2;
        }
        open_ft_device_id(port.index);
    }

    setRSTpin(0);
    setTESTpin(1);

    flushInput();
    flushOutput();
}

void programData(Memory segments, int action)
{
    
}