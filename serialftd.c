#include "serialFtd.h"

void open_ft_device_id(int index)
{
    serialFtd.ftStatus = FT_Open(index, &(serialFtd.ftHandle));

    printf("FT_HANDLE @SerialFtd = %d\n", serialFtd.ftHandle);

    if (serialFtd.ftStatus == FT_OK) {
        serialFtd.isOpen = true;
    } else {
        serialFtd.isOpen = false;
    }

    FT_W32_SetupComm(serialFtd.ftHandle, DEFAULT_BUF_SIZE, DEFAULT_BUF_SIZE);

    reconfigure_port();

    FT_W32_PurgeComm(serialFtd.ftHandle, FT_PURGE_RX);
    FT_W32_PurgeComm(serialFtd.ftHandle, FT_PURGE_TX);

    
}

void open_with_description(unsigned char* description)
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
    FTTIMEOUTS ftimeouts = {1000, 1000, 1000, 1000, 1000};
    FT_W32_SetCommTimeouts(serialFtd.ftHandle, &ftimeouts);
    FT_W32_SetCommMask(serialFtd.ftHandle, WIN32_EV_ERR);

    serialFtd.orgTimeout = &ftimeouts;

    FTDCB comDCB;
    FT_W32_GetCommState(serialFtd.ftHandle, &comDCB);

    printf("Serial Baud = %d\n", serialFtd.baudrate);

    comDCB.BaudRate = 300;
    // comDCB.ByteSize = serialFtd.byteSize;
    comDCB.ByteSize = 8;
    comDCB.fBinary = 1;

    printf("Serial Parity = %d\n", serialFtd.parity);

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

    printf("Serial StopBits = %d\n", serialFtd.stopBits);

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

    // comDCB.fOutX = serialFtd.xonxoff;
    // comDCB.fInX = serialFtd.xonxoff;
    comDCB.fNull = 0;
    comDCB.fErrorChar = 0;
    comDCB.fAbortOnError = 0;
    comDCB.XonChar = 'N';
    comDCB.XoffChar = 'F';
    
    FT_W32_SetCommState(serialFtd.ftHandle, &comDCB);

}


void close_ftdi_device(void)
{
    if (serialFtd.isOpen) {
        if (serialFtd.ftHandle) {
            FT_W32_SetCommTimeouts(serialFtd.ftHandle, serialFtd.orgTimeout);

            FT_W32_CloseHandle(serialFtd.ftHandle);
            serialFtd.ftHandle = NULL;
        }
    }
}

unsigned char* read_from_ftdi(int size)
{
    unsigned long read_count;
    unsigned char* buffer;
    
    if (size<=1) {
        size = 1;
    }
    if (serialFtd.isOpen) {
        buffer = malloc(size * sizeof(unsigned char));
        serialFtd.ftStatus = FT_Read(serialFtd.ftHandle, buffer, size, &read_count);
    }

    printf("Bytes read from device = %d\n", read_count);

    return buffer;
}

unsigned long write_to_ftdi(unsigned char* data, int length)
{
    unsigned long bytes_written;
    if (serialFtd.isOpen) {
        serialFtd.ftStatus = FT_Write(serialFtd.ftHandle, data, length, &bytes_written);
    } else {
        printf("Serial Is Closed => Write Failed");
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
    serialFtd.ftStatus = FT_CreateDeviceInfoList(&numDevs); 
    return numDevs;
}