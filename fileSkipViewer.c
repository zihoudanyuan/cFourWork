#include"util.h"

int main(int argc, char** argv)
{
    if( argc != 3){
        printf("usage ./fileSkipViewer fileName offset\n");
        exit(1);
    }
    FILE* fp = fopen(argv[1], "r");
    if(fp == NULL){
        printf("open file wrong\n");
        exit(2);
    }
    int offset = atoi(argv[2]);
    int result = skipViewFile(fp, offset);
    if(-1 == result){
        printf("打开文件失败\n");
    }else if( -2 == result){
        printf("偏移 %d 大于量文件长度 %ld\n", offset, fileSize(fp));
    }
    fclose(fp);
    return 0;
}