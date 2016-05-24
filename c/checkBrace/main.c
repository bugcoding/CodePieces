#include "main.h"

int main(int argc, const char *argv[])
{
	//文件路径
	char filePath[128] = "\0";
	//文件流
	FILE *stream;
	//得到一个空栈
	StackList braceStack = initStack();

	//输入文件路径
	printf("please input your check file path:\t");
REPEAT:
	scanf("%s", filePath);
	stream = fopen(filePath, "r");
	//打开失败处理
	if (stream == NULL)
	{
		printf("File Open Failed! You Input File Path is %s,Check And Input FilePath Repeat!\n", filePath);
		goto REPEAT;
	}
	//读取文件流处理括号匹配
	boolean flag = readFileAndHandle(stream, &braceStack, '{', '}');
	
	//输出提示信息
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
//输出提示信息
void outputMessage(char *colorCommand, char *mess)
{
	/*system("cls");*/
	system(colorCommand);
	printf("%s\n", mess);
}
