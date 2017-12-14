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

int skipViewFile(FILE *fp, int offset, int direction)
{
    if(fp == NULL ){
        return -1;
    }else if(offset < 0){
        return -2;
    }
    long length;
    if(direction == -1){
        char* buf = (char*)malloc(offset);
        //fseek(fp, OL, SEEK_SET);
        fread(buf, offset, 1, fp);
        for(int i = 0; i < offset; ++i){
            printf("%c", buf[i]);
        }
        free(buf);
        return 0;
    }else{
        length = fileSize(fp);
        if(offset > length){
            return -2;
        }
        char* buf = (char*)malloc(length);
        fseek(fp, offset, SEEK_SET);
        fread(buf, length - offset, 1, fp);
        for(int i = 0; i < length - offset; ++i){
            printf("%c", buf[i]);
        }
        free(buf);
        return 0;
    }
}