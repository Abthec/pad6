PAD6BootStrapLoader bslobj;
SerialFTD serialFtd;
PrattleyDevice prattley;

typedef struct {
    unsigned char* swVersion;
    unsigned char* infomem;
    int pcbType;
    int pcbVersion;
    int radioID;
} PrattleyDevice;

PrattleyDevice initPrattleyDevice(Port port, bool usb)
{
    bslobj.lowLevel.usb = usb;
    serialFtd.port = port;
}

void open()
{
    unsigned char* password;
    unsigned char* swVersion;
    unsigned char* pcbInfo;
    unsigned char* radioID;
    unsigned long bytesRead;
    int special;
    int pcbType;
    int pcbVersion;

    div_t typeVersion;

    comInitK(serialFtd.port, 300, bslobj.lowLevel.usb);

    flushInput();
    flushOutput();

    bytesRead = write_to_ftdi("K");

    printf("Bytes Read = %d\n", bytesRead);

    password = read_from_ftdi(1);


    comDone();
    comInitK(serialFtd.port, 300, true);
    bslReset(true);
    special = 1;

    serialFtd.timeout = 6;
    serialFtd.parity = PARITY_EVEN;
    serialFtd.baudrate = 9600;
    sleep(0.010);

    txPasswd(password);
    swVersion = uploadData(INFOMEM_ADDRESS, 2);

    infomem = repairInfomem(uploadData(INFOMEM_ADDRESS, INFOMEM_SIZE), special);

    // for (int i=0; i<strlen(infomem); i+=16) {
    //     for (int j=0; j<16; j++) {
    //         printf
    //     }
    // }

    //pcbInfo, RadioID = struct.unpack("<HH", infomem[:4]);
    typeVersion = div(pcbInfo, 10);

    prattley.swVersion = swVersion;
    prattley.pcbType = pcbType;
    prattley.pcbVersion = pcbVersion;
    prattley.radioID = radioID;

}

void close()
{
    unsigned char* go_addr;
    // go_addr = struct.unpack( "<H", self.password[-2:])[0] #ord(password[-1])*256 + ord(password[-2]);

    actionRun(go_addr);
    comDone(); 
}

void flash()
{
    loadTIText(FLASH_FILES[pcbType], bslobj.data);

    if (prattley.infomem) {
        for (int i=0; i<bslobj.data.numSegments; i++) {
            if (bslobj.data.segs[i].startAddr == INFOMEM_ADDRESS) {
                bslobj.data.segs[i].data = prattley.infomem;
                break;
            }
        }
    }

    actionMassErase();

    
}

void repairInfomem()
{

}

void userWantsToFlashDevice()
{

}

void populateComList()
{

}

void updatePrattleyDevice()
{
    
}

void updatePrattleyDevices()
{

}

void createUSBDevicesList()
{

}