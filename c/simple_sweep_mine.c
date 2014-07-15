/*=============================================================================
#     FileName: buttonTest.c
	������ɢ�з��������arrayΪM*N,��

	���Ȳ���һ��[0,M*N��֮��������R�����ż�� 
	array[R/N][R%N]���Ǹ����ӣ�Ҫ���Ǹ������Ѿ��������ף�
	������֮��ĵ�һ��Ϊ�յĸ��ӡ������׵���Ŀ�����M*N��Ŀ��һ��ʱ�����ַ�����Ч��Ҳ��Ƚϸߡ�
#         Desc:  
#       Author: zx
#        Email: zx_start@163.com
#     HomePage: 
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


//buttonλ��
typedef struct
{
	gint x_pos;
	gint y_pos;
}button_pos;

static gint button_row_num = 0;//ÿ���ж���button
static gint button_col_num = 0;//ÿ���ж���button

static gint time_info = 0;//ʱ����ʾ
gint time_id = 0;//��ʱ��id
static gint mine_num = 0;//�׵�����
static gint real_find_mine = 0;//�����ҵ��׵���Ŀ
static gint mine_find = 0;//��ǵ�����Ŀ
GtkWidget *label_find_num = NULL;//�������ʾ��label
GtkWidget *label_time = NULL;//��ʾʱ��
gchar find_str[8] = "\0";
GtkWidget *dialog = NULL;//����

gboolean **is_press_right;//ÿ����ť�����һ���

gint **game_area;//�������ֱ�ʾ
GtkWidget ***button_array;//��ť����
button_pos **pos;//���������

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
	//��ť����ռ�
	button_array = (GtkWidget ***)malloc(sizeof(GtkWidget **) * button_row_num);
	assert(button_array != NULL);
	for (gint i = 0; i < button_row_num; i++)
	{
		*(button_array + i) = (GtkWidget **)malloc(sizeof(GtkWidget *) * button_col_num);
		assert(*(button_array + i) != NULL);
	}
	//��Ϸ���ռ�
	game_area = (gint **)malloc(sizeof(gint *) * button_row_num);
	assert(game_area != NULL);
	for (gint i = 0; i < button_row_num; i++)
	{
		*(game_area + i) = (gint *)malloc(sizeof(gint) * button_col_num);
		assert(*(game_area + i) != NULL);
	}

	//�Ҽ������ж�
	is_press_right = (gboolean **)malloc(sizeof(gboolean *) * button_row_num);
	assert(is_press_right != NULL);
	for (gint i = 0; i < button_row_num; i++)
	{
		*(is_press_right + i) = (gboolean *)malloc(sizeof(gboolean) * button_col_num);
		assert(*(is_press_right + i) != NULL);
	}
	//���������
	pos = (button_pos **)malloc(sizeof(button_pos *) * button_row_num);
	for (gint i = 0; i < button_row_num; i++)
	{
		*(pos + i) = (button_pos *)malloc(sizeof(button_pos) * button_col_num);
	}

}

//��ʼ������ȫ�ֱ���
void init_global()
{
	button_row_num = 20;
	button_col_num = 20;
	mine_num = 50;
	real_find_mine = 0;
	mine_find = 0;
	time_info = 0;
	time_id = g_timeout_add(1000, disp_time, NULL);//��Ϸ��ʼ��ʱ�Ӷ�ʱ��
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
//�ͷŶ�̬���������
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

//��ʼ��������Ϸ���ݼ��������ݿռ�
void init_game_info()
{
	init_global();
	//����ռ�
	malloc_arrays();
	init_arrays();
	//��ʼ������
	init_area();//���������䰴ť��ͬʱ���ú�����

}

//����̨��ʱ��ӡ��Ϸ���Ĳ��ã� �������
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

//Ϊ�ݹ�չ��zero_zone���߽���
gboolean check_bound(gint x_pos, gint y_pos)
{

	if (x_pos < 0 || x_pos >= button_col_num || y_pos < 0 || y_pos >= button_row_num)
	{
		return FALSE;
	}
	return TRUE;
}

//����չ��Ϊ0��������������
void spread_zero_zone(gint x_pos, gint y_pos)
{
	//ʹ�õݹ�ķ�ʽչ�������Ŀհ�����
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


//���ĳ������Χ�Ƿ����׵Ĵ���
gint check_around(button_pos pos)
{
	//pos����Χ�׵ĸ�������ʼΪ0
	gint mine_num = 0;
	//�ֱ���pos����Χ����
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
	//����pos����Χ�׵ĸ���
	return mine_num;
}

//��ʼ������������fill_number_mine������ͬ
void fill_mine_info()
{
	button_pos pos;
	for (gint i = 0; i < button_row_num; i++)
	{
		for (gint j = 0; j < button_col_num; j++)
		{
			pos.x_pos = i;
			pos.y_pos = j;
			//��ǰλ�ò������ף����Ҳ���Խ��
			if (game_area[i][j] != MINE && check_around(pos))
			{
				//��ǰλ����Χ�м�����
				game_area[i][j] += check_around(pos);
			}
		}
	}

}

//��ʼ������
void init_area()
{
	button_pos pos;
	srand((unsigned)time(NULL));
	for (gint i = 0; i < mine_num; )
	{
		pos.x_pos = (rand() % (button_col_num * button_row_num)) / button_row_num;
		pos.y_pos = (rand() % (button_row_num * button_col_num)) % button_row_num;
		g_print("x_pos: %d, y_pos:%d\n", pos.x_pos, pos.y_pos);

		while (game_area[pos.x_pos][pos.y_pos] == MINE)//����ɢ�еĹ��в��ף������������ĵ��Ѿ����ף���ô�ͽ����װ����´˵����һ����,����ɢ�б�
		{
			pos.x_pos++;
			pos.x_pos %= button_row_num;
		}
		game_area[pos.x_pos][pos.y_pos] = MINE;//ɢ�к��װ���
		i++;
	}
	//���ú��׵�λ�ã�������Աߵ�����
	fill_mine_info();
}

//���³�ʼ������
void reset_interface(GtkWidget *widget, gpointer data)
{
	gtk_widget_hide(data);//�����Ӵ���
	init_global();//���³�ʼ��ȫ�ֱ���
	init_arrays();
	init_area();//��ʼ����Ϸ��

	//button��״��ԭ
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
//�˳�������ʾ
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

//button�Ϸ�ͼƬ
void put_image_button(GtkContainer *con, gchar *file_name)
{
	assert(file_name != NULL);
	GtkWidget *img = gtk_image_new_from_file(file_name);
	gtk_container_set_border_width(GTK_CONTAINER(con), 1);
	gtk_container_add(GTK_CONTAINER(con), img);
}
//�㵽�׺󣬷������е�button
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

//�հײ��ֱ�Ե��������ʾ����win�µ�ɨ��
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


//�������
void left_press_button(GtkWidget *widget, button_pos *pos_data)
{
	char label[3] = "\0";
		//buttonû�б��Ҽ����£������Ҽ�û�а��£�
	if (gtk_button_get_relief(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos])) != GTK_RELIEF_NONE
			&& !is_press_right[pos_data->x_pos][pos_data->y_pos])
	{
		//�㵽�׵����
		if (game_area[pos_data->x_pos][pos_data->y_pos] == MINE)
		{
			gtk_button_set_label(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos]), "X");
			g_source_remove(time_id);//�����ף���ʱ����ͣ
			open_all_button();//��ը��ȫ��button
			quit_dialog("Beng.....");
		}
		else
		{
			gtk_button_set_relief(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos]), GTK_RELIEF_NONE);
			//û���ף�ֻ�����֣�����ʾ����
			if (game_area[pos_data->x_pos][pos_data->y_pos] != 0)
			{
				sprintf(label, "%d", game_area[pos_data->x_pos][pos_data->y_pos]);
				gtk_button_set_label((GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos])), label);
			}
			else
			{
				//û������Ҳû���ף�����ʾ��,��ͬʱչ�������Ŀհ�����
				gtk_button_set_label((GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos])), "");
				spread_zero_zone(pos_data->x_pos, pos_data->y_pos);
				//��ʾ�հױ�Ե����
				disp_none_border();
			}
		}
	}
}

//�ҵ��׵���Ŀ��ʾ
void disp_find_mine(gint mine_num)
{
	sprintf(find_str, "%d", mine_num);
	gtk_label_set_label(GTK_LABEL(label_find_num), find_str);
}

//�Ҽ�����
void right_press_button(GtkWidget *widget, button_pos *pos_data)
{
	//buttonû�б�������£�����û�����Ҽ�
	if (gtk_button_get_relief(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos])) != GTK_RELIEF_NONE
			&& !is_press_right[pos_data->x_pos][pos_data->y_pos])
	{
		gtk_button_set_label(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos]), "F");
		if (game_area[pos_data->x_pos][pos_data->y_pos] == MINE)//ÿ�ҵ�һ���ױ�Ǿͼ�1
		{
			real_find_mine++;
			if (real_find_mine == mine_num)//ȫ���ҵ��󵯳�������ʾ
			{
				quit_dialog("You Win!");
			}
		}
		disp_find_mine(++mine_find);//��Ǻ󣬽��ҵ��׵���Ŀ��1

		is_press_right[pos_data->x_pos][pos_data->y_pos] = TRUE;//��ʾ��button������
	}
	else if(is_press_right[pos_data->x_pos][pos_data->y_pos])//button���Ҽ����£��ٰ��Ҽ���ָ�
	{
		gtk_button_set_label(GTK_BUTTON(button_array[pos_data->x_pos][pos_data->y_pos]), "");
		disp_find_mine(--mine_find);//ȡ����Ǻ��ҵ��׵���Ŀ��1
		if (game_area[pos_data->x_pos][pos_data->y_pos] == MINE)//ȡ����Ƕ�Ӧ�ļ�1
		{
			real_find_mine--;
		}
		is_press_right[pos_data->x_pos][pos_data->y_pos] = FALSE;//�ָ����ʾδ���Ҽ�����
	}
}

//button�����¼�����
void click_button(GtkWidget *widget, GdkEventButton *event, gpointer pos)
{
	if (event->type == GDK_BUTTON_PRESS)
	{
		if (event->button == 1)//����¼�
		{
			left_press_button(widget, (button_pos *)pos);
		}
		if (event->button == 3)//�Ҽ��¼�
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
//���Ž���
GtkWidget *create_table(gint row, gint col)
{
	GtkWidget *table = NULL;
	
	//����static���˺���������ϣ�������֣����Ƕ�Ӧ��λ����Ϣȫ���Ӷ�ջ����ʧ������ִ�ֵ����


	table = gtk_table_new(row, col, TRUE);
	//����button
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

//��ʾ������
GtkWidget *new_window(gchar *title, gint width, gint height)
{
	GtkWidget *window = NULL;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), title);
	gtk_window_move(GTK_WINDOW(window), 350, 20);
	
	return window;
}
//��Ϸ�����ϵ�label��Ϣ
GtkWidget *label_panel_info(gint num)
{
	GtkWidget *label_info = NULL;
	gchar num_str[8] = "\0";

	sprintf(num_str, "%d", num);
	label_info = gtk_label_new_with_mnemonic(num_str);
	
	return label_info;
}

//��ʾʱ��
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

	init_game_info();//��ʼ����Ϣ
	gtk_init(&argc, &argv);
	
	//���岼��
	window = new_window("SweepMine", button_row_num * WID, button_col_num * HEI + 5);
	//��һ����ܣ��������ڷ���������	
	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frame);
	
	
	//��table�淶buttonλ��
	GtkWidget *table = create_table(button_row_num, button_col_num);

	gtk_fixed_put(GTK_FIXED(frame), table, 0, 35);
	
	button = gtk_button_new_with_label("Re");
	gtk_widget_set_size_request(button, 50, 25);
	g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(reset_interface), NULL);
	gtk_fixed_put(GTK_FIXED(frame), button, 230, 6);

	//�׵�����
	gtk_fixed_put(GTK_FIXED(frame), label_panel_info(mine_num), 370, 8);

	//��ʾ��ǵ���
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



