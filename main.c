#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
//#include<stdint.h>
#include "base.h"
#include "util.h"
#include "preprocess.h"
#include "parse.h"

XmlParserContext *readXmlFile(char *fileName)
{
    XmlParserContext *pContext = (XmlParserContext *)malloc(sizeof(XmlParserContext));
    if (pContext == NULL)
    {
        return NULL;
    }
    FILE *fp = fopen(fileName, "r");
    pContext->xmlFileInSize = fileSize(fp);
    pContext->dataBufIndex = 0;
    //long size = fileSize(fp);
    pContext->buf = (char *)malloc(pContext->xmlFileInSize * sizeof(char));
    fread(pContext->buf, pContext->xmlFileInSize, 1, fp);
    fclose(fp);
    return pContext;
}

void freeResource(XmlParserContext *pContext)
{
    if (pContext == NULL)
    {
        return;
    }
    free(pContext->buf);
    free(pContext);
}

void traverseDataBuf(XmlParserContext *pContext)
{

    for (int i = 0; i < pContext->dataBufIndex; ++i)
    {
        for (int k = 0; k < pContext->dataBufList[i]->bufnum; ++k)
        {
            int startPos = pContext->dataBufList[i]->bcsay.bcs[k].bufpos;
            int endPos;
            if (k + 1 < pContext->dataBufList[i]->bufnum)
            {
                endPos = pContext->dataBufList[i]->bcsay.bcs[k + 1].bufpos;
            }
            else
            {
                endPos = pContext->dataBufList[i]->bufLen;
            }
            for (int l = startPos; l < endPos; l++)
            {
                printf("%c", pContext->dataBufList[i]->buf[l]);
            }
        }
    }
}
void traverseEventStream(XmlParserContext *pContext)
{
    for (int i = 0; i < pContext->dataBufIndex; ++i)
    {
        for (int k = 0; k < pContext->dataBufList[i]->eventIndex; ++k)
        {
            DataBuf *dataBuf = pContext->dataBufList[i];
            Event *event = &(dataBuf->eventStream[k]);
            if (event->type == STAG)
            {
                printf("SE ");
                for (int m = event->startPos; m < event->stopPos; m++)
                {
                    printf("%c", dataBuf->buf[m]);
                }
                printf(" ");
            }
            else if (event->type == ATTRIBUTE)
            {
                printf("A ");
                for (int m = event->startPos; m < event->stopPos; m++)
                {
                    printf("%c", dataBuf->buf[m]);
                }
                printf(" ");
                for (int m = event->startPos2; m < event->stopPos2; m++)
                {
                    printf("%c", dataBuf->buf[m]);
                }
                printf(" ");
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("2 argument is necessray\n");
        exit(1);
    }
    XmlParserContext *pContext = readXmlFile(argv[1]);
    if (pContext == NULL)
    {
        exit(2);
    }
    char *dataStart = pContext->buf;
    long size = pContext->xmlFileInSize;
    int it = 0;
    while (it < size)
    {
        DataBuf *dataBuf = (DataBuf *)malloc(sizeof(DataBuf));
        dataBuf->bufnum = 0;
        dataBuf->eventIndex = 0;
        memset(dataBuf, 0, sizeof(DataBuf));
        if (size - it < BUFLEN)
        {
            preprocess(dataStart, dataBuf, size - it);
            dataBuf->bufLen = size - it;
            parseEvents(dataBuf, size - it); //second parameter could be deleted
            it += (size - it);
            dataStart += (size - it);
            //printf("dataBuf bufnum = %d\n", dataBuf->bufnum);
        }
        else
        { //process one whole block data
            preprocess(dataStart, dataBuf, BUFLEN);
            dataBuf->bufLen = BUFLEN;
            parseEvents(dataBuf, BUFLEN);
            it += BUFLEN;
            dataStart += BUFLEN;
            //break;  //simply for test
        }
        pContext->dataBufList[pContext->dataBufIndex++] = dataBuf;
    }
    //printf("dataBufIndex = %d\n", pContext->dataBufIndex);

    //printf("eventIndex = %d\n", pContext->dataBufList[0]->eventIndex);

    //traverseDataBuf(pContext);

    traverseEventStream(pContext);
    freeResource(pContext);

    return 0;
}