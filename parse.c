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
#include"parse.h"

int judgeNameStartChar(char ch){
    if(ch == ':' || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_'){
        return TRUE;
    }else{
        return FALSE;
    }
}

int judgeNameChar(char ch){
    if(judgeNameStartChar(ch) || ch == '-' || ch == '.' || (ch >= '0' && ch <= '9')){
        return TRUE;
    }else{
        return FALSE;
    }
}

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

void parseStartTag(char* data, Bcs bcs, int len)
{



    

}

void parseEvents(DataBuf* dataBuf, int len)
{
    int i;
    for(i = 0; i < dataBuf->bufnum; ++i){
        if(dataBuf->bcsay.bcs[i].bt == StagorEmptytag_start){
            //printf("pi\n");
            //parseStartTag(dataBuf->buf, dataBuf->bcsay.bcs[i], 0);
            // int j = 0;
            // for(j = dataBuf->bcsay.bcs[i].bufpos; j < dataBuf->bcsay.bcs[i+1].bufpos; ++j){
            //     printf("%c", dataBuf->buf[j]);
            // }
            // break;
            i++;
        }else if(dataBuf->bcsay.bcs[i].bt == COMMENT_start){
            int j = 0;
            for(j = dataBuf->bcsay.bcs[i].bufpos; j < dataBuf->bcsay.bcs[i+1].bufpos; ++j){
                printf("%c", dataBuf->buf[j]);
            }
            i++;
        }else{
            i++;
        }
    }
}
