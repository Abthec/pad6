

#ifndef MEMORY_H
#define MEMORY_H

#define DEBUG 0

typedef struct {
    char* data;
    unsigned long startAddress;
} Segment;

typedef struct {
    Segment* segments;
    int numSegments;
} Memory;



#endif /* MEMORY_H */