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
    pContext->dataBufIndex = -1;
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

    for (int i = 0; i <= pContext->dataBufIndex; ++i)
    {
        for (int k = 0; k < pContext->dataBufList[i]->bcsNum; ++k)
        {
            int startPos = pContext->dataBufList[i]->bcsay.bcs[k].bufpos;
            int endPos;
            if (k + 1 < pContext->dataBufList[i]->bcsNum)
            {
                endPos = pContext->dataBufList[i]->bcsay.bcs[k + 1].bufpos;
            }
            else
            {
                endPos = pContext->dataBufList[i]->bufLen;
            }
            for (int l = startPos; l < endPos; l++)
            {
                //printf("%c", pContext->dataBufList[i]->buf[l]);
                printf("%c", pContext->dataBufList[i]->bufStart[l]);
            }
            //printf("dataBufIndex = %d, bcs_type = %d\n", i, pContext->dataBufList[i]->bcsay.bcs[k].bt);
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
            else if (event->type == ETAG)
            {
                printf("EE ");
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
            else if (event->type == CD)
            {
                printf("CD ");
                for (int m = event->startPos; m < event->stopPos; m++)
                {
                    printf("%c", dataBuf->buf[m]);
                }
                printf(" ");
            }
            else if (event->type == COMMENT)
            {
                printf("C ");
                for (int m = event->startPos; m < event->stopPos; m++)
                {
                    printf("%c", dataBuf->buf[m]);
                }
                printf(" ");
            }
            else if (event->type == CDATA)
            {
                printf("CDATA "); // how to sign
                for (int m = event->startPos; m < event->stopPos; m++)
                {
                    printf("%c", dataBuf->buf[m]);
                }
                printf(" ");
            }
        }
    }
}

void findLastBcs(XmlParserContext *pContext, char **dataBufStart, int *offset, int *bcsLen, DataBuf **lastDataBuf)
{
    int nowDataBuf = pContext->dataBufIndex;
    DataBuf *dataBuf = pContext->dataBufList[nowDataBuf];
    *lastDataBuf = dataBuf;
    int bcsIndex = dataBuf->bcsNum - 1;
    Bcs *bcs = &(dataBuf->bcsay.bcs[bcsIndex]);
    *dataBufStart = dataBuf->bufStart; //knowledge amend pointer with double pointer
    *offset = bcs->bufpos;
    *bcsLen = dataBuf->bufLen - bcs->bufpos;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("2 parameters are required\nUsage: ./hpXmlParser filename\n");
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
    char *buf = (char *)malloc(2 * BUFLEN);
    char *firstPartBuf = buf;
    char *secondPartBuf = &(buf[BUFLEN]);

    while (it < size)
    {
        if (pContext->dataBufIndex == -1)
        {
            DataBuf *dataBuf = (DataBuf *)malloc(sizeof(DataBuf));
            //dataBuf->bufnum = -1;
            dataBuf->bcsNum = 0;
            dataBuf->eventIndex = 0;
            dataBuf->bufStart = dataStart; //point at the global position
            //printf("%p %p\n", dataBuf->bufStart, dataStart);
            if (size - it < BUFLEN)
            {
                memcpy(secondPartBuf, dataStart, size - it);
                preprocess(secondPartBuf, dataBuf, size - it, 0);
                dataStart += (size - it);
                dataBuf->dataBufLen = size - it;
                dataBuf->bufLen = size - it;
                it += (size - it);
            }
            else
            {
                memcpy(secondPartBuf, dataStart, BUFLEN);
                preprocess(secondPartBuf, dataBuf, BUFLEN, 0);
                it += BUFLEN;
                dataBuf->dataBufLen = BUFLEN;
                dataBuf->bufLen = BUFLEN;
                dataStart += BUFLEN;
            }
            //preprocess(secondPartBuf, dataBuf, size - it);
            pContext->dataBufList[++pContext->dataBufIndex] = dataBuf;
            //printf("%p %p\n", dataBuf->bufStart, dataStart);
            //break;
        }
        else
        {
            //printf("dataStart = %s\n", dataStart);
            DataBuf *dataBuf = (DataBuf *)malloc(sizeof(DataBuf));
            //dataBuf->bufnum = -1;
            dataBuf->bcsNum = 0;
            dataBuf->eventIndex = 0;
            dataBuf->bufStart = dataStart; //point at the global position
                                           //printf("%p %p\n", dataBuf->bufStart, dataStart);
            char *dataBufStart;
            int offset, bcsLen;
            DataBuf *lastDataBuf;
            //assert the xml one data less than fixed size
            findLastBcs(pContext, &dataBufStart, &offset, &bcsLen, &lastDataBuf);
            // printf("offset = %d, bcsLen = %d ", offset, bcsLen);
            //printf("dataBufStart = %s\n", dataBufStart);
            if (size - it < BUFLEN)
            {
                // printf("ok!!!\n");
                memset(secondPartBuf, 0, BUFLEN);
                memcpy(secondPartBuf, dataStart, size - it);
                memcpy(&firstPartBuf[BUFLEN - bcsLen], &dataBufStart[offset], bcsLen);
                // printf("firstPartBuf = %s\n", &firstPartBuf[BUFLEN - bcsLen]);
                preprocess(&firstPartBuf[BUFLEN - bcsLen], dataBuf, size - it + bcsLen, bcsLen);
                // preprocess(secondPartBuf, dataBuf, size - it);
                dataStart += (size - it);
                dataBuf->dataBufLen = size - it;
                dataBuf->bufLen = size - it;
                it += (size - it);
            }
            else
            {
                assert(0);
                // memcpy(secondPartBuf, dataStart, BUFLEN);
                // preprocess(secondPartBuf, dataBuf, BUFLEN);
                // it += BUFLEN;
                // dataBuf->dataBufLen = BUFLEN;
                // dataBuf->bufLen = size - it;
                // dataStart += BUFLEN;
            }
            //preprocess(secondPartBuf, dataBuf, size - it);
            pContext->dataBufList[++pContext->dataBufIndex] = dataBuf;
            //printf("%p %p\n", dataBuf->bufStart, dataStart);
            //break;
        }
        // else
        // {
        //     DataBuf *dataBuf = (DataBuf *)malloc(sizeof(DataBuf));
        //     dataBuf->bufnum = 0;
        //     dataBuf->eventIndex = 0;
        //     char *dataBufStart;
        //     int *offset, *bcsLen;
        //     //assert the xml one data less than fixed size
        //     findLastBcs(pContext, dataBufStart, offset, bcsLen);
        //     if (size - it < BUFLEN)
        //     {
        //         memcpy(secondPartBuf, dataStart, size - it);
        //         memcpy(&firstPartBuf[BUFLEN - bcsLen], dataBufStart, bcsLen);
        //     }
        //     else
        //     {
        //         memcpy(secondPartBuf, dataStart, BUFLEN);
        //         memcpy(&firstPartBuf[BUFLEN - bcsLen], dataBufStart, bcsLen);
        //     }
        //     preprocess(secondPartBuf, dataBuf, size, &it);
        //     it += (size - it);
        //     pContext->dataBufList[++pContext->dataBufIndex] = dataBuf;
        // }

        // DataBuf *dataBuf = (DataBuf *)malloc(sizeof(DataBuf));
        // dataBuf->bufnum = 0;
        // dataBuf->eventIndex = 0;

        // memset(dataBuf, 0, sizeof(DataBuf));
        // if (size - it < BUFLEN)
        // {
        //     preprocess(dataStart, dataBuf, size, &it);
        //     dataBuf->bufLen = size - it;
        //     dataBuf->bufStart = dataStart;
        //     dataBuf->dataBufLen = size - it;
        //     dataBuf->startOffset = it;
        //     //parseEvents(dataBuf, size - it); //TODO second parameter could be deleted
        //     it += (size - it);
        //     //dataStart += (size - it);
        //     printf("there\n");
        // }
        // else //process one whole block data
        // {
        //     preprocess(dataStart, dataBuf, BUFLEN, &it);
        //     dataBuf->bufLen = BUFLEN;
        //     dataBuf->bufStart = dataStart;
        //     dataBuf->dataBufLen = size - it;
        //     dataBuf->startOffset = it;

        //     //parseEvents(dataBuf, BUFLEN);
        //     it += BUFLEN;
        //     //dataStart += BUFLEN;
        //     printf("here\n");
        // }
        // pContext->dataBufList[pContext->dataBufIndex++] = dataBuf;
    }

    for (int i = 0; i <= pContext->dataBufIndex; i++)
    {
        printf("%d bcsNum = %d\n", i, pContext->dataBufList[i]->bcsNum);
    }
    // printf("bcsNum = %d\n", pContext->dataBufList[0]->bcsNum);
    traverseDataBuf(pContext);

    // int i = 0, k = 4;
    // int startPos = pContext->dataBufList[i]->bcsay.bcs[k].bufpos;
    // int endPos;
    // if (k + 1 <= pContext->dataBufList[i]->bcsNum)
    // {
    //     endPos = pContext->dataBufList[i]->bcsay.bcs[k + 1].bufpos;
    // }
    // else
    // {
    //     endPos = pContext->dataBufList[i]->bufLen;
    // }
    // for (int l = startPos; l < endPos; l++)
    // {
    //     //printf("%c", pContext->dataBufList[i]->buf[l]);
    //     printf("%c", pContext->dataBufList[i]->bufStart[l]);
    // }

    //traverseEventStream(pContext);
    //freeResource(pContext);

    return 0;
}