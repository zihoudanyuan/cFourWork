#ifndef BASE_H
#define BASE_H

#include<stdio.h>
#define BUFLEN 1024
#define TRUE 1
#define FALSE 0
//typedef int Bool;
#define MAX_DATA_BUF_NUM 102400
typedef enum EventType{
    PI,
    COMMENT,
    CDATA,
    CONTENT
}EventType;

typedef struct Event{
    char* eventData;
    int offset;
    int eventLen;
    int startPos;
    EventType type;
}Event;

typedef enum Bcstype{
    StagorEmptytag_start,
    Etag_start,
    PI_start,
    //Content,
    CDSECT_start,
    COMMENT_start
}Bcstype;

typedef struct Bcs{
    //int fileoffset;
    //char* bcsStart;
    int bufnum;
    int bufpos;
    Bcstype bt;
}Bcs;

typedef struct Bcsarray{
    Bcs bcs[BUFLEN/10];
    //struct Bcsarray *pre, *next;
}Bcsarray;

typedef struct DataBuf{
    char buf[BUFLEN];
    Bcsarray bcsay;
    int bufnum;
    // int FINISH_STAGE0;
    // int START_STAGE0;
    // int FINISH_STAGE1;
    // int FINISH_STAGE2;
    // int FINISH_STAGE3;
    // int START_STAGE1;
    // int START_STAGE2;
    // int START_STAGE3;
    //struct DataBuf *next;
}DataBuf;

typedef struct XmlParserContext{
    //FILE *XmlFileIn;
    long xmlFileInSize;
    char *buf;
    DataBuf *dataBufList[MAX_DATA_BUF_NUM];
    int dataBufIndex;
}XmlParserContext;
#endif