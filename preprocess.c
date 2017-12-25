#include "preprocess.h"

void preprocessStartOrEmptyComplete(DataBuf *dataBuf, int *pos, int len, char *data, int fpLen, DataBuf *lastDataBuf)
{
    int i = *pos;
    while (i + 1 < len && data[i + 1] != '<')
    {
        i++;
    }
    if (*pos < fpLen)
    {
        lastDataBuf->bcsay.bcs[lastDataBuf->bcsNum - 1].bcsLen = i - *pos;
    }
    else if (*pos == fpLen && lastDataBuf != NULL)
    {
        int bcsNum = lastDataBuf->bcsNum++;
        lastDataBuf->bcsay.bcs[bcsNum].bt = StagorEmptytag_start;
        lastDataBuf->bcsay.bcs[bcsNum].bufpos = (*pos - fpLen);
        lastDataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
    }
    else
    {
        int bcsNum = dataBuf->bcsNum++;
        dataBuf->bcsay.bcs[bcsNum].bt = StagorEmptytag_start;
        dataBuf->bcsay.bcs[bcsNum].bufpos = (*pos - fpLen);
        dataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
    }
    *pos = i;
}

void preprocessEndTagComplete(DataBuf *dataBuf, int *pos, int len, char *data, int fpLen, DataBuf *lastDataBuf)
{
    int i = *pos;
    while (i + 1 < len && data[i + 1] != '<')
    {
        i++;
    }
    if (*pos < fpLen)
    {
        lastDataBuf->bcsay.bcs[lastDataBuf->bcsNum - 1].bcsLen = i - *pos;
    }
    else if (*pos == fpLen && lastDataBuf != NULL)
    {
        int bcsNum = lastDataBuf->bcsNum++;
        lastDataBuf->bcsay.bcs[bcsNum].bt = Etag_start;
        lastDataBuf->bcsay.bcs[bcsNum].bufpos = (*pos - fpLen);
        lastDataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
    }
    else
    {
        int bcsNum = dataBuf->bcsNum++;
        dataBuf->bcsay.bcs[bcsNum].bt = Etag_start;
        dataBuf->bcsay.bcs[bcsNum].bufpos = (*pos - fpLen);
        dataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
    }
    *pos = i;
}

void preprocessCommentCDATAComplete(DataBuf *dataBuf, int *pos, int len, char *data, int fpLen, DataBuf *lastDataBuf)
{
    //int bufNum = dataBuf->bufnum;
    //(*pos)++;
    int i = *pos + 1; //pos points at <
    //char *data = dataBuf->bufStart;
    //assert(data[i] == '!');
    //if (i + 2 < len && data[i + 1] == '-' && data[i + 2] == '-') //COMMENT
    if (i + 1 < len && data[i + 1] == '-')
    {
        i += 3;
        //(*pos) = i;
        // while (i + 3 < len && !(data[i++] == '-' && data[i++] == '-' && data[i++] == '>'))
        //     ;
        // if (*pos >= fpLen)
        // {
        //     int bcsNum = dataBuf->bcsNum++;
        //     dataBuf->bcsay.bcs[bcsNum].bt = COMMENT_start;
        //     dataBuf->bcsay.bcs[bcsNum].bufpos = *pos - fpLen; //dataBuf inner offset
        // }

        while (i + 2 < len && !(data[i] == '-' && data[i + 1] == '-' && data[i + 2] == '>'))
        {
            i++;
        }
        if (*pos < fpLen)
        {
            lastDataBuf->bcsay.bcs[lastDataBuf->bcsNum - 1].bcsLen = i - *pos;
        }
        else if (*pos == fpLen && lastDataBuf != NULL)
        {
            int bcsNum = lastDataBuf->bcsNum++;
            lastDataBuf->bcsay.bcs[bcsNum].bt = COMMENT_start;
            lastDataBuf->bcsay.bcs[bcsNum].bufpos = (*pos - fpLen);
            lastDataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
        }
        else
        {
            i += 2;
            int bcsNum = dataBuf->bcsNum++;
            dataBuf->bcsay.bcs[bcsNum].bt = COMMENT_start;
            dataBuf->bcsay.bcs[bcsNum].bufpos = *pos - fpLen; //dataBuf inner offset
            dataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
        }
        *pos = i;
    }
    // else if (i + 7 < len && data[i + 1] == '[' && data[i + 2] == 'C' && data[i + 3] == 'D' && data[i + 4] == 'A' && data[i + 5] == 'T' && data[i + 6] == 'A' && data[i + 7] == '[') //[CDATA[
    else if (i + 1 < len && data[i + 1] == '[')
    {
        i += 8;
        //(*pos) = i;
        // if (*pos >= fpLen)
        // {
        //     int bcsNum = dataBuf->bcsNum++;
        //     dataBuf->bcsay.bcs[bcsNum].bt = CDSECT_start;
        //     dataBuf->bcsay.bcs[bcsNum].bufpos = *pos - fpLen;
        // }

        // while (i + 3 < len && !(data[i++] == ']' && data[i++] == ']' && data[i++] == '>'))
        //     ;
        while (i + 2 < len && !(data[i] == ']' && data[i + 1] == ']' && data[i + 2] == '>'))
        {
            i++;
        }
        if (*pos < fpLen)
        {
            lastDataBuf->bcsay.bcs[lastDataBuf->bcsNum - 1].bcsLen = i - *pos;
        }
        else if (*pos == fpLen && lastDataBuf != NULL)
        {
            int bcsNum = lastDataBuf->bcsNum++;
            lastDataBuf->bcsay.bcs[bcsNum].bt = CDSECT_start;
            lastDataBuf->bcsay.bcs[bcsNum].bufpos = (*pos - fpLen);
            lastDataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
        }
        else
        {
            i += 2;
            int bcsNum = dataBuf->bcsNum++;
            dataBuf->bcsay.bcs[bcsNum].bt = CDSECT_start;
            dataBuf->bcsay.bcs[bcsNum].bufpos = *pos - fpLen;
            dataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
        }
        *pos = i;
    }
    else
    {
        *pos = i; //if can't distinguish comment and cdata, then we will not treat this as bcs.
        // This happens when '<!' is at the end of databuf.Ignore the Wrong situation.
    }
}

void preprocessPIComplete(DataBuf *dataBuf, int *pos, int len, char *data, int fpLen, DataBuf *lastDataBuf)
{

    int i = *pos + 1;
    while (i + 2 < len && !(data[i + 1] == '?' && data[i + 2] == '>'))
    {
        i++;
    }
    if (*pos < fpLen)
    {
        lastDataBuf->bcsay.bcs[lastDataBuf->bcsNum - 1].bcsLen = i - *pos;
    }
    else if (*pos == fpLen && lastDataBuf != NULL)
    {
        int bcsNum = lastDataBuf->bcsNum++;
        lastDataBuf->bcsay.bcs[bcsNum].bt = PI_start;
        lastDataBuf->bcsay.bcs[bcsNum].bufpos = (*pos - fpLen);
        lastDataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
    }
    else
    {
        i += 2;
        int bcsNum = dataBuf->bcsNum++;
        dataBuf->bcsay.bcs[bcsNum].bt = PI_start;
        dataBuf->bcsay.bcs[bcsNum].bufpos = (*pos - fpLen); //PI
        dataBuf->bcsay.bcs[bcsNum].bcsLen = i - *pos;
    }
    *pos = i;
}

void preprocessEndTag(DataBuf *dataBuf, int *pos)
{
    int bcsNum = dataBuf->bcsNum++;
    dataBuf->bcsay.bcs[bcsNum].bt = Etag_start;
    dataBuf->bcsay.bcs[bcsNum].bufpos = (*pos)++;
    // printf("pos = %d\n", *pos - 1);
}

void preprocessStartOrEmpty(DataBuf *dataBuf, int *pos)
{
    int bcsNum = dataBuf->bcsNum++;
    dataBuf->bcsay.bcs[bcsNum].bt = StagorEmptytag_start;
    dataBuf->bcsay.bcs[bcsNum].bufpos = (*pos)++;
    //printf("bufpos = %d\n", dataBuf->bcsay.bcs[bcsNum].bufpos);
}

//<!--This element indicates whether age<40.-->
//CDATA 部分由 "<![CDATA[" 开始，由 "]]>" 结束：
void preprocessCommentCDATA(DataBuf *dataBuf, int *pos, int len, char *data)
{
    //int bufNum = dataBuf->bufnum;
    //(*pos)++;
    int i = *pos + 1; //pos points at <
    //char *data = dataBuf->bufStart;
    //assert(data[i] == '!');
    //if (i + 2 < len && data[i + 1] == '-' && data[i + 2] == '-') //COMMENT
    if (i + 1 < len && data[i + 1] == '-')
    {
        i += 3;
        //(*pos) = i;
        // while (i + 3 < len && !(data[i++] == '-' && data[i++] == '-' && data[i++] == '>'))
        //     ;
        int bcsNum = dataBuf->bcsNum++;
        dataBuf->bcsay.bcs[bcsNum].bt = COMMENT_start;
        dataBuf->bcsay.bcs[bcsNum].bufpos = *pos; //dataBuf inner offset

        while (i + 2 < len && !(data[i] == '-' && data[i + 1] == '-' && data[i + 2] == '>'))
        {
            i++;
        }

        *pos = i;
    }
    // else if (i + 7 < len && data[i + 1] == '[' && data[i + 2] == 'C' && data[i + 3] == 'D' && data[i + 4] == 'A' && data[i + 5] == 'T' && data[i + 6] == 'A' && data[i + 7] == '[') //[CDATA[
    else if (i + 1 < len && data[i + 1] == '[')
    {
        i += 8;
        //(*pos) = i;
        int bcsNum = dataBuf->bcsNum++;
        dataBuf->bcsay.bcs[bcsNum].bt = CDSECT_start;
        dataBuf->bcsay.bcs[bcsNum].bufpos = *pos;

        // while (i + 3 < len && !(data[i++] == ']' && data[i++] == ']' && data[i++] == '>'))
        //     ;
        while (i + 2 < len && !(data[i] == ']' && data[i + 1] == ']' && data[i + 2] == '>'))
        {
            i++;
        }

        *pos = i;
    }
    else
    {
        *pos = i; //if can't distinguish comment and cdata, then we will not treat this as bcs.
        // This happens when '<!' is at the end of databuf.Ignore the Wrong situation.
    }
}
//<?page render multiple authors ?>
void preprocessPI(DataBuf *dataBuf, int *pos, int len, char *data)
{
    int bcsNum = dataBuf->bcsNum++;
    dataBuf->bcsay.bcs[bcsNum].bt = PI_start;
    dataBuf->bcsay.bcs[bcsNum].bufpos = *pos; //PI
    //(*pos)++;
    int i = *pos + 1;
    //char *data = dataBuf->buf;
    //assert(data[i] == '?');
    // if (i < len && data[i] == '?')
    // {
    // i += 1;
    //(*pos)++;
    // while (i + 2 < len && !(data[i++] == '?' && data[i++] == '>'))
    //     ;
    while (i + 2 < len && !(data[i + 1] == '?' && data[i + 2] == '>'))
    {
        i++;
    }
    // if (i == len)
    // { //TODO amend this extreme condition
    //     //assert(0);
    //     unFinishedType = PI_start;
    // }
    // else
    // {
    *pos = i;
    // }
    // }
}

void preprocess(char *data, DataBuf *dataBuf, int len, int fpLen, DataBuf *lastDataBuf)
{
    //memcpy(dataBuf->buf, data, len);
    int i = 0;
    //handle last unFinished element
    // if (0 != unFinishedType)
    // {
    //     if (unFinishedType == COMMENT_start)
    //     {
    //         while (i + 3 < len && !(data[i++] == '-' && data[i++] == '-' && data[i++] == '>'))
    //             ;
    //         if (i == len)
    //         {
    //             //assert(0);
    //             //unFinishedType = COMMENT_start;
    //             return;
    //         }
    //         else
    //         {
    //             unFinishedType = 0; //TODO rename to a constant variable
    //         }
    //     }
    //     else if (unFinishedType == CDSECT_start)
    //     {
    //         while (i + 3 < len && !(data[i++] == ']' && data[i++] == ']' && data[i++] == '>'))
    //             ;
    //         if (i == len)
    //         {
    //             //assert(0);
    //             // unFinishedType = COMMENT_start;
    //             return;
    //         }
    //         else
    //         {
    //             unFinishedType = 0; //TODO rename to a constant variable
    //         }
    //     }
    //     else if (unFinishedType == PI_start)
    //     {
    //         // while (i + 3 < len && !(data[i++] == ']' && data[i++] == ']' && data[i++] == '>'))
    //         //     ;
    //         while (i + 2 < len && !(data[i++] == '?' && data[i++] == '>'))
    //             ;

    //         if (i == len)
    //         {
    //             //assert(0);
    //             // unFinishedType = COMMENT_start;
    //             return;
    //         }
    //         else
    //         {
    //             unFinishedType = 0; //TODO rename to a constant variable
    //         }
    //     }
    // }
    while (i < len)
    {
        if (data[i] == '<') // whether = is needed
        {
            //assert(i + 1 != len);
            if (i + 1 >= len)
            {
                //(i)--;
                break;
            }
            switch (data[i + 1])
            {
            case '/': //endtag
            {
                preprocessEndTagComplete(dataBuf, &i, len, data, fpLen, lastDataBuf);
                // i++;
                break;
            }
            case '?': //start PI
            {
                preprocessPIComplete(dataBuf, &i, len, data, fpLen, lastDataBuf);
                // i++;
                break;
            }
            case '!': //comment or cdata
            {
                preprocessCommentCDATAComplete(dataBuf, &i, len, data, fpLen, lastDataBuf);
                // i++;
                break;
            }
            default: //start or empty
            {
                preprocessStartOrEmptyComplete(dataBuf, &i, len, data, fpLen, lastDataBuf);
            }
            }
        }
        else
        {
            i++;
        }
    }
}