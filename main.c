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
#include <ctype.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include "pad6bsl.h"
// #include "memory.h"

// unsigned char* F1X_BSL = "@0220\n24 02 2E 02 31 40 20 02 2B D2 C0 43 EA FF 32 C2\nF2 C0 32 00 00 00 B2 40 80 5A 20 01 F2 40 85 00\n57 00 F2 40 80 00 56 00 E2 D3 21 00 E2 D3 22 00\nE2 C3 26 00 E2 C2 2A 00 E2 C2 2E 00 B2 40 10 A5\n2C 01 B2 40 00 A5 28 01 3B C0 3A 00 B0 12 D6 04\n82 43 12 02 09 43 36 40 0A 02 37 42 B0 12 AC 05\nC6 4C 00 00 16 53 17 83 F9 23 D2 92 0C 02 0D 02\n28 20 55 42 0B 02 75 90 12 00 80 24 75 90 10 00\n6D 24 B0 12 9C 04 55 42 0B 02 75 90 18 00 31 24\n75 90 1E 00 B8 24 75 90 20 00 17 24 2B B2 11 24\n75 90 16 00 22 24 75 90 14 00 B3 24 75 90 1A 00\n18 24 75 90 1C 00 45 24 04 3C B0 12 36 05 BE 3F\n21 53 B0 12 3C 05 BA 3F 03 43 B0 12 36 05 D2 42\n0E 02 56 00 D2 42 0F 02 57 00 D2 42 10 02 16 02\nAD 3F B0 12 36 05 10 42 0E 02 16 42 0E 02 15 43\n07 3C 36 40 FE FF B2 40 06 A5 10 02 35 40 0C 00\nB2 40 00 A5 2C 01 92 42 10 02 28 01 B6 43 00 00\n92 B3 2C 01 FD 23 15 83 F3 23 36 90 FE FF CD 27\n37 40 80 00 36 F0 80 FF 36 90 00 11 0E 28 07 57\n36 F0 00 FF 36 90 00 12 08 28 07 57 36 F0 00 FE\n04 3C 16 42 0E 02 17 42 10 02 35 43 75 96 03 20\n17 83 FC 23 B2 3F 82 46 00 02 B3 3F 36 40 E0 FF\n37 40 20 00 B0 12 AC 05 7C 96 01 24 2B D3 17 83\nF9 23 2B C2 B0 12 9C 04 2B D2 9F 3F 16 42 0E 02\n17 42 10 02 2B B2 38 24 3B D0 10 00 B0 12 AC 05\n36 90 00 10 06 2C 36 90 00 01 09 2C C6 4C 00 00\n25 3C B2 40 00 A5 2C 01 B2 40 40 A5 28 01 16 B3\n03 20 C2 4C 14 02 1A 3C C2 4C 15 02 86 9A FD FF\n08 24 2B D3 3B B0 20 00 04 20 3B D0 20 00 82 46\n00 02 36 90 01 02 04 28 3B D2 3B B0 10 00 02 24\n3B C0 32 00 1A 42 14 02 86 4A FF FF 16 53 17 83\nCD 23 B0 12 9C 04 61 3F B0 12 AC 05 17 83 FC 23\nB0 12 9C 04 5E 3F B2 40 F0 0F 0E 02 B2 40 10 00\n10 02 B2 40 80 00 0A 02 D2 42 10 02 0C 02 D2 42\n10 02 0D 02 82 43 12 02 09 43 36 40 0A 02 27 42\n7C 46 B0 12 40 05 17 83 FB 23 16 42 0E 02 17 42\n10 02 36 90 00 01 0A 28 B2 46 14 02 5C 42 14 02\nB0 12 40 05 17 83 5C 42 15 02 01 3C 7C 46 B0 12\n40 05 17 83 EE 23 B2 E3 12 02 5C 42 12 02 B0 12\n40 05 5C 42 13 02 B0 12 40 05 E0 3E 18 42 12 02\nB0 12 AC 05 C2 4C 12 02 B0 12 AC 05 C2 4C 13 02\n38 E3 3B B2 0A 24 86 9A FE FF 07 24 3B B0 20 00\n04 20 16 53 82 46 00 02 2B D3 18 92 12 02 08 23\n2B B3 06 23 30 41 E2 B2 28 00 FD 27 E2 B2 28 00\nFD 23 B2 40 24 02 60 01 E2 B2 28 00 FD 27 15 42\n70 01 05 11 05 11 05 11 82 45 02 02 05 11 82 45\n04 02 B2 80 1E 00 04 02 57 42 16 02 37 80 03 00\n05 11 05 11 17 53 FD 23 35 50 40 A5 82 45 2A 01\n35 42 B2 40 24 02 60 01 92 92 70 01 02 02 FC 2F\n15 83 F7 23 09 43 7C 40 90 00 02 3C 7C 40 A0 00\nC2 43 07 02 C9 EC 12 02 19 E3 1B C3 55 42 07 02\n55 45 56 05 00 55 0C 2E 2E 2E 2E 2E 2E 2E 2E 1A\n34 34 92 42 70 01 72 01 B2 50 0C 00 72 01 07 3C\n1B B3 0B 20 82 43 62 01 92 B3 62 01 FD 27 E2 C3\n21 00 0A 3C 4C 11 F6 2B 1B E3 82 43 62 01 92 B3\n62 01 FD 27 E2 D3 21 00 92 52 02 02 72 01 D2 53\n07 02 F0 90 0C 00 61 FC D1 23 30 41 C2 43 09 02\n1B C3 55 42 09 02 55 45 BC 05 00 55 0C 56 56 56\n56 56 56 56 56 36 76 00 E2 B2 28 00 FD 23 92 42\n70 01 72 01 92 52 04 02 72 01 82 43 62 01 92 B3\n62 01 FD 27 E2 B2 28 00 1E 28 2B D3 1C 3C 4C 10\n1A 3C 82 43 62 01 92 B3 62 01 FD 27 E2 B2 28 00\n01 28 1B E3 1B B3 01 24 2B D3 C9 EC 12 02 19 E3\n0A 3C 82 43 62 01 92 B3 62 01 FD 27 E2 B2 28 00\nE6 2B 4C 10 1B E3 92 52 02 02 72 01 D2 53 09 02\nC0 3F 82 43 62 01 92 B3 62 01 FD 27 E2 B2 28 00\n01 2C 2B D3 30 41\nq";

#define ACTION_ERASE_CHECK 0x04
#define BSL_SYNC 0x80

// #define ADDR_DEF 0x0220
// #define ADDR_DEF_C "0220"

#define MAXDATA 224

PAD6BootStrapLoader bslobj;
SerialFTD serialFtd;
Port port = {0, "Prattley Auto Drafter 6"};

int main(void) {


    unsigned long bytesWritten;
    unsigned char* rxFrame = malloc(1);
    unsigned char* txFrame = "K";

    bslobj.serialFtd = &serialFtd;

    initBSLobj();
    initLowLevel();
    

    comInitK(port, 300, false);

    // bslReset(true);

    bytesWritten = write_to_ftdi(txFrame, 1);

    printf("Bytes Written = %d\n", bytesWritten);

    rxFrame = read_from_ftdi(1);

    printf("rxFrame = %s\n", rxFrame);

    FT_SetBitMode(serialFtd.ftHandle, 0x72, 0x20);

    printf("FT_HANDLE @Main = %d\n", serialFtd.ftHandle);


    // setRSTpin(1);
    // setTESTpin(0);

    comDone();

    // Hardware Connection Bellow

    // comInitK(port, 0, true);
    // bslReset(true);

    // unsigned char* s = "\x80\x20\x04\x04\x80\x8c\x02\x00\x00\x00";
    // // unsigned char x[] = {0x80, 0x20, 0x04, 0x04, 0x80, 0x8c, 0x02, 0x00, 0x00, 0x00};

    // // printf("%s\n", s);
    // // printf("%s\n", x);

    // bslobj.serialFtd.parity = PARITY_EVEN;
    // bslobj.serialFtd.baudrate = 9600;
    // bslReset(true);
    // sleep(0.10);

    // bslSync();

    // write_to_ftdi(s, strlen(s));

    // rxFrame = read_from_ftdi(1);

    // printf("rxFrame = %s\n", rxFrame);


    // unsigned char* tempData = malloc(1);
    // // int pstart=0;

    // Memory segments;
    // Segment* seg;
    // segments.segs = seg;
    // segments.numSegments=0;

    // loadTIText(F1X_BSL, segments);

    // tempData = realloc(tempData, MAXDATA);

    // memcpy(tempData, segments.segs[0].data+MAXDATA, MAXDATA);

    // printf("%s\n", tempData);

    // preparePatch();
    
}