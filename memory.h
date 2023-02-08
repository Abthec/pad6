#ifndef MEMORY_H
#define MEMORY_H

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

#define DEBUG 0

typedef struct {
    unsigned char* data;
    unsigned char* startAddress;
    int numBytes;
} Segment;

typedef struct {
    Segment* segs;
    int numSegments;
} Memory;

void loadTIText(unsigned char* file, Memory segments);

#endif /* MEMORY_H */