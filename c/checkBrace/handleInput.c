#include "handleInput.h"
//����ע�͵ı�־λ
static int commentFlagStart = 0, commentFlagEnd = 0;
//���ű�־λ
static int quotationFlagStart = 0, quotationFlagEnd = 0;
static int mulQuotationStart = 0;//˫���ſ���е��������ס��ʼλ��
//�����ű�־λ
static int singleQuotationStart = 0, singleQuotationEnd = 0;
//�Թ��հ���
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
//�Թ�����ע��
int getSingleCommentLineLoc(char *inputString)
{
	assert(inputString != NULL);
	int i = 1;
	while (inputString[i])
	{
		if ((inputString[i] == '/' && inputString[i - 1] == '/'))
		{
			return i - 1;//����ע�Ϳ�ʼ��λ��
		}
		i++;
	}
	return i + 1;//û��ע�;ͷ������һ���ַ���λ��
}

//����λ���滻�ַ���
void replaceString(char *string, int s, int e)
{
	for (int j = s; j < e + 1; j++)
	{
		string[j] = ' ';
	}
}

//�������ע�͵����
void handleMulComment(char *inputString)
{
	assert(inputString != NULL);
	int i = 0, start = 0, end = 0;
	while (inputString[i])
	{
		if (inputString[i] == '/' && inputString[i + 1] == '*'
			&& !commentFlagStart)//�ҵ��˶���ע�͵Ŀ�ͷ
		{
			commentFlagStart = 1;//����ע�Ϳ�ʼ
			start = i;//�����ע�Ϳ�ʼ��λ��
		}
		if (!commentFlagEnd && inputString[i] == '*'
			&& inputString[i + 1] == '/')//�ҵ��˶���ע�͵�ĩβ
		{
			commentFlagEnd = 1;//����ע�ͽ���
			end = i + 1;//�������ע�ͽ�����λ��
		}
		i++;
		if (commentFlagStart && commentFlagEnd)
		{//�������ͬһ�У���ô�ͽ����滻���Ѷ���ע�ͱ任�ɿո�������Ŀո�����ȥ����
			replaceString(inputString, start, end);//ͬһ���е��滻
			commentFlagEnd = commentFlagStart = 0;//����ע�ͱ�־
		}
	}
}

void handleDoubleQuotation(char *inputString)
{
	assert(inputString != NULL);
	int i = 0, start = 0, end = 0;//��¼˫����λ��
	while (inputString[i])
	{
		if (inputString[i] == '"' && !quotationFlagStart)//�ҵ���һ��˫����
		{
			quotationFlagStart = 1;//��ʼ��־λ��1
			mulQuotationStart = start = i;//�����һ��˫����λ��
		}
		//�����ҵ���һ��˫���ţ��ٿ�ʼƥ�������˫����
		else if (inputString[i] == '"' && quotationFlagStart && !quotationFlagEnd)
		{
			quotationFlagEnd = 1;//������־λ��1
			end = i;//�������λ��
		}
		if (quotationFlagStart && quotationFlagEnd)//�ҵ�һ��˫����
		{
			replaceString(inputString, start, end);//�滻���м������
			quotationFlagEnd = quotationFlagStart = 0;//��־λ��λ
		}
		i++;
	}
}
//�����������ֻ�е���������߼���˫������ͬ
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

//��ȡ�ļ����ݽ�����Ӧ�Ĵ���
boolean readFileAndHandle(FILE *inputStream, StackList *braceStack,
						ElementType leftSymbol, ElementType rightSymbol)
{
	ElementType value = '\0';
	//��ȡ��ÿһ��
	char streamBuffer[256] = "\0";
	//�ļ�ָ��
	/*FILE *inputStream = fopen(fileName, "r+");*/
	assert(inputStream != NULL);

	int loc = 0;
	//ѭ����ȡ����
	while (!feof(inputStream))
	{
		fgets(streamBuffer, 256, inputStream);//��ȡ�ļ�����

		loc = getSingleCommentLineLoc(streamBuffer);//�Թ�����ע��
		streamBuffer[loc] = '\n';//����ע�ͣ�����������
		streamBuffer[loc + 1] = '\0';//ȥ��ע��

		handleMulComment(streamBuffer);//Ѱ�Ҷ���ע��
		if (commentFlagStart == 1 && commentFlagEnd == 0)//����ע�͵����
		{
			//����ע�ͻ��е�������ҵ�ע��ͷ�Ϳ�ʼ�滻
			replaceString(streamBuffer, 0, strlen(streamBuffer));
		}
		handleDoubleQuotation(streamBuffer);
		//˫���ſ����
		if (quotationFlagStart == 1 && quotationFlagEnd == 0)
		{
			replaceString(streamBuffer, mulQuotationStart, strlen(streamBuffer));
			mulQuotationStart = 0;//�����������
		}
		handleSingleQuotation(streamBuffer);

		if (isSpaceLine(streamBuffer))//ɾ���հ���
		{
			streamBuffer[0] = '\0';//��յ��У�ֱ�ӽ����´ζ�ȡ
			continue;
		}

		/*printf("%s", streamBuffer);*/
		//���������
		//ѭ�����
		for (int i = 0; streamBuffer[i] != '\0'; i++)
		{
			//��⵽��������ջ
			if (streamBuffer[i] == leftSymbol)
			{
				*braceStack = pushElementToStack(*braceStack, leftSymbol);
			}
			else if (streamBuffer[i] == rightSymbol)//��⵽������
			{
				//�����ʱջ��Ҳ˵������ƥ��
				if (isEmptyStack(*braceStack))
				{
					return false;
				}
				if (getStackTopElement(*braceStack) == leftSymbol)//��⵽�����ź����ջ����������
				{
					*braceStack = popElementFromStack(*braceStack, &value);//˵��ƥ�䣬������Ӧ��������
				}
				else
				{
					return false;//���������ţ���ƥ��
				}
			}
		}
		streamBuffer[0] = '\0';//���õ�ǰ����Ļ��壬Ϊ�´ζ���׼��
	}

	if (!isEmptyStack(*braceStack))
	{
		return false;//ջ��Ϊ�գ�˵����ƥ��
	}
	return true;
}
