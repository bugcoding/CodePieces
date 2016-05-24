#include "main.h"

int main(int argc, const char *argv[])
{
	//�ļ�·��
	char filePath[128] = "\0";
	//�ļ���
	FILE *stream;
	//�õ�һ����ջ
	StackList braceStack = initStack();

	//�����ļ�·��
	printf("please input your check file path:\t");
REPEAT:
	scanf("%s", filePath);
	stream = fopen(filePath, "r");
	//��ʧ�ܴ���
	if (stream == NULL)
	{
		printf("File Open Failed! You Input File Path is %s,Check And Input FilePath Repeat!\n", filePath);
		goto REPEAT;
	}
	//��ȡ�ļ�����������ƥ��
	boolean flag = readFileAndHandle(stream, &braceStack, '{', '}');
	
	//�����ʾ��Ϣ
	if (flag)
	{
		outputMessage("color 0A", "Brace Match Success!");
	}
	else
	{
		outputMessage("color 0C", "Brace Match Fail!");
	}
	system("pause");
	
	return 0;
}
//�����ʾ��Ϣ
void outputMessage(char *colorCommand, char *mess)
{
	/*system("cls");*/
	system(colorCommand);
	printf("%s\n", mess);
}
