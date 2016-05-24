#ifndef _HANDLE_INPUT_H_
#define _HANDLE_INPUT_H_

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "braceStack.h"

//检测是否为空行
boolean isSpaceLine(char *inputString);
//处理单行注释
int getSingleCommentLineLoc(char *inputString);
//处理多行注释
void handleMulComment(char *inputString);
//替换字符串
void replaceString(char *string, int s, int e);
//处理双引号
void handleDoubleQuotation(char *inputString);
//处理单引号
void handleSingleQuotation(char *inputString);
//处理最终文件流和括号匹配情况
boolean readFileAndHandle(FILE *stream, StackList *braceStack, ElementType leftSymbol, ElementType rightSymbol);


#endif 
