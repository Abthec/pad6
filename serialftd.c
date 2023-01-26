#include "serialftd.h"

void open_ft_device_id(int index)
{
    serialFtd.ftStatus = FT_Open(index, &(serialFtd.ftHandle));

    serialFtd.orgTimeout = (serialFtd.ftHandle, DEFAULT_BUF_SIZE, DEFAULT_BUF_SIZE);

    reconfigure_port();

    FT_W32_PurgeComm(serialFtd.ftHandle, FT_PURGE_RX);
    FT_W32_PurgeComm(serialFtd.ftHandle, FT_PURGE_TX);

    serialFtd.isOpen = true;
}

void open(char* description)
{
    serialFtd.ftStatus = FT_OpenEx(description, FT_OPEN_BY_DESCRIPTION, &(serialFtd.ftHandle));

    FT_W32_SetupComm(serialFtd.ftHandle, DEFAULT_BUF_SIZE, DEFAULT_BUF_SIZE);

    reconfigure_port();

    FT_W32_PurgeComm(serialFtd.ftHandle, FT_PURGE_RX);
    FT_W32_PurgeComm(serialFtd.ftHandle, FT_PURGE_TX);

    serialFtd.isOpen = true;
}

void reconfigure_port(void) 
{
    FTTIMEOUTS ftimeouts = {5000, 5000, 5000, 5000, 5000};
    FT_W32_SetCommTimeouts(serialFtd.ftHandle, &ftimeouts);
    FT_W32_SetCommMask(serialFtd.ftHandle, WIN32_EV_ERR);

    FTDCB comDCB;
    FT_W32_GetCommState(serialFtd.ftHandle, &comDCB);
    comDCB.BaudRate = serialFtd.baudrate;
    comDCB.ByteSize = serialFtd.byteSize;
    comDCB.fBinary = 1;

    switch(serialFtd.parity) 
    {
        case NO_PARITY:
            comDCB.Parity = NO_PARITY;
            comDCB.fParity = 0;
            break;
        case EVEN_PARITY:
            comDCB.Parity = EVEN_PARITY;
            comDCB.fParity = 1;
            break;
        case ODD_PARITY:
            comDCB.Parity = ODD_PARITY;
            comDCB.fParity = 1;
            break;
        case MARK_PARITY:
            comDCB.Parity = MARK_PARITY;
            comDCB.fParity = 0;
            break;
        case SPACE_PARITY:
            comDCB.Parity = SPACE_PARITY;
            comDCB.fParity = 0;
            break;
        default:
            comDCB.Parity = NO_PARITY;
            comDCB.fParity = 0;
            break;
    }

    switch (serialFtd.stopBits)
    {
    case STOPBITS_ONE:
        comDCB.StopBits = STOPBITS_ONE;
        break;
    case STOPBITS_TWO:
        comDCB.StopBits = STOPBITS_TWO;
        break;    
    default:
        comDCB.StopBits = STOPBITS_ONE;
        break;
    }

    comDCB.fOutX = serialFtd.xonxoff;
    comDCB.fInX = serialFtd.xonxoff;
    comDCB.fNull = 0;
    comDCB.fErrorChar = 0;
    comDCB.fAbortOnError = 0;
    comDCB.XonChar = "N";
    comDCB.XoffChar = "F";
    
    FT_W32_SetCommState(serialFtd.ftHandle, &comDCB);

}


void close(void)
{
    if (serialFtd.isOpen) {
        if (serialFtd.ftHandle) {
            FT_W32_SetCommTimeouts(serialFtd.ftHandle, serialFtd.orgTimeout);

            FT_W32_CloseHandle(serialFtd.ftHandle);
            serialFtd.ftHandle = NULL;
        }
    }
}

unsigned char* read(int size)
{
    unsigned long read_count;
    char *buffer;
    
    if (size==NULL) {
        size = 1;
    }
    if (serialFtd.isOpen) {
        buffer = (unsigned char *) malloc(size * sizeof(unsigned char));
        serialFtd.ftStatus = FT_READ(serialFtd.ftHandle, buffer, size, read_count);
    }

    return buffer;
}

unsigned long write(unsigned char* data)
{
    unsigned long bytes_written;
    if (serialFtd.isOpen) {
        serialFtd.ftStatus = (serialFtd.ftHandle, data, sizeof(data), bytes_written);
    }
    return bytes_written;
}

void flushInput(void)
{
    if (serialFtd.isOpen) {
        FT_W32_PurgeComm(serialFtd.ftHandle, FT_PURGE_RX);
    }
}

void flushOutput(void)
{
    if (serialFtd.isOpen) {
        FT_W32_PurgeComm(serialFtd.ftHandle, FT_PURGE_TX);
    }
}

unsigned long getNumDevices(void)
{
    unsigned long numDevs;
    serialFtd.ftStatus = FT_CreateDeviceInfoList(numDevs); 
    return numDevs;
}