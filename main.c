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
    //printf("bufnum = %d\n", pContext->dataBufList[0]->bufnum);
    // if (dataBuf->bcsay.bcs[i].bt == COMMENT_start)
    // {
    //     printf("%d, %d\n", dataBuf->bcsay.bcs[i].bufpos, dataBuf->bcsay.bcs[i + 1].bufpos);
    //     for (int j = dataBuf->bcsay.bcs[i].bufpos; j < dataBuf->bcsay.bcs[i + 1].bufpos; ++j)
    //     {
    //         printf("%c", dataBuf->buf[j]);
    //     }
    //     break;
    // }
    //printf("%d\n", dataBuf->bcsay.bcs[i].bufpos);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("2 argument is necessray\n");
        exit(1);
    }
    //FILE* fp = fopen(argv[1], "r");
    //long size = fileSize(fp);
    //char* data = (char*)malloc(size * sizeof(char));
    //fread(data, (unsigned int)size, 1, fp);
    //fclose(fp);
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
        { //处理一整块的数据
            preprocess(dataStart, dataBuf, BUFLEN);
            dataBuf->bufLen = BUFLEN;
            parseEvents(dataBuf, BUFLEN);
            it += BUFLEN;
            dataStart += BUFLEN;
            //break;  //simply for test
        }
        pContext->dataBufList[pContext->dataBufIndex++] = dataBuf;
    }
    printf("dataBufIndex = %d\n", pContext->dataBufIndex);
    // for(int i = 0; i <= pContext->dataBufIndex; i++){
    //     parseEvents(pContext->dataBufList[i], BUFLEN);
    //     break;//TODO how to get every dataBuf length, or to put it right after preprocess
    // }

    //traverseDataBuf(pContext);
    freeResource(pContext);
    return 0;
}