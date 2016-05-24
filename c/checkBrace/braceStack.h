#ifndef _BRACE_STACK_H_
#define _BRACE_STACK_H_

#include <stdio.h>
#include <stdlib.h>
//��������
typedef enum{false, true} boolean;
//ջ��Ԫ�����Ͷ���
typedef char ElementType;

//ջ�ṹ
typedef struct StackNode 
{
	ElementType data;
	struct StackNode *next;
}Node, *StackList;

//��ʼ��
StackList initStack();
//�ж�ջ��
boolean isEmptyStack(StackList top);
//ѹ��Ԫ��
StackList pushElementToStack(StackList top, ElementType element);
//����
StackList popElementFromStack(StackList top, ElementType *element);
//�õ�ջ��Ԫ��
ElementType getStackTopElement(StackList top);
//�õ�ջ�ĳ���
int getStackLength(StackList top);
//��ӡջԪ��
void printStackElement(StackList top);
#endif
