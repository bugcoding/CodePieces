/*=============================================================================
#     FileName: getDirectory.c
#         Desc: 
#       Author: zx
#        Email: zx_start@163.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2014-03-03 12:30:39
#=============================================================================*/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#if (defined(_WIN32) || defined(WIN32) || defined(_win32))
	#include <windows.h>
#elif defined(_LINUX)
	#include <sys/types.h>
	#include <unistd.h>
	#include <dirent.h>
	#include <sys/stat.h>
#endif

//文件总行数
int total_lines = 0;
//后缀名的标记
#define DOT_FLAG '.'
//buffer的最大长度
#define MAX_BUF_LEN 512
//模拟bool
typedef enum{FALSE, TRUE} boolean;
//全局票房下一行是不是属于注释行
boolean next_line_is_comment = FALSE;

int get_and_count_lines(const char *file_name);
void get_file_extension_name(const char *file_name, char *extension);


//读取对应目录下的文件
#if (defined(_WIN32) || defined(WIN32) || defined(_win32))

void list_dir_files(const char *file_name, const char *ext)
{
	assert(file_name);
	WIN32_FIND_DATA file_data;//文件信息结构
	char file_path[256 + 1] = "\0";//文件路径
	char extension[32] = "\0";//扩展名
	HANDLE file_find = INVALID_HANDLE_VALUE;//找到文件的句柄
	
	//匹配当前目录下的所有文件,包括目录
	strcpy(file_path, file_name);
	strcat(file_path, "\\*");
	
	//找到第一个文件
	file_find = FindFirstFile(file_path, &file_data);
	if (INVALID_HANDLE_VALUE == file_find)//找不到
	{
		fprintf(stderr, "%s\n", "Get File Error!");
		return ;
	}
	//进入循环
	do
	{
		//如果是..和.二个目录
		if (!strcmp(file_data.cFileName, ".") || !strcmp(file_data.cFileName, ".."))
		{
			continue;
		}
		//如果文件是目录
		if (file_data.dwFileAttributes &　FILE_ATTRIBUTE_DIRECTORY)
		{
			list_dir_files(file_path, ext);
		}
		else
		{
			//获取文件后缀名
			get_file_extension_name(file_data.cFileName, extension);
			if (!strcmp(extension, ext))
			{
				printf("%s\n", file_path);
				total_lines += get_and_count_lines(file_path);
			}
		}

	}while ((FindNextFile(file_find, &file_data)));//找到了下一个文件,或者目录

	//关闭目录
	FindClose(file_find);
}

#elif (defined(_LINUX))

void list_dir_files(const char *file_name, const char *ext)
{
	assert(file_name);
	DIR *temp_dir = NULL;//目录指针
	struct dirent *files = NULL;//目录下的文件
	struct stat file_state = {0};//获取文件状态,这里用来检测文件是否是目录
	char file_path[256 + 1] = "\0";//文件路径
	char extension[32] = "\0";//文件名扩展
	
	//打开目录
	if (!(temp_dir = opendir(file_name)))
	{
		fprintf(stderr, "%s\n", "Open Directory Error!");
		return ;
	}

	while ((files = readdir(temp_dir)))//循环读目录下的文件
	{
		//跳过二个默认目录
		if (!strcmp(files->d_name, ".") || !strcmp(files->d_name, ".."))
		{
			continue;				
		}

		//拼装文件路径
		sprintf(file_path, "%s/%s", file_name, files->d_name);

		//检查文件状态,如果出错就继续下一个文件
		if (lstat(file_path, &file_state) < 0)
		{
			fprintf(stderr, "%s\n", "Check File State Error!");
			continue;
		}
		//如果文件是目录,递归向下读
		if (S_ISDIR(file_state.st_mode))
		{
			list_dir_files(file_path, ext);
		}
		else//如果是文件并且是
		{
			get_file_extension_name(files->d_name, extension);//获取文件后缀名
			if (!strcmp(extension, ext))
			{
				printf("%s\n", file_path);
				total_lines += get_and_count_lines(file_path);
			}
		}
	}
	closedir(temp_dir);//关闭目录
}
#endif

//获取文件名的后续
void get_file_extension_name(const char *file_name, char *extension)
{
	assert(file_name);

	char temp_name[256] = "\0";
	//获取文件名的副本字符串
	strcpy(temp_name, file_name);
	char *loc = strrchr(temp_name, DOT_FLAG);
	//找到了位置
	if (loc)
	{
		//取到的是.xxx,这里将xxx部分复制到extension中	
		strcpy(extension, loc + 1);
	}
	else
	{
		//如果没有找到后缀,将entention置空
		strcpy(extension, "\0");
	}
}
//判定一行是不是空行
boolean is_blank_line(const char *line)
{
	//本行的空格数
	int count_blank = 0;
	//如果第一个字符就是\n,是空行
	if (line[0] == '\n')
	{
		return TRUE;
	}
	for (int i = 0; i < strlen(line); i++)
	{
		//如果是空格或者是tab
		if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
		{
			count_blank++;//空格计数	
		}
		//不是空字符,则这一行不是空行
		else if (line[i] != ' ' || line[i] != '\t' || line[i] != '\n')
		{
			return FALSE;
		}
	}
	//这一行全是空格
	if (count_blank == strlen(line))
	{
		return TRUE;	
	}
	//以上情况都不符合,直接返回不是空行
	return FALSE;
}


//读取文本的一行,并统计
int get_and_count_lines(const char *file_name)
{
	assert(file_name);
	
	int line_count = 0;//此文件中非空行数
	char buffer[MAX_BUF_LEN] = "\0";
	FILE *file_in = NULL;
	
	//以只读的方式打开文件
	if ((file_in = fopen(file_name, "r")))
	{
		while (!feof(file_in))//读取到文件流的结束
		{
			//不加判定,会多读取一次最后一行
			if ((fgets(buffer, sizeof(buffer), file_in)))
			{
				//如果读取的这一行不是空行,算入计数
				if (!is_blank_line(buffer))
				{
					line_count++;	
				}	
			}
		}
	}
	return line_count;
}

//检测读取到buff中的一行是不是单行注释
boolean check_is_single_line_comment(const char *buff, int buf_size)
{
	assert(buff);

	for (int i = 0; i < buf_size; i++)	
	{
		//前面是空格,就忽略,如果其前面有其他不是//的字符,那就说明这一行是有效代码
		if (buff[i] == ' ' || buff[i] == '\t')
		{
			continue;
		}
		else if (buff[i] == '/' && buff[i + 1] == '/')//二个连续单行注释
		{
			return TRUE;	
		}
		//一行中的/* some comment */形式,也算单行注释
		else if (buff[i] == '/' && buff[i + 1] == '*'
				&& buff[buf_size - 3] == '*' && buff[buf_size - 2] == '/')
		{
			return TRUE;
		}
	}
	return FALSE;//不是上面的情况,这行就不是单行注释
}
//检测是不是多行注释,会设置全局变量next_line_is_comment
boolean check_is_multiple_line_comment(const char *buff, int buf_size)
{
	assert(buff);

	//如果全局变量next_line_is_comment == false的时候,就是上一行不是多行注释
	if (!next_line_is_comment)
	{
		for (int i = 0; i < buf_size; i++)
		{
			// 以/*说明多行注释开始, 并且结尾没有*/这二个字符
			if (buff[i] == '/' && buff[i + 1] == '*' && 
					(buff[buf_size - 3] != '*' || buff[buf_size - 2] != '/'))
			{
				//再同结尾扫描,防止出现/* some comment */ int i = 0;这种注释情况
				for (int j = buf_size; j > i + 1; j--)
				{
					if (buff[j] == '/' && buff[j - 1] == '*')
					{
						next_line_is_comment = FALSE;
						return FALSE;//是一个单行注释
					}
				}
				//下一行是注释
				next_line_is_comment = TRUE;
				return TRUE;
			}
		}
		//没有找到开始行,本行不是注释行
		return FALSE;
	}
	else//next_line_is_comment == true,说明上一行是注释
	{
		for (int i = 0; i < buf_size; i++)
		{
			//在上一行是多行注释的情况下找到了多行注释的结尾
			if (buff[i] == '*' && buff[i + 1] == '/')
			{
				next_line_is_comment = FALSE;
				return TRUE;
			}
		}
		//没找到多行注释的结束, 本行还是注释行
		return TRUE;
	}
}


int main(int argc, const char *argv[])
{
	//list_dir_files("/Users/bugcode/test_get", "c");
	//get_file_extension_name("test.c", extension);
	//printf("line_count == %d\n", get_and_count_lines("strlen_test.c"));
	//printf("total_lines == %d\n", total_lines);
	char buf[512] = "\0";
	FILE *fp = fopen("test.c", "r");
	while (!feof(fp))
	{
		if (fgets(buf, sizeof(buf), fp))
		{
			int len = strlen(buf);
			if (check_is_single_line_comment(buf, len) || check_is_multiple_line_comment(buf, len))
			{
				printf("It's comment line!\n");
			}
			else
			{
				printf("It's not comment line!\n");
			}
		}
	}
	
	return 0;
}
