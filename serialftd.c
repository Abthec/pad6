#include "serialftd.h"

Port port;
SerialFTD serialFtd;

void open_ft_device_id(int index)
{
    serialFtd.ftHandle = FT_Open(index);

    FT_W32_SetupComm(serialFtd.ftHandle, DEFAULT_BUF_SIZE, DEFAULT_BUF_SIZE);

    reconfigure_port();

    FT_W32_PurgeComm(ftHandle, FT_PURGE_RX);
    FT_W32_PurgeComm(ftHandle, FT_PURGE_TX);

    serialFtd.isOpen = true;
}

void open(char* description)
{
    serialFtd.ftHandle = FT_OpenEx(description, FT_OPEN_BY_DESCRIPTION);

    FT_W32_SetupComm(serialFtd.ftHandle, DEFAULT_BUF_SIZE, DEFAULT_BUF_SIZE);

    reconfigure_port();

    FT_W32_PurgeComm(ftHandle, FT_PURGE_RX);
    FT_W32_PurgeComm(ftHandle, FT_PURGE_TX);

    serialFtd.isOpen = true;
}

void reconfigure_port(void) 
{
    FTTIMEOUTS timeouts = {5000, 5000, 5000, 5000, 5000};
    FT_W32_SetCommTimeouts(serialFtd.ftHandle, &timeouts);
    FT_W32_SetCommMask(serialFtd.ftHandle, WIN32_EV_ERR);

    FTDCB comDCB;
    FT_W32_GetCommState(serialFtd.ftHandle, &comDCB);
    comDCB.BaudRate = serialFtd.baudrate;
}