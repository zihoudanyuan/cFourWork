/**
 * NameStartChar   ::=   ":" | [A-Z] | "_" | [a-z]  
 * NameChar   ::=   NameStartChar | "-" | "." | [0-9] 
 * Name   ::=   NameStartChar (NameChar)*
 * S   ::=   (#x20 | #x9 | #xD | #xA)+
 * Eq   ::=    S? '=' S?
 * EntityRef   ::=   '&' Name ';'
 * CharRef   ::=   '&#' [0-9]+ ';' | '&#x' [0-9a-fA-F]+ ';'
 * Reference   ::=    EntityRef | CharRef 
 * AttValue   ::=   '"' ([^<&"] | Reference)* '"' |  "'" ([^<&'] | Reference)* "'"
 * Attribute   ::=    Name Eq AttValue
 * STag   ::=   '<' Name (S Attribute)* S? '>'
 * 
 * */
#include "parse.h"

int judgeNameStartChar(char ch)
{
    if (ch == ':' || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int judgeNameChar(char ch)
{
    if (judgeNameStartChar(ch) || ch == '-' || ch == '.' || (ch >= '0' && ch <= '9'))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int judgeName(char *data, int *i, int len)
{
    if (*i >= len)
    {
        return FALSE;
    }
    if (!judgeNameStartChar(data[*i]))
    {
        errorState = WRONGSTARTCHAR;
        return FALSE;
    }
    (*i)++;
    while (*i < len && judgeNameChar(data[*i]))
    {
        (*i)++;
    }
    return TRUE;
}

int judgeNameEx(char *data, int *i, int len, int *stop)
{
    if (*i >= len)
    {
        return FALSE;
    }
    if (!judgeNameStartChar(data[*i]))
    {
        errorState = WRONGSTARTCHAR;
        return FALSE;
    }
    (*i)++;
    while (*i < len && judgeNameChar(data[*i]))
    {
        (*i)++;
    }
    *stop = *i;
    return TRUE;
}

int judgeChar(char ch)
{
    if (ch == 0x9 || ch == 0xA || ch == 0xD || (ch >= 0x20 && ch <= 0x7F))
        return TRUE;
    else
        return FALSE;
}

int judgeNegative(char ch, char exclude)
{
    for (int i = 0; i < illegalCharRefNum; i++)
    {
        if (ch == illegalCharRef[i])
            return FALSE;
    }
    if (ch == exclude)
        return FALSE;
    if (judgeChar(ch))
        return TRUE;
    else
        return FALSE;
}

int judgeSpace(char ch)
{
    if (ch == 0x20 || ch == 0x9 || ch == 0xD || ch == 0xA)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int judgeEqual(char *data, int *i, int len)
{
    if (*i >= len)
    {
        return FALSE;
    }
    int j = *i;
    if (j < len && judgeSpace(data[j]))
    {
        j++;
    }

    if (j < len && data[j] == '=')
    {
        ++j;
    }
    else
    {
        //*i = j;
        return FALSE;
    }
    if (j < len && judgeSpace(data[j]))
    {
        j++;
    }
    // while (j < len && judgeSpace(data[j++]))
    //     ;
    *i = j;
    return TRUE;
}

int judgeEntityRef(char *data, int *i, int len)
{
    if (*i >= len)
    {
        return FALSE; //TODO add another status to stand for incomplete
    }
    //int j = *i;
    if (data[*i] != '&')
    {
        errorState = WRONGENTITYREF;
        return FALSE;
    }
    (*i)++;
    //judge name and ;
    if (!judgeName(data, i, len))
    {
        //errorstate
        return FALSE;
    }
    if (*i >= len)
    {
        return FALSE; //TODO add another status to stand for incomplete
    }
    if (data[*i] == ';')
    {
        (*i)++;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int judgeCharRef(char *data, int *i, int len)
{
    if (*i >= len)
    {
        return FALSE; //TODO add another status to stand for incomplete
    }
    if (data[*i] == '&' || data[*i] == '#' || data[*i] == 'x')
    {
        if (data[*i] == 'x')
        {
            int count = 0;
            for ((*i)++; *i < len; (*i)++)
            {
                char ch = data[*i];
                if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
                {
                    count++;
                }
                else
                {
                    //因为若不是，则应由别的程序处理，所以这里没有加1
                    break;
                }
            }
            //printf("%c\n", data[*i]);
            if ((*i) < len && count >= 1 && data[*i] == ';')
            {
                (*i)++;
                return TRUE;
            }
            else if (*i == len)
            {
                return INCOMPLETE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            int count = 0;
            for ((*i)++; *i < len; (*i)++)
            {
                char ch = data[*i];
                if ((ch >= '0') && (ch <= '9'))
                {
                    count++;
                }
                else
                {
                    break;
                }
            }
            if ((*i) < len && count >= 1 && data[*i] == ';')
            {
                (*i)++;
                return TRUE;
            }
            else if (*i == len)
            {
                return INCOMPLETE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    else
    {
        return FALSE;
    }
}
int judgeReference(char *data, int *i, int len)
{
    assert(*i < len); //TODO ignore the incomplete station
    int j = *i;
    if (TRUE == judgeEntityRef(data, i, len))
    {
        return TRUE;
    }
    else
    {
        *i = j;
        if (TRUE == judgeCharRef(data, i, len))
        {
            return TRUE;
        }
        else
        {
            *i = j; //解析失败恢复指针的位置*i
            return FALSE;
        }
    }
    // if (TRUE == judgeEntityRef(data, i, len) || TRUE == judgeCharRef(data, i, len))
    // {
    //     return TRUE;
    // }
    // else //ignore INCOMPLETE ////TODO
    // {
    //     return FALSE;
    // }
}
int judgeAttValue(char *data, int *i, int len)
{
    assert(*i < len);
    if (data[*i] == '\"')
    {
        //(*i)++;
        assert((*i) + 1 < len);
        // if (data[(*i) + 1] == '\"')//TODO ignore the attvalue""
        // {
        //     (*i) += 2;
        //     return TRUE;
        // }
        (*i)++;
        assert(*i < len);
        //int count = 0;
        int k;
        for (k = *i; k < len; k++)
        {
            if (judgeNegative(data[k], '\"'))
            {
                k++;
                while (judgeNegative(data[k], '\"'))
                {
                    k++;
                }
                k--;
            }
            else if (judgeReference(data, &k, len))
            {
            }
            else if (*i < len && data[k] == '\"') //bug detail in testparse.c
            {
                *i = k + 1;
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        if (k == len)
        {
            errorState = INCOMPLETE;
            return INCOMPLETE;
        }
        else
        {
            return FALSE;
        }
    }
    else if (data[*i] == '\'')
    {
        //assert(*i < len);
        assert((*i) + 1 < len);
        // if (data[(*i) + 1] == '\'')
        // {
        //     (*i) += 2;
        //     return TRUE;
        // }
        (*i)++;
        //assert(*i < len);
        //int count = 0;
        int k;
        for (k = *i; k < len; k++)
        {
            if (judgeNegative(data[k], '\''))
            {
                k++;
                while (judgeNegative(data[k], '\"'))
                {
                    k++;
                }
                k--;
            }
            else if (judgeReference(data, &k, len))
            {
            }
            else if (*i < len && data[k] == '\'')
            {
                *i = k + 1;
                return TRUE;
            }
            else // because one call only find one AttValue
            {
                return FALSE;
            }
        }
        if (k == len)
        {
            errorState = INCOMPLETE;
            return INCOMPLETE;
        }
        else
        {
            return FALSE; //when arrive here
        }
    }
    else
    {
        return FALSE;
    }
}

//maybe it shouldn't return TRUE or FALSE
int judgeAttribute(char *data, int *i, int len, int *nameStart, int *nameEnd, int *attValueStart, int *attValueStop)
{
    //int nameStart = *i, nameEnd, attValueStart, attValueStop;
    // if (TRUE == judgeName(data, i, len))
    // {
    //     nameEnd = i - 1;
    //     if(judgeEqual(data, i, len)){
    //         attValueStart = i;
    //         if
    //     }
    // }
    *nameStart = *i;
    if (judgeNameEx(data, i, len, nameEnd) && judgeEqual(data, i, len))
    {
        *attValueStart = *i;
        if (judgeAttValue(data, i, len))
        {
            *attValueStop = *i;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

    // if (judgeNameEx(data, i, len, &nameEnd) && judgeEqual(data, i, len) && judgeAttValue(data, i, len))
    // {
    //     return TRUE;
    // }
    // else
    // {
    //     return FALSE;
    // }
}

/**
 * Char   ::=   #x9 | #xA | #xD | [#x20-#x7F]
 * NameStartChar   ::=   ":" | [A-Z] | "_" | [a-z]  
 * NameChar   ::=   NameStartChar | "-" | "." | [0-9] 
 * Name   ::=   NameStartChar (NameChar)*
 * S   ::=   (#x20 | #x9 | #xD | #xA)+
 * Eq   ::=    S? '=' S?
 * EntityRef   ::=   '&' Name ';'
 * CharRef   ::=   '&#' [0-9]+ ';' | '&#x' [0-9a-fA-F]+ ';'
 * Reference   ::=    EntityRef | CharRef 
 * AttValue   ::=   '"' ([^<&"] | Reference)* '"' |  "'" ([^<&'] | Reference)* "'"
 * Attribute   ::=    Name Eq AttValue
 * STag   ::=   '<' Name (S Attribute)* S? '>'
 * content   ::=    CharData? ((element | Reference | CDSect | PI | Comment) CharData?)*
 * element   ::=    EmptyElemTag | STag content ETag
 * EmptyElemTag   ::=   '<' Name (S Attribute)* S? '/>'
 *
 * */
/**
 *<Order date="1999-1-1">
<shipTo country="US">
<name>
Alice Smith
</name>
<!--comments-->
</Order>
SE Order A date “1999-1-1” SE shipTo A country “US” SE name CD “Alice Smith” EE C “comments” ... EE
 * */

void parseStartTag(char *data, Bcs bcs, int len, DataBuf *dataBuf)
{
    //printf("start parseStartTag\n");
    int i = bcs.bufpos + 1;
    if (!judgeNameStartChar(data[i]))
    {
        errorState = 2; // WRONGSTART
        return;         // what happened here
    }
    int nameStart = i++;
    while (i < len && judgeNameChar(data[i]))
    {
        i++;
    }
    if (i >= len)
    {
        errorState = INCOMPLETE;
        return;
    }
    int nameEnd = i;
    Event *event = &(dataBuf->eventStream[dataBuf->eventIndex++]);
    //(Event *)malloc(sizeof(Event));
    event->type = STAG; //name
    event->startPos = nameStart;
    event->stopPos = nameEnd;

    // for (int m = nameStart; m < nameEnd; ++m)
    // {
    //     printf("%c", data[m]);
    // }
    //break;

    //handle empty attribute or empty element
    // if (data[i] == '>')
    // {
    //     return;
    // }
    // else if (judgeSpace(data[i]) && i + 1 < len && data[i + 1] == '>')
    // {
    //     return;
    // }
    // else if (i + 2 < len && judgeSpace(data[i]) && data[i + 1] == '/' && data[i + 2] == '>')
    // {
    //     //empty ignore the empty element
    //     return;
    // }
    // else if (i + 1 < len && data[i] == '/' && data[i + 1] == '>')
    // {
    //     //empty ignore the empty element
    //     return;
    // }

    //TODO here if need space i++
    int nameStop, attValueStart, attValueStop;
    for (; i < len; ++i)
    {
        if (TRUE == judgeAttribute(data, &i, len, &nameStart, &nameStop, &attValueStart, &attValueStop))
        {
            // printf("true\n");
            // for (int k = nameStart; k < nameStop; ++k)
            // {
            //     printf("%c", data[k]);
            // }
            // printf("\n");
            // for (int k = attValueStart; k < attValueStop; ++k)
            // {
            //     printf("%c", data[k]);
            // }
            // printf("\n");
            Event *event = &(dataBuf->eventStream[dataBuf->eventIndex++]);
            //(Event *)malloc(sizeof(Event));
            event->type = ATTRIBUTE; //name
            event->startPos = nameStart;
            event->stopPos = nameStop;
            event->startPos2 = attValueStart;
            event->stopPos2 = attValueStop;
        } //因为多加了1个1，所以跳过了多个属性值时，属性间的空格
        else if (data[i] == '>')
        {
            break;
            //printf("false\n");
        }
    }
}

void parseEvents(DataBuf *dataBuf, int len)
{
    int i;
    for (i = 0; i < dataBuf->bufnum; ++i)
    {
        if (dataBuf->bcsay.bcs[i].bt == StagorEmptytag_start)
        {
            if (i + 1 < dataBuf->bufnum)
            {
                //printf("first = %d, second = %d\n", dataBuf->bcsay.bcs[i].bufpos, dataBuf->bcsay.bcs[i+1].bufpos);
                parseStartTag(dataBuf->buf, dataBuf->bcsay.bcs[i], dataBuf->bcsay.bcs[i + 1].bufpos, dataBuf);
                if (errorState != 0)
                {
                    printf("failure parseStartTag at %d\n", i);
                    //error process further
                }
                //return;
            }
        }
        else if (dataBuf->bcsay.bcs[i].bt == COMMENT_start)
        {
            i++;
        }
        else if (dataBuf->bcsay.bcs[i].bt == CDSECT_start)
        {
            i++;
        }
        else if (dataBuf->bcsay.bcs[i].bt == PI_start)
        {
            i++;
        }
    }
}
