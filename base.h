#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#define BUFLEN 1024
#define TRUE 1
#define FALSE 0
#define INCOMPLETE 2
//typedef int Bool;
#define MAX_DATA_BUF_NUM 102400

static int errorState = 0; //if not set to static, will occur multiple definitions

typedef enum ErrorStatusType {
    WRONGSTARTCHAR,
    WRONGSPACE,
    WRONGATTRIBUTE,
    WRONGEQUALS,
    WRONGENTITYREF
} ErrorStatusType;

typedef enum EventType { //ignore the empty type
    STAG,
    NAME,
    ATTRIBUTE,
    PI,
    COMMENT,
    CDATA,
    CONTENT,
    ETAG,
} EventType;

typedef struct Event
{
    int bcsNum;
    int startPos;
    int stopPos;
    int len;
    int startPos2;
    int stopPos2;
    // char* eventData;
    // int offset;
    // int eventLen;
    // int startPos;
    // int incomplete;
    EventType type;
} Event;

typedef enum Bcstype {
    StagorEmptytag_start,
    Etag_start,
    PI_start,
    //Content,
    CDSECT_start,
    COMMENT_start
} Bcstype;

typedef struct Bcs
{
    //int fileoffset;
    int bufnum;
    int bufpos;
    Bcstype bt;
} Bcs;

typedef struct Bcsarray
{
    Bcs bcs[BUFLEN / 10];
    //struct Bcsarray *pre, *next;
} Bcsarray;

// typedef struct EventStream
// {
//     Event event[BUFLEN / 10];
// } EventStream;

typedef struct DataBuf
{
    char buf[BUFLEN];
    Bcsarray bcsay;
    int bufnum; //start from 0
    int bufLen; //start from 1
    //EventStream eventStream;
    int eventIndex;
    Event eventStream[BUFLEN / 10];
    // int FINISH_STAGE0;
    // int START_STAGE0;
    // int FINISH_STAGE1;
    // int FINISH_STAGE2;
    // int FINISH_STAGE3;
    // int START_STAGE1;
    // int START_STAGE2;
    // int START_STAGE3;
    //struct DataBuf *next;
} DataBuf;

typedef struct XmlParserContext
{
    //FILE *XmlFileIn;
    long xmlFileInSize;
    char *buf;
    DataBuf *dataBufList[MAX_DATA_BUF_NUM];
    int dataBufIndex;
} XmlParserContext;
#endif