#include"util.h"

long fileSize(FILE *fp)
{
    if(fp == NULL){
        return -1;
    }
    long curpos, length;
    curpos = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    length = ftell(fp);
    fseek(fp, curpos, SEEK_SET);
    return length;
}

int skipViewFile(FILE *fp, int offset)
{
    if(fp == NULL){
        return -1;
    }
    long length;
    length = fileSize(fp);
    char* buf = (char*)malloc(length);
    fseek(fp, offset, SEEK_SET);
    fread(buf, length - offset, 1, fp);
    for(int i = 0; i < length - offset; ++i){
        printf("%c", buf[i]);
    }
    free(buf);
}