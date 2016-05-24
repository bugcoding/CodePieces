#ifndef _BRACE_STACK_H_
#define _BRACE_STACK_H_

#include <stdio.h>
#include <stdlib.h>
//布尔定义
typedef enum{false, true} boolean;
//栈中元素类型定义
typedef char ElementType;

//栈结构
typedef struct StackNode 
{
	ElementType data;
	struct StackNode *next;
}Node, *StackList;

//初始化
StackList initStack();
//判定栈空
boolean isEmptyStack(StackList top);
//压入元素
StackList pushElementToStack(StackList top, ElementType element);
//弹出
StackList popElementFromStack(StackList top, ElementType *element);
//拿到栈顶元素
ElementType getStackTopElement(StackList top);
//得到栈的长度
int getStackLength(StackList top);
//打印栈元素
void printStackElement(StackList top);
#endif
