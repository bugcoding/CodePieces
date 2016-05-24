#include "handleInput.h"
//多行注释的标志位
static int commentFlagStart = 0, commentFlagEnd = 0;
//引号标志位
static int quotationFlagStart = 0, quotationFlagEnd = 0;
static int mulQuotationStart = 0;//双引号跨多行的情况，记住起始位置
//单引号标志位
static int singleQuotationStart = 0, singleQuotationEnd = 0;
//略过空白行
boolean isSpaceLine(char *inputString)
{
	assert(inputString != NULL);
	int i = 0;
	while (inputString[i])
	{
		if (!isspace(inputString[i++]))
		{
			return false;
		}
	}
	return true;
}
//略过单行注释
int getSingleCommentLineLoc(char *inputString)
{
	assert(inputString != NULL);
	int i = 1;
	while (inputString[i])
	{
		if ((inputString[i] == '/' && inputString[i - 1] == '/'))
		{
			return i - 1;//返回注释开始的位置
		}
		i++;
	}
	return i + 1;//没有注释就返回最后一个字符的位置
}

//根据位置替换字符串
void replaceString(char *string, int s, int e)
{
	for (int j = s; j < e + 1; j++)
	{
		string[j] = ' ';
	}
}

//处理多行注释的情况
void handleMulComment(char *inputString)
{
	assert(inputString != NULL);
	int i = 0, start = 0, end = 0;
	while (inputString[i])
	{
		if (inputString[i] == '/' && inputString[i + 1] == '*'
			&& !commentFlagStart)//找到了多行注释的开头
		{
			commentFlagStart = 1;//多行注释开始
			start = i;//保存多注释开始的位置
		}
		if (!commentFlagEnd && inputString[i] == '*'
			&& inputString[i + 1] == '/')//找到了多行注释的末尾
		{
			commentFlagEnd = 1;//多行注释结束
			end = i + 1;//保存多行注释结束的位置
		}
		i++;
		if (commentFlagStart && commentFlagEnd)
		{//如果是在同一行，那么就进行替换，把多行注释变换成空格，由下面的空格处理函数去处理
			replaceString(inputString, start, end);//同一行中的替换
			commentFlagEnd = commentFlagStart = 0;//重置注释标志
		}
	}
}

void handleDoubleQuotation(char *inputString)
{
	assert(inputString != NULL);
	int i = 0, start = 0, end = 0;//记录双引号位置
	while (inputString[i])
	{
		if (inputString[i] == '"' && !quotationFlagStart)//找到第一个双引号
		{
			quotationFlagStart = 1;//开始标志位置1
			mulQuotationStart = start = i;//保存第一个双引号位置
		}
		//必须找到第一个双引号，再开始匹配结束的双引号
		else if (inputString[i] == '"' && quotationFlagStart && !quotationFlagEnd)
		{
			quotationFlagEnd = 1;//结束标志位置1
			end = i;//保存结束位置
		}
		if (quotationFlagStart && quotationFlagEnd)//找到一对双引号
		{
			replaceString(inputString, start, end);//替换掉中间的内容
			quotationFlagEnd = quotationFlagStart = 0;//标志位复位
		}
		i++;
	}
}
//单引号情况，只有单行情况，逻辑与双引号相同
void handleSingleQuotation(char *inputString)
{
	assert(inputString != NULL);
	int i = 0, start = 0, end = 0;
	while (inputString[i])
	{
		if (inputString[i] == '\'' && !singleQuotationStart)
		{
			singleQuotationStart = 1;
			start = i;
		}
		else if (inputString[i] == '\'' && singleQuotationStart && !singleQuotationEnd)
		{
			singleQuotationEnd = 1;
			end = i;
		}
		if (singleQuotationEnd && singleQuotationStart)
		{
			replaceString(inputString, start, end);
			singleQuotationStart = singleQuotationEnd = 0;
		}
		i++;
	}

}

//读取文件内容进行相应的处理
boolean readFileAndHandle(FILE *inputStream, StackList *braceStack,
						ElementType leftSymbol, ElementType rightSymbol)
{
	ElementType value = '\0';
	//读取的每一行
	char streamBuffer[256] = "\0";
	//文件指针
	/*FILE *inputStream = fopen(fileName, "r+");*/
	assert(inputStream != NULL);

	int loc = 0;
	//循环读取内容
	while (!feof(inputStream))
	{
		fgets(streamBuffer, 256, inputStream);//读取文件内容

		loc = getSingleCommentLineLoc(streamBuffer);//略过单行注释
		streamBuffer[loc] = '\n';//忽略注释，但保留这行
		streamBuffer[loc + 1] = '\0';//去掉注释

		handleMulComment(streamBuffer);//寻找多行注释
		if (commentFlagStart == 1 && commentFlagEnd == 0)//多行注释的情况
		{
			//多行注释换行的情况，找到注释头就开始替换
			replaceString(streamBuffer, 0, strlen(streamBuffer));
		}
		handleDoubleQuotation(streamBuffer);
		//双引号跨多行
		if (quotationFlagStart == 1 && quotationFlagEnd == 0)
		{
			replaceString(streamBuffer, mulQuotationStart, strlen(streamBuffer));
			mulQuotationStart = 0;//重置起点数据
		}
		handleSingleQuotation(streamBuffer);

		if (isSpaceLine(streamBuffer))//删除空白行
		{
			streamBuffer[0] = '\0';//清空当行，直接进行下次读取
			continue;
		}

		/*printf("%s", streamBuffer);*/
		//处理大括号
		//循环检测
		for (int i = 0; streamBuffer[i] != '\0'; i++)
		{
			//检测到左括号入栈
			if (streamBuffer[i] == leftSymbol)
			{
				*braceStack = pushElementToStack(*braceStack, leftSymbol);
			}
			else if (streamBuffer[i] == rightSymbol)//检测到右括号
			{
				//如果此时栈空也说明，不匹配
				if (isEmptyStack(*braceStack))
				{
					return false;
				}
				if (getStackTopElement(*braceStack) == leftSymbol)//检测到右括号后，如果栈顶是左括号
				{
					*braceStack = popElementFromStack(*braceStack, &value);//说明匹配，弹出对应的左括号
				}
				else
				{
					return false;//不是左括号，不匹配
				}
			}
		}
		streamBuffer[0] = '\0';//重置当前输入的缓冲，为下次读入准备
	}

	if (!isEmptyStack(*braceStack))
	{
		return false;//栈不为空，说明不匹配
	}
	return true;
}
