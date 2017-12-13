#ifndef UTIL_H
#define UTIL_H
#include<stdio.h>
#include<stdlib.h>
long fileSize(FILE *fp);
int skipViewFile(FILE *fp, int offset);
#endif