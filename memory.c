#include "memory.h"

void loadTIText(unsigned char* file, Memory segments)
{

    Segment seg;
    int count=0;
    int counter=0;
    int index=0;
    int num=0;
    int numBytes;
    int sixteen=16;

    bool getAddr=false;
    bool gotAddr=false;
    
    unsigned char* addr = malloc(5);
    unsigned char* newFile = malloc(strlen(file));
    unsigned char* tempData = malloc(2);
    unsigned char tempData2[2];

    setlocale(LC_CTYPE, "");

    wchar_t wNum;

    for (int i=0; i<strlen(file); i++) {

        if (file[i] == 'q') {

            break;

        } else if (file[i] == '@') {

            getAddr=true;
            count=0;

        } else if (getAddr) {

            if (!isalnum(file[i])) {

                getAddr=false;
                gotAddr=true;

            } else {

                addr[count] = file[i];
                count++;

            }
        }

        if (gotAddr && isalnum(file[i])) {

            if (counter == 2) {

                num=((tempData[0]-48) + ((tempData[1]-48)*16));
                // printf("%d\n", num);
                sprintf(tempData2, "%c", num);
                num=0;
                printf("%s", tempData2);
                counter=0;

            }

            // printf("%c", file[i]);
            newFile[index] = file[i];
            tempData[counter] = file[i];
            counter++;
            index++;

        }
    }

    addr[count] = '\0';
    newFile[index] = '\0';

    numBytes = strlen(newFile);

    segments.segs[segments.numSegments].data = newFile;
    segments.segs[segments.numSegments].numBytes = strlen(newFile);
    segments.segs[segments.numSegments].startAddress = addr;
    segments.numSegments+=1;

    // printf("%s\n", newFile);

}