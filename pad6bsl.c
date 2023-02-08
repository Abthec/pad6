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
    bslobj.lowLevel._CBUS = (unsigned char*)0x70;
    bslobj.lowLevel.usb = false;

}

void comDone(void)
{
    if (DEBUG == 1) {
        setRSTpin(0);
        setTESTpin(0);
    }
    if (bslobj.lowLevel.usb == 1) {
        FT_SetBitMode(bslobj.serialFtd.ftHandle, 0x00, 0x20);
    }
    close_ftdi_device();
}

RxHeader* comRxHeader(void) 
{
    RxHeader *header;
    (*header).rxHeader = malloc(sizeof(unsigned char));
    (*header).rxNum = malloc(sizeof(unsigned char));

    unsigned char* hdr;
    hdr = read_from_ftdi(1);

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
    unsigned char*  rxFrameData = read_from_ftdi(RX_FRAME_IN);
    unsigned char* rxFrame = malloc(RX_FRAME_LENGTH * sizeof(unsigned char));

    memcpy(rxFrame, &rxFrameTemp, 1);
    memcpy(rxFrame+1, rxFrameData, RX_FRAME_IN);

    if (rxFrame[1] == '0' && rxFrame[2] == rxFrame[3]) {
        rxLengthCrc = rxFrame[2] + 2;
    }

    free(rxFrameData);

    unsigned char* rxFrameData2 = read_from_ftdi(rxLengthCrc);

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

    write_to_ftdi(txFrame, length+5);

    header = comRxHeader();

    if (*(*header).rxHeader == DATA_ACK) {
        if (*(*header).rxNum == bslobj.lowLevel.reqNo) {
            bslobj.lowLevel.seqNo = bslobj.lowLevel.reqNo;
            return "";
        }
    } else if (*(*header).rxHeader == DATA_NAK) {
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
    printf("RST lvl=%d\n", level);
    if (bslobj.lowLevel.invertRST) {
        if (level == 0) {
            level = 1;
        } else {
            level = 0;
        }
    }

    uint16_t mask = (uint16_t)bslobj.lowLevel._CBUS;

    if (level > 0) {
        mask |= 0x22;
    } else {
        mask &= ~0x02;
    }

    FT_SetBitMode(serialFtd.ftHandle, mask, 0x20);
    bslobj.lowLevel._CBUS = (unsigned char*)mask;

    sleep(0.010);
}

void setTESTpin(int level)
{
    printf("TEST lvl=%d\n", level);
    if (bslobj.lowLevel.invertTEST) {
        if (level == 0) {
            level = 1;
        } else {
            level = 0;
        }
    }

    uint16_t mask = (uint16_t)bslobj.lowLevel._CBUS;
    if (level > 0) {
        mask |= 0x44;
    } else {
        mask &= ~0x44;
    }

    FT_SetBitMode(serialFtd.ftHandle, mask, 0x20);
    bslobj.lowLevel._CBUS = (unsigned char*)mask;

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

void bslSync(void) 
{
    int loopcnt = 3;

    unsigned char* c;
    unsigned char bslSyncChar = BSL_SYNC;

    while (loopcnt > 0) {
        loopcnt--;
        flushInput();

        write_to_ftdi(&bslSyncChar, 1);

        c = read_from_ftdi(1);

        printf("c = %s\n", c);

        if (c == DATA_ACK) {
            // "success"
            printf("Sync Success\n");
        } else {
            // "failure"
            printf("Sync Failure\n");
        }
    }
}

unsigned char* bslTxRx(unsigned char* cmd, unsigned char* addr, unsigned char* length, unsigned char* blkout, int wait, bool sync)
{   

    unsigned char filler = 0xFF;
    unsigned char* dataOut;
    unsigned char* rxFrame;

    if (*cmd == BSL_TXBLK) {

        unsigned char* blkout2 = malloc((*length)+1);
        
        if ((*addr % 2) != 0) {
            (*addr)--;
            memcpy(blkout2, &filler, 1);
            memcpy(blkout2+1, blkout, *length);
            (*length)++;
        }

        if ((*length % 2) != 0) {
            blkout2 = realloc(blkout2, (*length)+1);
            memcpy(blkout2+(*length), &filler, 1);
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
       memcpy(dataOut+2, blkout, *length);
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

void initBSLobj(void)
{
    bslobj.byteCtr = 0;
    bslobj.meraseCycles = 1;
    bslobj.patchRequired = 0;
    bslobj.patchLoaded = 0;
    bslobj.bslVer = 0;
    bslobj.maxData = MAXDATA;
    bslobj.retransmitPasswd = 1;
}

void setDebug(int debug)
{
    bslobj.debug = debug;
}

void comInitK(Port port, int baud, bool usb)
{

    if (baud == 0) {
        baud = 300;
    }

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

    int pstart=0;
    int length=0;

    unsigned char* tempData = malloc(1);
    unsigned char* currentAddr;

    for (int i=0; i<segments.numSegments; i++) {
        currentAddr = segments.segs[i].startAddress;
        while (pstart < segments.segs[i].numBytes) {
            length = bslobj.maxData;

            if (pstart+length > segments.segs[i].numBytes) {
                length = segments.segs[i].numBytes - pstart;
            }

            tempData = realloc(tempData, length);
            memcpy(tempData, segments.segs[segments.numSegments].data+pstart, length);
            programBlk(currentAddr, tempData, action);

            pstart += length;
            currentAddr = currentAddr + length;
            bslobj.byteCtr += length;
        }
    }
}

void preparePatch(void)
{
    unsigned char* length = 0b0;
    unsigned char loadPcChar = BSL_LOADPC;
    unsigned char* address = "0220";
    if (bslobj.patchLoaded) {
        bslTxRx(&loadPcChar, address, length, NULL, 0, true);

        bslobj.lowLevel.BSLMemAccessWarning = 0;
    }
}

void postPatch(void)
{
    if (bslobj.patchLoaded) {
        bslobj.lowLevel.BSLMemAccessWarning = 1;
    }
}

void verifyBlk(unsigned char* addr, unsigned char* blkout, unsigned char* strLength, int action)
{
    unsigned char* blkin;
    unsigned char rxBlk = BSL_RXBLK;
    int length = strlen(blkout);

    if ((action & ACTION_VERIFY) || (action & ACTION_ERASE_CHECK)) {
        
        preparePatch();
        blkin = bslTxRx(&rxBlk, addr, strLength, blkout, 0, false);
        postPatch();

        for (int i=0; i<length; i++) {
            if (action & ACTION_VERIFY) {
                if (blkin[i] != blkout[i]) {
                    // verification failed
                }
                continue;
            } else if (action & ACTION_ERASE_CHECK) {
                if (blkin[i] != 0xFF) {
                    // Erase check failed
                }
                continue;
            }
        }
    }
}

void programBlk(unsigned char* addr, unsigned char* blkout, int action)
{
    unsigned char* strLength = malloc(1);
    unsigned char txBlk = BSL_TXBLK;
    int length = strlen(blkout);
    sprintf(strLength, "%d", length);
    verifyBlk(addr, blkout, strLength, action);
    
    if (action & ACTION_PROGRAM) {
        preparePatch();
        bslTxRx(&txBlk, addr, strLength, blkout, 0, true);
        postPatch();
    }

    verifyBlk(addr, blkout, strLength, action & ACTION_VERIFY);
}

unsigned char* uploadData(unsigned char* addr, int size)
{

    unsigned char* data;
    unsigned char* tempData = malloc(1);
    unsigned char* startAddr;
    unsigned char* lenPtr;
    int pstart = 0;
    int copyStart = 0;
    int length;

    while (pstart < size) {

        length = bslobj.maxData;

        if (pstart+length > size) {
            length = size - pstart;
        }

        startAddr = (unsigned char*) (pstart + (int)addr);
        lenPtr = (unsigned char*) length;

        tempData = realloc(tempData, length);

        tempData = bslTxRx(BSL_RXBLK, startAddr, lenPtr, NULL, false, true);

        memcpy(data+copyStart, tempData, length-2);

        copyStart += length-2;
        pstart += length;

        free(tempData);
    }

    return data;
}

void txPasswd(unsigned char* passwd)
{

    unsigned char* addr = 0xFFE0;
    unsigned char* length = 0x0020;
    unsigned char* cmd = BSL_TXPWORD;

    bslTxRx(cmd, addr, length, passwd, false , true);

}

void actionMassErase(void)
{

    unsigned char* cmd = BSL_MERAS;
    unsigned char* addr = (unsigned char*) 0xFFFE;
    unsigned char* length = (unsigned char*) 0xA506;
    unsigned char* passwd = "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

    for (int i=0; i<bslobj.meraseCycles; i++) {

        bslTxRx(cmd, addr, length, NULL, false, true);

    }

    txPasswd(passwd);
}

void actionInfoErase(void)
{
    unsigned char* cmd = BSL_ERASE;
    unsigned char* addr = (unsigned char*) 0x1010;
    unsigned char* length = (unsigned char*) 0xA504;

    bslTxRx(cmd, addr, length, NULL, false, true);
}

void actionSegmentErase(unsigned char* address)
{
    unsigned char* cmd = BSL_ERASE;
    unsigned char* length = 0xA502;

    bslTxRx(cmd, address, length, NULL, false, true);
}

void actionDownWatchDogReset(Memory bslsegments, unsigned char* startAddress)
{

    unsigned char length = 0x02;
    unsigned char* blkin;

    programData(bslsegments, ACTION_PROGRAM);

    if (startAddress == 0) {

        blkin = bslTxRx(BSL_RXBLK, bslsegments.segs[0].startAddress, &length, NULL, false, true);
        memcpy(startAddress, blkin, 2);
        
    }

    bslTxRx(BSL_LOADPC, startAddress, 0, NULL, false, true);

}

void actionProgram(void)
{
    programData(bslobj.data, ACTION_PROGRAM);
}