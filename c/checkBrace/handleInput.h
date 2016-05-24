#ifndef _HANDLE_INPUT_H_
#define _HANDLE_INPUT_H_

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "braceStack.h"

//����Ƿ�Ϊ����
boolean isSpaceLine(char *inputString);
//������ע��
int getSingleCommentLineLoc(char *inputString);
//�������ע��
void handleMulComment(char *inputString);
//�滻�ַ���
void replaceString(char *string, int s, int e);
//����˫����
void handleDoubleQuotation(char *inputString);
//��������
void handleSingleQuotation(char *inputString);
//���������ļ���������ƥ�����
boolean readFileAndHandle(FILE *stream, StackList *braceStack, ElementType leftSymbol, ElementType rightSymbol);


#endif 
