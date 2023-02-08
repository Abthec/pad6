#include "PED7update.h"

PrattleyDevice padd;

void initPrattleyDevice(void)
{   
    PAD6BootStrapLoader bslobj;
    BSLlowlevel lowLevel;
    SerialFTD serialFtd;
    Port port = {0, "Prattley Auto Drafter 6"};

    serialFtd.port = port;
    bslobj.lowLevel = lowLevel;
    bslobj.serialFtd = serialFtd;

    padd.bslobj = bslobj;

    initLowLevel();
    initBSLobj();

    padd.password = 0;
    padd.infomem = 0;
    padd.infoD = 0;
    padd.pcbType = 0;
    padd.pcbVersion = 0;
    padd.swVersion = 0;
    padd.radioID = 0;

}

void close(void)
{
    comDone();
}

void softwareConnection(void)
{
    comInitK(padd.bslobj.serialFtd.port, 0, padd.bslobj.lowleve.usb);

    sleep(0.25);

    flushInput();
    flushOutput();

    
}

void hardwareBSLConnection(void)
{

}

void repairInfomem(unsigned char* missingSerialNo)
{

}

void getInfo(void)
{

}

void openBlank(void)
{

}

void reboot(void)
{

}

void flash(void)
{

}

void access_PED7(void)
{

}

void USBDevices(void)
{

}

void serialDevices(void)
{

}

void displayInfomem(unsigned char* serialNum)
{

}

void autoUpdate(unsigned char* serialNum)
{

}

void autoUpdates(unsigned char* serialNum)
{

}

void mainWindow(void)
{

}