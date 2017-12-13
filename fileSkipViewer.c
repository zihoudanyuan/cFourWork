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
    skipViewFile(fp, offset);
    fclose(fp);
    return 0;
}