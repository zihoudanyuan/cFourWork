#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "base.h"
#include "parse.h"

//testJudgeNameStartChar()

void testJudgeEntityRef()
{
    char *data = "&hellokitty;hellokitty";
    int len = strlen(data);
    int i = 0;
    printf("%c\n", data[i]);
    if (judgeEntityRef(data, &i, len))
    {
        printf("true\n");
        printf("now at %c\n", data[i]);
    }
    else
    {
        printf("error\n");
        printf("now at %c\n", data[i]);
    }
    char *data2 = "&hello kitty;hellokitty";
    int len2 = strlen(data2);
    int i2 = 0;
    printf("%c\n", data2[i2]);
    if (judgeEntityRef(data2, &i2, len2))
    {
        printf("true\n");
        printf("now at %c\n", data2[i2]);
    }
    else
    {
        printf("error\n");
        printf("now at %c\n", data2[i2]);
    }
}

void testJudgeCharRef()
{
    char *data1 = "&09";
    char *data2 = "&9;";
    char *data3 = "&f";
    char *data4 = "x9;";
    char *data5 = "x09FfaA";
    char *data6 = "#F;";
    char *ch[] = {data1, data2, data3, data4, data5, data6};
    for (int j = 0; j < 6; ++j)
    {
        int i = 0;
        int result = judgeCharRef(ch[j], &i, strlen(ch[j]));
        if (INCOMPLETE == result)
        {
            printf("%s is incomplete\n", ch[j]);
        }
        else if (FALSE == result)
        {
            printf("%s is false\n", ch[j]);
        }
        else
        {
            printf("%s is true\n", ch[j]);
        }
    }
}

void testJudgeRefefrence()
{
    // char *data1 = "&09";
    // char *data2 = "&9;";
    // char *data3 = "&f";
    // char *data4 = "x9;";
    // char *data5 = "x09FfaA";
    // char *data6 = "#F;";
    // char *ch[] = {data1, data2, data3, data4, data5, data6};
    // for (int j = 0; j < 6; ++j)
    // {
    //     int i = 0;
    //     int result = judgeReference(ch[j], &i, strlen(ch[j]));
    //     if (INCOMPLETE == result)
    //     {
    //         printf("%s is incomplete\n", ch[j]);
    //     }
    //     else if (FALSE == result)
    //     {
    //         printf("%s is false\n", ch[j]);
    //     }
    //     else
    //     {
    //         printf("%s is true\n", ch[j]);
    //     }
    // }

    char *data = "&hellokitty;hellokitty";
    int len = strlen(data);
    int i = 0;
    printf("%c\n", data[i]);
    if (judgeReference(data, &i, len))
    {
        printf("true\n");
        printf("now at %c\n", data[i]);
    }
    else
    {
        printf("error\n");
        printf("now at %c\n", data[i]);
    }
    char *data2 = "&hello kitty;hellokitty";
    int len2 = strlen(data2);
    int i2 = 0;
    printf("%c\n", data2[i2]);
    if (judgeReference(data2, &i2, len2))
    {
        printf("true\n");
        printf("now at %c\n", data2[i2]);
    }
    else
    {
        printf("error\n");
        printf("now at %c\n", data2[i2]);
    }
}

void testJudgeAttValue()
{
    //char *data = "\"\"1999-1-1\""; //bug example
    char *data = "\"1999-1-1\"";
    //char *data = "\"data\"";
    int len = strlen(data);
    int i = 0;
    if (TRUE == judgeAttValue(data, &i, len))
    {
        for (int k = 0; k < i; ++k)
            printf("%c", data[k]);
    }
    else
    {
        printf("false %s\n", data);
    }
}

void testJudgeAttribute()
{
    char *data = "date=\"1999-1-1\"";
    //char *data = "&date=\"1999-1-1\"";
    //char *data = "date =\"1999-1-1\"";
    //char *data = "date = \"1999-1-1\"  time = \"23:23:23\"";
    int len = strlen(data);
    int i = 0;
    // int nameStart, nameStop, attValueStart, attValueStop;
    // if (TRUE == judgeAttribute(data, &i, len, &nameStart, &nameStop, &attValueStart, &attValueStop))
    // {
    //     printf("true\n");
    //     for (int k = nameStart; k <= nameStop; ++k)
    //     {
    //         printf("%c", data[k]);
    //     }
    //     printf("\n");
    //     for (int k = attValueStart; k <= attValueStop; ++k)
    //     {
    //         printf("%c", data[k]);
    //     }
    //     printf("\n");
    // }
    // else
    // {
    //     printf("false\n");
    // }
    int nameStart, nameStop, attValueStart, attValueStop;
    for (; i < len; ++i)
    {
        if (TRUE == judgeAttribute(data, &i, len, &nameStart, &nameStop, &attValueStart, &attValueStop))
        {
            printf("true\n");
            for (int k = nameStart; k < nameStop; ++k)
            {
                printf("%c", data[k]);
            }
            printf("\n");
            for (int k = attValueStart; k < attValueStop; ++k)
            {
                printf("%c", data[k]);
            }
            printf("\n");
        }
        else
        {
            printf("false\n");
        }
    }
}

int main(int argc, char **argv)
{
    //testJudgeEntityRef();
    // testJudgeCharRef();
    //testJudgeRefefrence();
    //testJudgeAttValue();
    testJudgeAttribute();
    // if (judgeNegative('\"', '\"'))
    // {
    //     printf("true\n");
    // }
    // else
    // {
    //     printf("false\n");
    // }
    // if (judgeNameStartChar('c'))
    // {
    //     printf("true\n");
    // }
    // else
    // {
    //     printf("false\n");
    // }

    // char *data = "Order data=\"1999-1-1\"";
    // int i = 0, len = sizeof("Order data=\"1999-1-1\"");
    // int nameStart = i, nameEnd;
    // if (judgeName(data, &i, len))
    // {
    //     nameEnd = i - 1;
    //     // for (int j = 0; j < i; ++j)
    //     // {
    //     //     printf("%c", data[j]);
    //     // }
    // }

    // if (!judgeSpace(data[i]))
    // {
    //     printf("judge equal wrong\n");
    // }
    // else
    // {
    //     i++;
    //     printf("new i = %d\n", i);
    // }

    // for (int j = i; j < len; ++j)
    // {
    //     if (!judgeSpace(data[j]))
    //     {
    //         printf("here is %c\n", data[j]);
    //         errorState = WRONGSPACE;
    //         //return;
    //         printf("wrong judgeEqual = %d\n", errorState);
    //         break;
    //     }
    //     j++;
    //     if (j < len && !judgeNameStartChar(data[j]))
    //     {
    //         errorState = 2; // WRONGSTART
    //         return;         // what happened here
    //     }
    //     int nameStart2 = j++;
    //     while (j < len && judgeNameChar(data[j]))
    //     {
    //         j++;
    //     }
    //     if (j >= len)
    //     {
    //         errorState = INCOMPLETE;
    //         return;
    //     }
    //     int nameEnd2 = j - 1;
    //     for (int k = nameStart2; k <= nameEnd2; ++k)
    //     {
    //         printf("%c", data[k]);
    //     }
    //     //break;
    //     // Event *event = (Event *)malloc(sizeof(Event)); //&(dataBuf->eventStream[dataBuf->eventIndex++]);
    //     // //(Event *)malloc(sizeof(Event));
    //     // event->type = ATTRIBUTE; //name
    //     // event->startPos = nameStart;
    //     // event->stopPos = nameEnd;

    //     //judge eq
    //     printf("eq is %c\n", data[j]);
    //     if (!judgeEqual(data, &j, len))
    //     {
    //         //errorState = WRONGEQUALS;
    //         printf("wrong judgeEqual = %d\n", errorState);
    //         break;
    //     }
    //     else
    //     {
    //         printf("ok equal, %c\n", data[j]);
    //         //break;
    //     }
    //     int attValueBegin = j, attValueEnd;
    //     if (!judgeAttValue(data, &j, len))
    //     {
    //         printf("wrong judgeAttValue , now at %c n", data[j]);
    //         break;
    //     }
    //     else
    //     {
    //         attValueEnd = j - 1;
    //         for (int k = attValueBegin; k <= attValueEnd; ++k)
    //         {
    //             printf("attValue = %c", data[k]);
    //         }
    //         printf("\n");
    //         break;
    //     }
    // }
}