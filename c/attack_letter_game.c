#include<gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#define WIN_WID 500
#define WIN_HEI 400
#define COLOR_BITS 7

//全局游戏区
GtkWidget *frame = NULL;
GtkWidget *window = NULL;

//游戏字母结构，包括位置和label还有定时器id信息
typedef struct letter_info 
{
	GtkWidget *label;//承载letter的label
	gchar disp_char;//label要显示的char
	gint x_pos, y_pos;//label的位置
	gint timer_id;//letter对应的定时器id
	gchar markup_str[128];//letter对应的label的属性
	gboolean press_correct;
} LetterWidget;

void letter_put(LetterWidget *letter);
LetterWidget *new_letter(gint x_pos, gint y_pos);
gint letter_move(gpointer _letter);
gboolean out_bound(gint x_pos, gint bound);
void game_continue();
void clear_label(GtkWidget *label);
gint rand_num(gint s, gint e);
gchar *rand_color(gint count);
gchar rand_letter();
gboolean window_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data);


//画游戏区边框
gboolean draw_rectangle_border(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GdkGC *gc = gdk_gc_new(GDK_DRAWABLE(widget->window));
	GdkColor color;
	gdk_color_parse("red", &color);
	gdk_gc_set_line_attributes(gc, 3, GDK_LINE_SOLID, GDK_CAP_ROUND, GDK_JOIN_MITER);
	gdk_gc_set_rgb_fg_color(gc, &color);
	gdk_draw_rectangle(widget->window, gc, FALSE, 10, 10, 500, 370);
	gtk_widget_queue_draw(widget);
	return TRUE;
}

//键盘响应事件
gboolean window_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
	LetterWidget *letter = (LetterWidget *)data;
	guint key_val = event->keyval;//获取键值
	//如果按键和label上相同，刚表示按键正确
	if (letter->disp_char == gdk_keyval_to_upper(key_val))
		letter->press_correct = TRUE;

	return FALSE;
}

//产生随机位置
gint rand_num(gint s, gint e)
{
	gint xpos = rand() % e + s;
	return xpos;
}
//游戏初始化
void game_continue()
{
	LetterWidget *new = new_letter(rand_num(0, WIN_WID - 50), 5);
	//s为新的letter放到frame
	letter_put(new);
}
//清除label上的标签
void clear_label(GtkWidget *label)
{
	gtk_label_set_label(GTK_LABEL(label), "");
}
//检测是否出边界
gboolean out_bound(gint x_pos, gint bound)
{
	return (x_pos > bound);
}
//定时器回调，移动label
gint letter_move(gpointer _letter)
{
	LetterWidget *letter = (LetterWidget *)_letter;
	gtk_fixed_move(GTK_FIXED(frame), GTK_WIDGET(letter->label), letter->x_pos, letter->y_pos += 2);
	if (out_bound(letter->y_pos, WIN_HEI - 100) || letter->press_correct)
	{
		clear_label(letter->label);//letter到底，消去
		g_source_remove(letter->timer_id);//移除定时器
		g_free((gpointer)letter);//释放内在
		letter = NULL;
		game_continue();//游戏继续
	}
	return TRUE;
}
//产生一个随机十六进制字符串
gchar *rand_color(gint count)
{
	static gchar color_str[10] = "\0";
	assert(color_str != NULL);
	gchar hex_str[] = "0123456789ABCDEF";
	color_str[0] = '#';
	for (gint i = 1; i < count; i++)
	{
		color_str[i] = hex_str[rand() % 15];//产生随机颜色字符串
	}
	g_print("%s\n", color_str);
	return color_str;
}

//产生随机字母
gchar rand_letter()
{
	return rand() % 26 + 'A';//产生随机26个大写字母
}


//产生一个新的letter结构，并初始化内部数据
LetterWidget *new_letter(gint x_pos, gint y_pos)
{
	LetterWidget *new = g_new0(LetterWidget, 1);//为新letter分配空间
	memset(new->markup_str, sizeof(new->markup_str), '\0');//初始化属性字符串
	new->press_correct = FALSE;//按键不正确
	new->disp_char = rand_letter();//初始化label上的字母

	//生成对应的属性串
	strcpy(new->markup_str, g_markup_printf_escaped("<span foreground='%s' weight='%s' size='%d'>%c</span>",
													rand_color(COLOR_BITS), "BOLD", rand_num(15000, 25000), new->disp_char));

	new->label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(new->label), new->markup_str);//设置letter属性
	gtk_widget_set_size_request(GTK_WIDGET(new->label), 60, 60);
	g_signal_connect(G_OBJECT(window), "key_press_event", G_CALLBACK(window_key_press_event), new);
	new->x_pos = x_pos;
	new->y_pos = y_pos;
	new->timer_id = g_timeout_add(20, letter_move, new);//定时器设置

	return new;
}

//在游戏区放置对应的label
void letter_put(LetterWidget *letter)
{
	assert(letter != NULL);
	//放置结构里的label
	gtk_fixed_put(GTK_FIXED(frame), letter->label, letter->x_pos, letter->y_pos);
	gtk_widget_show(letter->label);
}


int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_move(GTK_WINDOW(window), 500, 90);
	gtk_widget_set_size_request(GTK_WIDGET(window), WIN_WID + 100, WIN_HEI);
	gtk_window_set_title(GTK_WINDOW(window), "LabelTest");
	g_signal_connect(G_OBJECT(window), "expose_event", G_CALLBACK(draw_rectangle_border), NULL);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	gtk_widget_show(window);
	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frame);
	gtk_widget_show(frame);

	/*letter_put(new_letter('A', rand_num(0, WIN_WID), 10));*/
	game_continue();
		
	gtk_main();
	return 0;
}



