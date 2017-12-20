#include "preprocess.h"

void preprocessEndTag(DataBuf *dataBuf, int *pos)
{
    int bufNum = dataBuf->bufnum++;
    dataBuf->bcsay.bcs[bufNum].bt = Etag_start;
    dataBuf->bcsay.bcs[bufNum].bufpos = (*pos)++;
    // printf("pos = %d\n", *pos - 1);
}

void preprocessStartOrEmpty(DataBuf *dataBuf, int *pos)
{
    int bufNum = dataBuf->bufnum++;
    dataBuf->bcsay.bcs[bufNum].bt = StagorEmptytag_start;
    dataBuf->bcsay.bcs[bufNum].bufpos = (*pos)++;
    //printf("bufpos = %d\n", dataBuf->bcsay.bcs[bufNum].bufpos);
}

//<!--This element indicates whether age<40.-->
//CDATA 部分由 "<![CDATA[" 开始，由 "]]>" 结束：
void preprocessCommentCDATA(DataBuf *dataBuf, int *pos, int len)
{
    int bufNum = dataBuf->bufnum++;
    //(*pos)++;
    int i = *pos + 1;
    char *data = dataBuf->buf;
    assert(data[i] == '!');
    if (i + 2 < len && data[i + 1] == '-' && data[i + 2] == '-') //COMMENT
    {
        i += 3;
        //(*pos) = i;
        assert(i < len); //TODO make sure about it
        dataBuf->bcsay.bcs[bufNum].bt = COMMENT_start;
        dataBuf->bcsay.bcs[bufNum].bufpos = *pos;
        while (i + 3 < len && !(data[i++] == '-' && data[i++] == '-' && data[i++] == '>'))
            ;
        if (i == len)
        {
            //assert(0);
            unFinishedType = COMMENT_start;
            return;
        }
        else
        {
            *pos = i;
        }
    }
    else if (i + 7 < len && data[i + 1] == '[' && data[i + 2] == 'C' && data[i + 3] == 'D' && data[i + 4] == 'A' && data[i + 5] == 'T' && data[i + 6] == 'A' && data[i + 7] == '[') //[CDATA[
    {
        i += 8;
        //(*pos) = i;
        assert(i < len);
        dataBuf->bcsay.bcs[bufNum].bt = CDSECT_start;
        dataBuf->bcsay.bcs[bufNum].bufpos = *pos;

        while (i + 3 < len && !(data[i++] == ']' && data[i++] == ']' && data[i++] == '>'))
            ;

        if (i == len)
        { //TODO amend this extreme condition
            //assert(0);
            unFinishedType = CDSECT_start;
            return;
        }
        else
        {
            *pos = i;
        }
    }
}

//<?page render multiple authors ?>
void preprocessPI(DataBuf *dataBuf, int *pos, int len)
{
    int bufNum = dataBuf->bufnum++;
    //(*pos)++;
    int i = *pos + 1;
    char *data = dataBuf->buf;
    assert(data[i] == '?');
    if (i < len && data[i] == '?')
    {
        i += 1;
        //(*pos)++;
        dataBuf->bcsay.bcs[bufNum].bt = PI_start;
        dataBuf->bcsay.bcs[bufNum].bufpos = *pos; //PI
        while (i + 2 < len && !(data[i++] == '?' && data[i++] == '>'))
            ;
        if (i == len)
        { //TODO amend this extreme condition
            //assert(0);
            unFinishedType = PI_start;
        }
        else
        {
            *pos = i;
        }
    }
}

void preprocess(char *data, DataBuf *dataBuf, int len, int *start)
{
    memcpy(dataBuf->buf, data, len);
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
        if (data[i] == '<')
        {
            //assert(i + 1 != len);
            if (i + 1 >= len)
            {
            }
            switch (data[i + 1])
            {
            case '/': //endtag
            {
                preprocessEndTag(dataBuf, &i);
                break;
            }
            case '?': //start PI
            {
                preprocessPI(dataBuf, &i, len);
                break;
            }
            case '!': //comment or cdata
            {
                preprocessCommentCDATA(dataBuf, &i, len);
                break;
            }
            default: //start or empty
            {
                preprocessStartOrEmpty(dataBuf, &i);
            }
            }
        }
        else
        {
            i++;
        }
    }
}