/*=============================================================================
#     FileName: sweep_mine.c
#         Desc: smiple sweep game by gtk
#       Author: zx
#        Email: bugcoding@hotmail.com
#     HomePage: http://bugcode.net
#      Version: 0.0.1
#   LastChange: 2013-01-01 11:47:05
=============================================================================*/
#include<gtk/gtk.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define WID 30
#define HEI 30

#define MINE -1

#define DEBUG printf("%s(%d)--%s\n", __FILE__, __LINE__, __func__);


//button位置
typedef struct
{
	gint x_pos;
	gint y_pos;
}button_pos;

static gint button_row_num = 0;//每行有多少button
static gint button_col_num = 0;//每列有多少button

static gint time_info = 0;//时间显示
gint time_id = 0;//定时器id
static gint mine_num = 0;//雷的总数
static gint real_find_mine = 0;//真正找到雷的数目
static gint mine_find = 0;//标记的雷数目
GtkWidget *label_find_num = NULL;//标记雷显示的label
GtkWidget *label_time = NULL;//显示时间
gchar find_str[8] = "\0";
GtkWidget *dialog = NULL;//弹窗

gboolean **is_press_right;//每个按钮不否右击过

gint **game_area;//雷区数字表示
GtkWidget ***button_array;//按钮数组
button_pos **pos;//坐标点数组

gboolean disp_time(gpointer data);
void malloc_arrays();
void init_global();
void free_res();
void init_game_info();
gboolean check_bound(gint x_pos, gint y_pos);
void spread_zero_zone(gint x_pos, gint y_pos);
gint check_around(button_pos pos);
void fill_mine_info();
void init_area();
void reset_interface(GtkWidget *widget, gpointer data);
void quit_dialog(gchar *message);
void put_image_button(GtkContainer *con, gchar *file_name);
void open_all_button();
void disp_none_border();
void left_press_button(GtkWidget *widget, button_pos *pos_data);
void disp_find_mine(gint mine_num);
void right_press_button(GtkWidget *widget, button_pos *pos_data);
void click_button(GtkWidget *widget, GdkEventButton *event, gpointer pos);

GtkWidget *create_table(gint row, gint col);
GtkWidget *new_window(gchar *title, gint width, gint height);
GtkWidget *label_panel_info(gint num);


void malloc_arrays()
{
	//按钮数组空间
	button_array = (GtkWidget ***)malloc(sizeof(GtkWidget **) * button_row_num);
	assert(button_array != NULL);
	for (gint i = 0; i < button_row_num; i++)
	{
		*(button_array + i) = (GtkWidget **)malloc(sizeof(GtkWidget *) * button_col_num);
		assert(*(button_array + i) != NULL);
	}
	//游戏区空间
	game_area = (gint **)malloc(sizeof(gint *) * button_row_num);
	assert(game_area != NULL);
	for (gint i = 0; i < button_row_num; i++)
	{
		*(game_area + i) = (gint *)malloc(sizeof(gint) * button_col_num);
		assert(*(game_area + i) != NULL);
	}

	//右键按下判断
	is_press_right = (gboolean **)malloc(sizeof(gboolean *) * button_row_num);
	assert(is_press_right != NULL);
	for (gint i = 0; i < button_row_num; i++)
	{
		*(is_press_right + i) = (gboolean *)malloc(sizeof(gboolean) * button_col_num);
		assert(*(is_press_right + i) != NULL);
	}
	//坐标点数组
	pos = (button_pos **)malloc(sizeof(button_pos *) * button_row_num);
	for (gint i = 0; i < button_row_num; i++)
	{
		*(pos + i) = (button_pos *)malloc(sizeof(button_pos) * button_col_num);
	}

}

//初始化其他全局变量
void init_global()
{
	button_row_num = 20;
	button_col_num = 20;
	mine_num = 50;
	real_find_mine = 0;
	mine_find = 0;
	time_info = 0;
	time_id = g_timeout_add(1000, disp_time, NULL);//游戏初始化时加定时器
}
void init_arrays()
{
	for (gint i = 0; i < button_row_num; i++)
	{
		for (gint j = 0; j < button_col_num; j++)
		{
			game_area[i][j] = 0;
			is_press_right[i][j] = FALSE;
		}
	}

}
//释放动态分配的数组
void free_res()
{
	for (gint i = 0; i < button_row_num; i++)
	{
		free(button_array[i]);
		free(game_area[i]);
		free(is_press_right[i]);
		free(pos[i]);
	}
	free(button_array);
	free(game_area);
	free(is_press_right);
	free(pos);
}

//初始化各种游戏数据及分配数据空间
void init_game_info()
{
	init_global();
	//分配空间
	malloc_arrays();
	init_arrays();
	//初始化变量
	init_area();//创建表格填充按钮的同时布置好雷区

}

//控制台临时打印游戏区的布置， 方便调试
void print()
{
	for (gint i = 0; i < button_row_num; i++)
	{
		for (gint j = 0; j < button_col_num; j++)
		{
			g_print("%3d", game_area[j][i]);
		}
		g_print("\n");
	}
}

//为递归展开zero_zone做边界检查
gboolean check_bound(gint x_pos, gint y_pos)
{

	if (x_pos < 0 || x_pos >= button_col_num || y_pos < 0 || y_pos >= button_row_num)
	{
		return FALSE;
	}
	return TRUE;
}

//连续展开为0的无数字无雷区
void spread_zero_zone(gint x_pos, gint y_pos)
{
	//使用递归的方式展开相连的空白区域
	if (check_bound(x_pos - 1, y_pos) && game_area[x_pos - 1][y_pos] == 0 && 
		gtk_button_get_relief(GTK_BUTTON(button_array[x_pos - 1][y_pos])) != GTK_RELIEF_NONE)
	{
		gtk_button_set_relief(GTK_BUTTON(button_array[x_pos - 1][y_pos]), GTK_RELIEF_NONE);
		spread_zero_zone(x_pos - 1, y_pos);
	}
	if (check_bound(x_pos + 1, y_pos) && game_area[x_pos + 1][y_pos] == 0 && 
		gtk_button_get_relief(GTK_BUTTON(button_array[x_pos + 1][y_pos])) != GTK_RELIEF_NONE)
	{
		gtk_button_set_relief(GTK_BUTTON(button_array[x_pos + 1][y_pos]), GTK_RELIEF_NONE);
		spread_zero_zone(x_pos + 1, y_pos);
	}
	if (check_bound(x_pos, y_pos - 1) && game_area[x_pos][y_pos - 1] == 0 && 
		gtk_button_get_relief(GTK_BUTTON(button_array[x_pos][y_pos - 1])) != GTK_RELIEF_NONE)
	{
		gtk_button_set_relief(GTK_BUTTON(button_array[x_pos][y_pos - 1]), GTK_RELIEF_NONE);
		spread_zero_zone(x_pos, y_pos - 1);
	}
	if (check_bound(x_pos, y_pos + 1) && game_area[x_pos][y_pos + 1] == 0 && 
		gtk_button_get_relief(GTK_BUTTON(button_array[x_pos][y_pos + 1])) != GTK_RELIEF_NONE)
	{
		gtk_button_set_relief(GTK_BUTTON(button_array[x_pos][y_pos + 1]), GTK_RELIEF_NONE);
		spread_zero_zone(x_pos, y_pos + 1);
	}
}


//检查某个点周围是否有雷的存在
gint check_around(button_pos pos)
{
	//pos点周围雷的个数，初始为0
	gint mine_num = 0;
	//分别在pos的周围找雷
	if (check_bound(pos.x_pos - 1, pos.y_pos) && game_area[pos.x_pos - 1][pos.y_pos] == MINE)		
	{
		mine_num++;
	}
	if (check_bound(pos.x_pos - 1, pos.y_pos - 1) && game_area[pos.x_pos - 1][pos.y_pos - 1] == MINE)
	{
		mine_num++;
	}
	if( check_bound(pos.x_pos, pos.y_pos - 1) && game_area[pos.x_pos][pos.y_pos - 1] == MINE)
	{
		mine_num++;
	}
	if (check_bound(pos.x_pos + 1, pos.y_pos - 1) && game_area[pos.x_pos + 1][pos.y_pos - 1] == MINE)
	{
		mine_num++;
	}

	if (check_bound(pos.x_pos + 1, pos.y_pos) && game_area[pos.x_pos + 1][pos.y_pos] == MINE)		
	{
		mine_num++;
	}
	if (check_bound(pos.x_pos + 1, pos.y_pos + 1) && game_area[pos.x_pos + 1][pos.y_pos + 1] == MINE)
	{
		mine_num++;
	}
	if(check_bound(pos.x_pos, pos.y_pos + 1) && game_area[pos.x_pos][pos.y_pos + 1] == MINE)
	{
		mine_num++;
	}
	if (check_bound(pos.x_pos - 1, pos.y_pos + 1) && game_area[pos.x_pos - 1][pos.y_pos + 1] == MINE)
	{
		mine_num++;
	}
	//返回pos点周围雷的个数
	return mine_num;
}

//初始化雷区数字与fill_number_mine作用相同
void fill_mine_info()
{
	button_pos pos;
	for (gint i = 0; i < button_row_num; i++)
	{
		for (gint j = 0; j < button_col_num; j++)
		{
			pos.x_pos = i;
			pos.y_pos = j;
			//当前位置不能是雷，并且不能越界
			if (game_area[i][j] != MINE && check_around(pos))
			{
				//当前位置周围有几个雷
				game_area[i][j] += check_around(pos);
			}
		}
	}

}

//初始化雷区
void init_area()
{
	button_pos pos;
	srand((unsigned)time(NULL));
	for (gint i = 0; i < mine_num; )
	{
		pos.x_pos = (rand() % (button_col_num * button_row_num)) / button_row_num;
		pos.y_pos = (rand() % (button_row_num * button_col_num)) % button_row_num;
		g_print("x_pos: %d, y_pos:%d\n", pos.x_pos, pos.y_pos);

		while (game_area[pos.x_pos][pos.y_pos] == MINE)//按照散列的规列布雷，如果随机出来的点已经布雷，那么就将此雷安排下此点的下一个点,类似散列表
		{
			pos.x_pos++;
			pos.x_pos %= button_row_num;
		}
		game_area[pos.x_pos][pos.y_pos] = MINE;//散列后将雷安置
		i++;
	}
	//布置好雷的位置，填充雷旁边的数字
	fill_mine_info();
}

//重新初始化窗体
void reset_interface(GtkWidget *widget, gpointer data)
{
	gtk_widget_hide(data);//隐藏子窗体
	init_global();//重新初始化全局变量
	init_arrays();
	init_area();//初始化游戏区

	//button形状复原
	for (gint i = 0; i < button_row_num; i++)
	{
		for (gint j = 0; j < button_col_num; j++)
		{
			gtk_button_set_relief(GTK_BUTTON(button_array[i][j]), GTK_RELIEF_NORMAL);
			gtk_button_set_label(GTK_BUTTON(button_array[i][j]), "");
		}
	}
	print();
}
//退出弹窗提示
void quit_dialog(gchar *message)
{
	assert(message != NULL);
	GtkWidget *frame = NULL;
	GtkWidget *quit_button = NULL;
	GtkWidget *again_button = NULL;
	GtkWidget *label = NULL;

	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
	gtk_window_set_title(GTK_WINDOW(dialog), message);
	gtk_widget_set_size_request(dialog, 200, 100);
	frame = gtk_fixed_new();
	label = gtk_label_new_with_mnemonic(message);
	quit_button = gtk_button_new_with_label("Quit");
	again_button = gtk_button_new_with_label("Again");
	gtk_widget_set_size_request(quit_button, 50, 30);
	gtk_widget_set_size_request(again_button, 50, 30);
	g_signal_connect(G_OBJECT(quit_button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(again_button), "clicked", G_CALLBACK(reset_interface), dialog);
	gtk_fixed_put(GTK_FIXED(frame), quit_button, 125, 60);
	gtk_fixed_put(GTK_FIXED(frame), again_button, 25, 60);
	gtk_fixed_put(GTK_FIXED(frame), label, 10, 20);
	gtk_container_add(GTK_CONTAINER(dialog), frame);
	g_signal_connect(G_OBJECT(dialog), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(dialog);
}

//button上放图片
void put_image_button(GtkContainer *con, gchar *file_name)
{
	assert(file_name != NULL);
	GtkWidget *img = gtk_image_new_from_file(file_name);
	gtk_container_set_border_width(GTK_CONTAINER(con), 1);
	gtk_container_add(GTK_CONTAINER(con), img);
}
//点到雷后，翻开所有的button
void open_all_button()
{
	char temp[5] = "\0";
	for (gint i = 0; i < button_row_num; i++)
	{
		for (gint j = 0; j < button_col_num; j++)
		{
			gtk_button_set_relief(GTK_BUTTON(button_array[i][j]), GTK_RELIEF_NONE);
			if (game_area[i][j] == MINE)
			{
				gtk_button_set_label(GTK_BUTTON(button_array[i][j]), "X");
			}
			else if (game_area[i][j] == 0)
			{
				gtk_button_set_label(GTK_BUTTON(button_array[i][j]), "");
			}
			else
			{
				sprintf(temp, "%d", game_area[i][j]);
				gtk_button_set_label(GTK_BUTTON(button_array[i][j]), temp);	
			}
		}
	}
}

//空白部分边缘的数字显示，像win下的扫雷
void disp_none_border()
{
	gchar temp[5] = "\0";
	for (gint i = 0; i < button_row_num; i++)
	{
		for (gint j = 0; j < button_col_num; j++)
		{
			if (game_area[i][j] == 0 && gtk_button_get_relief(GTK_BUTTON(button_array[i][j])) == GTK_RELIEF_NONE)
			{
				if (check_bound(i - 1, j) && game_area[i - 1][j] != 0 && game_area[i - 1][j] != MINE)
				{
					gtk_button_set_relief(GTK_BUTTON(button_array[i - 1][j]), GTK_RELIEF_NONE);
					sprintf(temp, "%d", game_area[i - 1][j]);
					gtk_button_set_label(GTK_BUTTON(button_array[i - 1][j]), temp);
				}
				if (check_bound(i, j - 1) && game_area[i][j - 1] != 0 && game_area[i][j - 1] != MINE)
				{
					gtk_button_set_relief(GTK_BUTTON(button_array[i][j - 1]), GTK_RELIEF_NONE);
					sprintf(temp, "%d", game_area[i][j - 1]);
					gtk_button_set_label(GTK_BUTTON(button_array[i][j - 1]), temp);
				}
				if (check_bound(i + 1, j) && game_area[i + 1][j] != 0 && game_area[i + 1][j] != MINE)
				{
					gtk_button_set_relief(GTK_BUTTON(button_array[i + 1][j]), GTK_RELIEF_NONE);
					sprintf(temp, "%d", game_area[i + 1][j]);
					gtk_button_set_label(GTK_BUTTON(button_array[i + 1][j]), temp);
				}
				if (check_bound(i, j + 1) && game_area[i][j + 1] != 0 && game_area[i][j + 1] != MINE)
				{
					gtk_button_set_relief(GTK_BUTTON(button_array[i][j + 1]), GTK_RELIEF_NONE);
					sprintf(temp, "%d", game_area[i][j + 1]);
					gtk_button_set_label(GTK_BUTTON(button_array[i][j + 1]), temp);
				}

			}
		}
	}
}


//左键处理
void left_press_button(GtkWidget *widget, button_pos *pos_data)
{
	char label[3] = "\0";
		//button没有被右键按下，并且右键没有按下，
	if (gtk_button_get_relief(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos])) != GTK_RELIEF_NONE
			&& !is_press_right[pos_data->x_pos][pos_data->y_pos])
	{
		//点到雷的情况
		if (game_area[pos_data->x_pos][pos_data->y_pos] == MINE)
		{
			gtk_button_set_label(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos]), "X");
			g_source_remove(time_id);//遇到雷，定时器暂停
			open_all_button();//爆炸打开全部button
			quit_dialog("Beng.....");
		}
		else
		{
			gtk_button_set_relief(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos]), GTK_RELIEF_NONE);
			//没有雷，只是数字，就显示数字
			if (game_area[pos_data->x_pos][pos_data->y_pos] != 0)
			{
				sprintf(label, "%d", game_area[pos_data->x_pos][pos_data->y_pos]);
				gtk_button_set_label((GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos])), label);
			}
			else
			{
				//没有数字也没有雷，就显示空,并同时展开相连的空白区域
				gtk_button_set_label((GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos])), "");
				spread_zero_zone(pos_data->x_pos, pos_data->y_pos);
				//显示空白边缘数字
				disp_none_border();
			}
		}
	}
}

//找到雷的数目显示
void disp_find_mine(gint mine_num)
{
	sprintf(find_str, "%d", mine_num);
	gtk_label_set_label(GTK_LABEL(label_find_num), find_str);
}

//右键处理
void right_press_button(GtkWidget *widget, button_pos *pos_data)
{
	//button没有被左键按下，并且没按下右键
	if (gtk_button_get_relief(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos])) != GTK_RELIEF_NONE
			&& !is_press_right[pos_data->x_pos][pos_data->y_pos])
	{
		gtk_button_set_label(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos]), "F");
		if (game_area[pos_data->x_pos][pos_data->y_pos] == MINE)//每找到一个雷标记就加1
		{
			real_find_mine++;
			if (real_find_mine == mine_num)//全部找到后弹出窗口提示
			{
				quit_dialog("You Win!");
			}
		}
		disp_find_mine(++mine_find);//标记后，将找到雷的数目加1

		is_press_right[pos_data->x_pos][pos_data->y_pos] = TRUE;//标示此button被按下
	}
	else if(is_press_right[pos_data->x_pos][pos_data->y_pos])//button被右键按下，再按右键会恢复
	{
		gtk_button_set_label(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos]), "");
		disp_find_mine(--mine_find);//取消标记后找到雷的数目减1
		if (game_area[pos_data->x_pos][pos_data->y_pos] == MINE)//取消标记对应的减1
		{
			real_find_mine--;
		}
		is_press_right[pos_data->x_pos][pos_data->y_pos] = FALSE;//恢复后标示未被右键按下
	}
}

//button按下事件处理
void click_button(GtkWidget *widget, GdkEventButton *event, gpointer pos)
{
	if (event->type == GDK_BUTTON_PRESS)
	{
		if (event->button == 1)//左键事件
		{
			left_press_button(widget, (button_pos *)pos);
		}
		if (event->button == 3)//右键事件
		{
			right_press_button(widget, (button_pos *)pos);
		}
	}

	if (event->type == GDK_2BUTTON_PRESS)
	{
		g_print("double press!\n");
		/*quit_dialog("double clicked");*/
	}
}
//安排界面
GtkWidget *create_table(gint row, gint col)
{
	GtkWidget *table = NULL;
	
	//不加static，此函数运行完毕，界面出现，但是对应的位置信息全部从堆栈中消失，会出现传值错误


	table = gtk_table_new(row, col, TRUE);
	//布置button
	for (gint i = 0; i < row; i++)
	{
		for (gint j = 0; j < col; j++)
		{
			pos[i][j].x_pos = i;
			pos[i][j].y_pos = j;
			button_array[i][j] = gtk_button_new();
			gtk_widget_set_size_request(button_array[i][j], WID, HEI);
			/*put_image_button(GTK_CONTAINER(button_array[i][j]), "beng.png");*/
			g_signal_connect(G_OBJECT(button_array[i][j]), "button_press_event", G_CALLBACK(click_button), &pos[i][j]);
			gtk_table_attach_defaults(GTK_TABLE(table), button_array[i][j], i, i + 1, j, j + 1);
		}
	}
	return table;
}

//显示主窗体
GtkWidget *new_window(gchar *title, gint width, gint height)
{
	GtkWidget *window = NULL;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), title);
	gtk_window_move(GTK_WINDOW(window), 350, 20);
	
	return window;
}
//游戏窗体上的label信息
GtkWidget *label_panel_info(gint num)
{
	GtkWidget *label_info = NULL;
	gchar num_str[8] = "\0";

	sprintf(num_str, "%d", num);
	label_info = gtk_label_new_with_mnemonic(num_str);
	
	return label_info;
}

//显示时间
gboolean disp_time(gpointer data)
{
	gchar time_str[5] = "\0";
	sprintf(time_str, "%d", time_info);
	gtk_label_set_label(GTK_LABEL(label_time), time_str);
	time_info ++;
	return TRUE;
}

int main(int argc, char *argv[])
{
	GtkWidget *frame = NULL;
	GtkWidget *window = NULL;
	GtkWidget *button = NULL;

	init_game_info();//初始化信息
	gtk_init(&argc, &argv);
	
	//窗体布置
	window = new_window("SweepMine", button_row_num * WID, button_col_num * HEI + 5);
	//加一个框架，用于灵活摆放其他构件	
	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frame);
	
	
	//用table规范button位置
	GtkWidget *table = create_table(button_row_num, button_col_num);

	gtk_fixed_put(GTK_FIXED(frame), table, 0, 35);
	
	button = gtk_button_new_with_label("Re");
	gtk_widget_set_size_request(button, 50, 25);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(reset_interface), NULL);
	gtk_fixed_put(GTK_FIXED(frame), button, 230, 6);

	//雷的总数
	gtk_fixed_put(GTK_FIXED(frame), label_panel_info(mine_num), 370, 8);

	//显示标记的雷
	label_find_num = label_panel_info(mine_find);
	gtk_fixed_put(GTK_FIXED(frame), label_find_num, 20, 8);

	//label time
	label_time = gtk_label_new(NULL);
	gtk_fixed_put(GTK_FIXED(frame), label_time, 80, 8);
	

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_show_all(window);
	print();
	gtk_main();
	free_res();
	return 0;
}



