/*
一个简易的万历查询  2011-7-28 04:37pm->2011-7-29 09:03am
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>



#define BASE_YEAR 1901 			//基准年
#define LIST_LENGTH 42 			//打印列表的数组大小
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define ESC 'q'



extern 	const int every_month_day[2][13] = {{0,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
											{0,31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}}; 	//对应每个月的天数
extern const int leapyear[2] = {365, 366}; 		//对应闰年平年的天数

/*
 *函数声明
 */
void iniput(int * year, int * month, int * day);
int is_leap_year(int year);
int yeardays(int year);
int monthdays(int year, int month);
int weekday(int year, int month, int day);
int get_first_month(int year, int month);
void show_calendar(int year, int month, int day, int first_month_day);
void show_calendar_List(int year, int month, int day);
void calendar_page(int year, int month, int day);


int main(void)
{
	int year;
	int month;
	int day;
	char chin;

	do
	{
		iniput(&year, &month, &day);

		show_calendar_List(year, month, day);

		calendar_page(year,month,day);

		printf("\nDo you want to continue ? (Y/N)\n");

		chin = getch();

	}while (chin == 'Y' || chin == 'y');
	
		

	return 0;
}

void iniput(int * year, int * month, int * day) 		//进行输入日期的检查
{
	do
	{
		printf("\nPlease input the date <example: 2001-6-12>:\t");

		scanf("%d-%d-%d", year, month, day);

	}while (*year < BASE_YEAR ||*year < 0 || *month > 13 || *month < 0 || *day > every_month_day[is_leap_year(*year)][* month]);

}

int is_leap_year(int year)
{
	return  0 == year % 4 && (0 != year % 100 || 0 == year % 400); 			//判定闰年
}

int yeardays(int year)
{
	int yearsum = 0; 									//存储当前年份到基准年的天数

	for (int i = BASE_YEAR; i < year; i++)
	{
		yearsum += leapyear[is_leap_year(i)]; 			//计算当前年份到基准年的天数
	}

	return yearsum;
}

int monthdays(int year, int month)
{
	int monthsum = 0; 									//存储当前这个月到第一个月的天数

	for (int i = 1; i < month; i++)
	{
		monthsum += every_month_day[is_leap_year(year)][i]; 		//计算当前这个月到第一个月的天数
	}

	return monthsum;
}

int weekday(int year, int month, int day) 		//计算当前日期到基准日的星期
{
	const int weeknum[7] = {1,2,3,4,5,6,0};

	int weeksum = yeardays(year) + monthdays(year,month)+ day;

	int week = weeksum % 7;

	return weeknum[week];

}

int get_first_month(int year, int month)
{
	const int weeknum[7] = {1,2,3,4,5,6,0}; 			//数组的作用是对应不同余数的星期

	int weeksum = yeardays(year) + monthdays(year,month) + 1; 				//计算这个月第一天到基准日的天数

	int week = weeksum % 7;   							//计算这些天的星期余数

	return weeknum[week];

}

void show_calendar(int year, int month, int day, int first_month_day)
{
	int calendar[LIST_LENGTH];

	char sign[4] = {' ',' ', '[', ']'}; 			//为突出显示要查询的时间而设置的数组

	int flag = 0;

	for (int j = 0; j < LIST_LENGTH; j++)
	{
		calendar[j] = 0;
	}

	for (int i = 1; i <= every_month_day[is_leap_year(year)][month]; i++)
	{
		calendar[first_month_day] = i; 				//将当前这个月的第一天的星期作为数组的开始位置
		first_month_day++;
	}

	system("CLS");

	printf("\n   ------< Calendar >------   \n");
	printf("  [ EasyCalendar v0.1 by ZX ]\n\n ");

	printf("Date is : %d-%d-%d\n", year, month, day);

	printf("\n #*****************************#\n");			//边框

	printf(" $  S   M   T   W   T   F   S  $\n");

	for (int k = 0; k < LIST_LENGTH; k++)
	{
		if (k == 0)
		{
			printf(" $");					//边框
		}
		if (k == 41)
		{
			printf("     $");				//边框
		}
		if (k && (0 == k % 7)) 					//每七个换行
		{
			printf(" $");
			printf("\n");
			printf(" $");
		}

		if(calendar[k] == day) 					//判定是不是输入的那个日期如果是就标志位置2不是就置1
		{
			flag = 2;
		}
		else
		{
			flag = 0;
		}

		printf("%c", sign[flag]); 			//通过上面的判定，这行决定打印sign数组中的哪个符号


		if (calendar[k] == 0) 					//为0说明数组当前位置没有元素
		{
			printf("  ");						//因为下面的打印是%2d，所以为0的时候必须多打二个空格
		}
		else
		{
			printf("%2d", calendar[k]);
		}

		printf("%c", sign[flag + 1]);  			//打印完数字后，再补全刚才打印数组中的等号，即另一半

	}
	printf("\n #*****************************#");			//边框

	printf("\n\nPlease press a and d check the month,\n");
	printf("press w and s check year, q is exit...\n");
}

void show_calendar_List(int year, int month, int day)
{
	int first_month_day = get_first_month(year,month);

	show_calendar(year,month,day,first_month_day);
}

void calendar_page(int year, int month, int day)
{
	char key = 0;

	do
	{
		key = getch();						//捕获按键
		switch(key)
		{
		case UP:
			year--;
			show_calendar_List(year, month, day);
			break;
		case DOWN:
			year++;
			show_calendar_List(year, month, day);
			break;
		case LEFT:
			month--;
			if (month == 0)
			{
				year--;
				month = 12;
			}
			show_calendar_List(year, month, day);
			break;
		case RIGHT:
			month++;
			if (month == 13)
			{
				year++;
				month = 1;
			}
			show_calendar_List(year, month, day);
			break;
		default:
			break;
		}

	} while (key != ESC);
}
