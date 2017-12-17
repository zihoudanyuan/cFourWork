#ifndef PARSE_H
#define PARSE_H
#include "base.h"
#include <stdio.h>
#include <assert.h>
static const int illegalCharRefNum = 2;
static char illegalCharRef[2] = {'<', '&'};
void parseEvents(DataBuf *dataBuf, int len);
#endif