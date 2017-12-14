#include"preprocess.h"
#include<stdio.h>
void preprocessEndTag(DataBuf* dataBuf, int* pos)
{
    int bufNum = dataBuf->bufnum++;
    dataBuf->bcsay.bcs[bufNum].bt = Etag_start;
    dataBuf->bcsay.bcs[bufNum].bufpos = (*pos)++;
    printf("pos = %d\n", *pos - 1);
}

void preprocessStartOrEmpty(DataBuf* dataBuf, int* pos)
{
    int bufNum = dataBuf->bufnum++;
    dataBuf->bcsay.bcs[bufNum].bt = StagorEmptytag_start;
    dataBuf->bcsay.bcs[bufNum].bufpos = (*pos)++;
    //printf("bufpos = %d\n", dataBuf->bcsay.bcs[bufNum].bufpos);
}

//<!--This element indicates whether age<40.-->
//CDATA 部分由 "<![CDATA[" 开始，由 "]]>" 结束：
void preprocessCommentCDATA(DataBuf* dataBuf, int* pos, int len)
{
    int bufNum = dataBuf->bufnum++;
    //(*pos)++;
    int i = *pos + 1;
    char* data = dataBuf->buf;
    assert(data[i] == '!');
    if(i+2 < len && data[i+1] == '-' && data[i+2] == '-'){
        i += 3;
        //(*pos) = i;
        assert(i < len);//TODO make sure about it
        dataBuf->bcsay.bcs[bufNum].bt = COMMENT_start;
        dataBuf->bcsay.bcs[bufNum].bufpos = *pos;//COMMENT
        while(i + 3 < len && !(data[i++] == '-' && data[i++] == '-' && data[i++] == '>'));
        *pos = i;
        if(i == len){//TODO amend this extreme condition
            //结尾在之后的数据块
            assert(0);
        }
    }else if(i+6 < len && data[i+1] == '[' && data[i+2] == 'C' && data[i+3] == 'D' && data[i+4] == 'A' && data[i+5] == 'T' && data[i+6] == 'A'){
        i += 7;
        //(*pos) = i;
        assert(i < len);
        dataBuf->bcsay.bcs[bufNum].bt = CDSECT_start;
        dataBuf->bcsay.bcs[bufNum].bufpos = *pos;//[CDATA[

        while(i+3 < len && !(data[i++] == ']' && data[i++] == ']' && data[i++] == '>'));
        *pos = i;
        if(i == len){//TODO amend this extreme condition
            assert(0);
        }
    }
}

//<?page render multiple authors ?>
void preprocessPI(DataBuf* dataBuf, int* pos, int len)
{
    int bufNum = dataBuf->bufnum++;
    //(*pos)++;
    int i = *pos + 1;
    char* data = dataBuf->buf;
    assert(data[i] == '?');
    if(i < len && data[i] == '?'){
        i += 1;
        //(*pos)++;
        dataBuf->bcsay.bcs[bufNum].bt = PI_start;
        dataBuf->bcsay.bcs[bufNum].bufpos = *pos;//PI
        while(i + 2 < len && !(data[i++] == '?' && data[i++] == '>'));
        *pos = i;
        if(i == len){//TODO amend this extreme condition
            assert(1);
        }
    }
}

void preprocess(char* data, DataBuf* dataBuf, int len)
{
    memcpy(dataBuf->buf, data, len);
    int i = 0;
    while(i < len){
        if(data[i] == '<'){
           //assert(i+1 != len);
           switch(data[i+1]){
               case '/':{
                   //endtag
                   preprocessEndTag(dataBuf, &i);
                   //++i;
                   break;
               }
               case '?':{
                   //start PI
                   preprocessPI(dataBuf, &i, len);
                   //++i;
                   break;
               }
               case '!':{
                   //comment or cdata
                   preprocessCommentCDATA(dataBuf, &i, len);
                   //++i;
                   break;
               }
               default:{
                   //++i;
                   //start or empty
                   preprocessStartOrEmpty(dataBuf, &i);
               }
           } 
        }else{
          i++;
        }
    }
}