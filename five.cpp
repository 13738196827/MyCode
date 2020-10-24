# include <stdio.h>
# include <windows.h>
# include <conio.h>
# include <time.h>
# include <graphics.h>
# include <malloc.h>

# define fan_qi_se(a) (a==1)?2:1//反色棋子 若当前为黑棋对应数字则输出白棋对应数字
# define qi_zi(a) (a==1||a==2)?1:0//如果形参是棋子编号（1或2）则输出1其余输出0

typedef struct zuo_biao_tong_ji//坐标统计
{
	int fen_zhi;
	int y;
	int x;
	int jie_guo;
}ZBTJ;

typedef struct bu_shu_tong_ji//步数统计
{
	int bu_shu;
	int y;
	int x;
}BSTJ;

typedef struct fen_zhi_hui_zong//分值汇总
{
	int fen_zhi;
	int shu_liang;
	int zu_dang;
	int rong_yu;
	int rong_yu_zu_dang;
	int sheng_yu_kong_jian;//达成五连的剩余空间
}FZHZ;

typedef struct xi_tong_can_shu//系统参数
{
	int nan_du = 11;//难度 默认为11，0简单 7正常 11困难
	int mo_shi = 0;//默认为0，0人机 1玩家 
	int sheng_fu = 0;//0胜负未分 1白方胜利 2黑方胜利 3平局
	int bu_shu = 0;//当前落子次数
	int huan_se = 2;//开局棋色 人机对战默认玩家先手
	int jie_mian = 0;//界面0为主界面 1为设置界面	
	int luo_zi_shun_xu = 0;//落子顺序 默认为0，0关闭 1打开
}XTCS;

typedef struct lin_jie_biao
{
	int y = 0;
	int x = 0;
	lin_jie_biao* next = NULL;
}LJB;


//定义全局变量
XTCS xtcs;
BSTJ bstj[225];
LJB ljb[17][17] = { 0 };//用于存放AI位置判断关系
int luo_zi_dian[17][17] = { 0 };//AI可落子点

int qi_pan[17][17] =   //0未落子 1白子 2黑子 3边缘
{
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3}
};





//游戏主要函数
void chu_shi_hua(void);//预处理初始化
void da_yin_qi_pan(void);//画面打印
void shu_biao_jiao_hu(MOUSEMSG);//鼠标交互
int xia_zi(int, int, int);//落子和储存落子记录并返回0胜负未分 1白方胜 2黑方胜 3平局
int wu_lian_pan_ding(int, int, int);//判定是否达到五连或平局
void pan_ding_sheng_fu(int);//判断胜利的一方

//游戏模式
void hui_qi(int);//悔棋
void chong_xin_kai_shi(void);//重新开始
void qi_zi_huan_se(int);//人机对战时切换ai持子颜色
void shun_xu(void);//落子顺序的显示

//AI函数
int zhu_yao_tong_ji(int, int, int);//统计主要分值并与副分值相加
int ji_suan_zui_you_luo_zi(int, int, int);//全局遍历判断落子的最优位置

int cha_ru(ZBTJ*, int, ZBTJ);//插入参数并排序
int shan_chu(ZBTJ*, int, int);//覆盖指定位置数据

void kou_fen(int, int, int, int*);//获取阻挡值来判断如何扣分
int you_shi_zhi_tong_ji(int, int, int);//统计当前这步棋落子后全局优势情况
int you_shi_zhi_ji_suan(int, int, int);

void yu_ce_shu_pai_wu_lian(int, int, int, FZHZ*);//预测当前|排是否可以达到预期五连条件
void yu_ce_heng_pai_wu_lian(int, int, int, FZHZ*);//预测当前-排是否可以达到预期五连条件
void yu_ce_fan_xie_pai_wu_lian(int, int, int, FZHZ*);//预测当前\排是否可以达到预期五连条件
void yu_ce_zheng_xie_pai_wu_lian(int, int, int, FZHZ*);//预测当前/排是否可以达到预期五连条件

void shu_pai_ji_suan(FZHZ*, int, int, int, int);//计算当前|排分值
void heng_pai_ji_suan(FZHZ*, int, int, int, int);//计算当前-排分值
void fan_xie_pai_ji_suan(FZHZ*, int, int, int, int);//计算当前\排分值
void zheng_xie_pai_ji_suan(FZHZ*, int, int, int, int);//计算当前/排分值


int ji_suan_zui_you_luo_zi1(int qi_se);
int aa(void);




void gou_zao_lin_jie_biao(void);
void lian_jie_guan_xi(int y, int x);//AI落子判断连接关系
void gou_zao_jie_dian(LJB* ljb, int y, int x);//构造节点
void tian_jia_luo_zi_dian(int y, int x);//添加落子点
void shan_chu_luo_zi_dian(int y, int x);//删除落子点

int main(void)
{
	chu_shi_hua();

	while (1)
	{
		if (MouseHit())
		{
			shu_biao_jiao_hu(GetMouseMsg());
		}
	}

	EndBatchDraw();//关闭批量绘图
	closegraph();// 关闭图形窗口

	return 0;
}

//游戏主要函数
void chu_shi_hua(void)
{
	initgraph(1024, 768, SHOWCONSOLE);// 初始化图形窗口显示控制台
	BeginBatchDraw();//启动批量绘图
	LOGFONT f;//定义字体的属性结构体
	gettextstyle(&f);//获取当前字体设置
	setbkmode(TRANSPARENT);//设置字体背景为透明
	f.lfQuality = ANTIALIASED_QUALITY;//字体抗锯齿
	settextstyle(&f);//设置字体
	da_yin_qi_pan();
	gou_zao_lin_jie_biao();//构造邻接表

	return;
}

void da_yin_qi_pan(void)
{
	int i, j;
	char s[50];
	// 初始化图形窗口
	setbkcolor(RGB(115, 115, 115));
	setlinecolor(BLACK);
	cleardevice();
	//初始化基本字体
	settextcolor(BLACK);//设置字体颜色
	settextstyle(16, 0, _T("宋体"));




	//打印棋盘
	setfillcolor(RGB(205, 155, 29));
	fillrectangle(174, 46, 850, 722);//打印外框
	rectangle(204, 76, 820, 692);//打印内框

	//打印棋盘交汇线条
	for (i = 1; i < 14; ++i)
	{
		line(204 + (44 * i), 76, 204 + (44 * i), 692);//打印竖线
		line(204, 76 + (44 * i), 820, 76 + (44 * i));//打印横线
	}

	//打印五个点
	setfillcolor(BLACK);
	solidcircle(512, 384, 5);
	solidcircle(204 + (44 * 3), 76 + (44 * 3), 5);
	solidcircle(204 + (44 * 11), 76 + (44 * 11), 5);
	solidcircle(204 + (44 * 11), 76 + (44 * 3), 5);
	solidcircle(204 + (44 * 3), 76 + (44 * 11), 5);


	//棋盘坐标
	settextcolor(BLACK);//设置字体颜色
	for (i = 0; i < 15; ++i)
	{
		sprintf(s, "%c", 'A' + i);
		outtextxy(200 + (44 * i), 50, s);
		sprintf(s, "%d", 1 + i);
		outtextxy(183, 69 + (44 * i), s);
	}

	//行棋记录
	settextcolor(BLACK);//设置字体颜色
	settextstyle(25, 0, _T("幼圆"));
	sprintf(s, "行棋记录");
	outtextxy(35, 135, s);
	setfillcolor(RGB(240, 235, 213));//背景色
	fillrectangle(25, 180, 150, 640);//打印矩形

	settextstyle(16, 0, _T("宋体"));
	for (i = 0, j = xtcs.bu_shu; j > 0 && i < 21; ++i, --j)
	{
		sprintf(s, "%0d：", bstj[j - 1].bu_shu);
		outtextxy(55, 200 + 20 * i, s);
		sprintf(s, "%c%0d", 'A' + (bstj[j - 1].x - 1), bstj[j - 1].y);
		outtextxy(85, 200 + 20 * i, s);
	}

	//打印棋子
	for (int y = 1; y < 16; ++y)
	{
		for (int x = 1; x < 16; ++x)
		{
			if (qi_pan[y][x] == 1)
			{
				setfillcolor(WHITE);
				solidcircle(204 + (44 * (x - 1)), 76 + (44 * (y - 1)), 16);//打印白子
			}
			else if (qi_pan[y][x] == 2)
			{
				setfillcolor(BLACK);
				solidcircle(204 + (44 * (x - 1)), 76 + (44 * (y - 1)), 16);//打印黑子
			}
		}
	}
	if (xtcs.luo_zi_shun_xu == 1)
	{
		shun_xu();
	}

	//显示胜负
	settextcolor(YELLOW);//设置字体颜色
	settextstyle(60, 0, _T("宋体"));
	RECT r = { 0, 0, 1024, 768 };
	if (xtcs.sheng_fu == 1)
	{
		drawtext(_T("  白方胜！"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (xtcs.sheng_fu == 2)
	{
		drawtext(_T("  黑方胜！"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (xtcs.sheng_fu == 3)
	{
		drawtext(_T("  平  局！"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	//右侧信息显示
	settextcolor(BLACK);//设置字体颜色
	settextstyle(26, 0, _T("华文琥珀"));

	if (xtcs.mo_shi == 0)
	{
		r = { 940 - 100, 100 - 20, 940 + 100, 100 + 20 };//居中字体
		if (xtcs.nan_du == 11)
		{
			drawtext(_T("电脑棋手(困难)"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if (xtcs.nan_du == 7)
		{
			drawtext(_T("电脑棋手(正常)"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else if (xtcs.nan_du == 0)
		{
			drawtext(_T("电脑棋手(简单)"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	else if (xtcs.mo_shi == 1)
	{
		r = { 920 - 100, 100 - 20, 920 + 100, 100 + 20 };//居中字体
		drawtext(_T("人类棋手"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	r = { 920 - 100, 300 - 20, 920 + 100, 300 + 20 };//居中字体
	drawtext(_T("人类棋手"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	r = { 900 - 50, 160 - 20, 900 + 50, 160 + 20 };//居中字体
	drawtext(_T("棋 色"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 900 - 50, 360 - 20, 900 + 50, 360 + 20 };//居中字体
	drawtext(_T("棋 色"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (xtcs.mo_shi == 1)
	{
		setfillcolor(WHITE);
		solidcircle(970, 160, 16);//打印棋子

		setfillcolor(BLACK);
		solidcircle(970, 360, 16);//打印棋子
	}
	else if (xtcs.mo_shi == 0)
	{
		if (xtcs.huan_se == 2)
		{
			setfillcolor(WHITE);
			solidcircle(970, 160, 16);//打印棋子

			setfillcolor(BLACK);
			solidcircle(970, 360, 16);//打印棋子
		}
		else
		{
			setfillcolor(BLACK);
			solidcircle(970, 160, 16);//打印棋子

			setfillcolor(WHITE);
			solidcircle(970, 360, 16);//打印棋子
		}

	}



	//右侧选项栏
	settextcolor(BLACK);//设置字体颜色
	settextstyle(24, 0, _T("华文琥珀"));
	setfillcolor(RGB(165, 165, 165));//填充色

	fillrectangle(940 - 50, 630 - 20, 940 + 50, 630 + 20);//打印矩形
	r = { 940 - 50, 630 - 20, 940 + 50, 630 + 20 };//居中字体
	drawtext(_T("游戏设置"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(940 - 50, 560 - 20, 940 + 50, 560 + 20);//打印矩形
	r = { 940 - 50, 560 - 20, 940 + 50, 560 + 20 };//居中字体
	drawtext(_T("重新开始"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(940 - 50, 490 - 20, 940 + 50, 490 + 20);//打印矩形
	r = { 940 - 50, 490 - 20, 940 + 50, 490 + 20 };//居中字体
	drawtext(_T("悔    棋"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//在落子位置标记红色方块
	if (xtcs.bu_shu > 0)
	{
		setlinecolor(RED);
		rectangle(221 + (44 * (bstj[xtcs.bu_shu - 1].x - 1)), 93 + (44 * (bstj[xtcs.bu_shu - 1].y - 1)), 187 + (44 * (bstj[xtcs.bu_shu - 1].x - 1)), 59 + (44 * (bstj[xtcs.bu_shu - 1].y - 1)));
	}

	//设置界面
	if (xtcs.jie_mian == 1)
	{
		settextcolor(BLACK);//设置字体颜色
		setlinecolor(BLACK);

		setfillcolor(RGB(115, 115, 115));//界面填充色
		fillrectangle(512 + 300, 384 + 200, 512 - 300, 384 - 200);

		settextstyle(50, 0, _T("华文琥珀"));
		r = { 772 + 40, 224 + 40, 772 - 40, 224 - 40 };//居中字体
		drawtext(_T("×"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		settextstyle(24, 0, _T("华文琥珀"));
		r = { 290 + 60, 300 + 20, 290 - 60, 300 - 20 };//居中字体
		drawtext(_T("模式选择："), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		r = { 290 + 60, 370 + 20, 290 - 60, 370 - 20 };//居中字体
		drawtext(_T("落子顺序："), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		r = { 290 + 60, 440 + 20, 290 - 60, 440 - 20 };//居中字体
		drawtext(_T(" A  I 难度："), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		r = { 290 + 60, 510 + 20, 290 - 60, 510 - 20 };//居中字体
		drawtext(_T(" A  I 持色："), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


		if (xtcs.mo_shi == 0)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//填充色
		}
		fillrectangle(420 - 50, 300 - 20, 420 + 50, 300 + 20);//打印矩形
		r = { 420 - 50, 300 - 20, 420 + 50, 300 + 20 };//居中字体
		drawtext(_T("人机对战"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.mo_shi == 1)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//填充色
		}
		fillrectangle(550 - 50, 300 - 20, 550 + 50, 300 + 20);//打印矩形
		r = { 550 - 50, 300 - 20, 550 + 50, 300 + 20 };//居中字体
		drawtext(_T("双人对战"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);



		setfillcolor(RGB(165, 165, 165));//填充色

		fillrectangle(680 - 50, 300 - 20, 680 + 50, 300 + 20);//打印矩形
		r = { 680 - 50, 300 - 20, 680 + 50, 300 + 20 };//居中字体
		drawtext(_T("自 定 义"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.luo_zi_shun_xu == 1)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//填充色
		}
		fillrectangle(420 - 50, 370 - 20, 420 + 50, 370 + 20);//打印矩形
		r = { 420 - 50, 370 - 20, 420 + 50, 370 + 20 };//居中字体
		drawtext(_T("开  启"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.luo_zi_shun_xu == 0)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//填充色
		}
		fillrectangle(550 - 50, 370 - 20, 550 + 50, 370 + 20);//打印矩形
		r = { 550 - 50, 370 - 20, 550 + 50, 370 + 20 };//居中字体
		drawtext(_T("关  闭"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.nan_du == 0)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//填充色
		}
		fillrectangle(420 - 50, 440 - 20, 420 + 50, 440 + 20);//打印矩形
		r = { 420 - 50, 440 - 20, 420 + 50, 440 + 20 };//居中字体
		drawtext(_T("简  单"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.nan_du == 7)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//填充色
		}
		fillrectangle(550 - 50, 440 - 20, 550 + 50, 440 + 20);//打印矩形
		r = { 550 - 50, 440 - 20, 550 + 50, 440 + 20 };//居中字体
		drawtext(_T("正  常"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.nan_du == 11)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//填充色
		}
		fillrectangle(680 - 50, 440 - 20, 680 + 50, 440 + 20);//打印矩形
		r = { 680 - 50, 440 - 20, 680 + 50, 440 + 20 };//居中字体
		drawtext(_T("困  难"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.huan_se == 1)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//填充色
		}
		fillrectangle(420 - 50, 510 - 20, 420 + 50, 510 + 20);//打印矩形
		r = { 420 - 50, 510 - 20, 420 + 50, 510 + 20 };//居中字体
		drawtext(_T("持  黑"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (xtcs.huan_se == 2)
		{
			setfillcolor(RGB(65, 65, 65));
		}
		else
		{
			setfillcolor(RGB(165, 165, 165));//填充色
		}
		fillrectangle(550 - 50, 510 - 20, 550 + 50, 510 + 20);//打印矩形
		r = { 550 - 50, 510 - 20, 550 + 50, 510 + 20 };//居中字体
		drawtext(_T("持  白"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	FlushBatchDraw();//批量绘图

	return;
}

void shu_biao_jiao_hu(MOUSEMSG m)
{
	RECT r;
	char s[50];

	// 定义鼠标消息
	int tx = 182, ty = 54;//最大范围左上角的坐标
	int w = 44;//每格的长与宽都为44
	int x, y;

	if (xtcs.jie_mian == 0)
	{
		switch (m.uMsg)
		{

		case WM_MOUSEMOVE:
			LOGFONT f;//定义字体的属性结构体
			gettextstyle(&f);//获取当前字体设置
			settextcolor(BLACK);//设置字体颜色
			setbkmode(OPAQUE);//设置字体背景为透明
			f.lfQuality = ANTIALIASED_QUALITY;//字体抗锯齿
			settextstyle(&f);//设置字体
			sprintf(s, "Y:%d  X:%d    ", m.y, m.x);
			outtextxy(10, 20, s);
			//sprintf(s, "%d", m.y);
			//outtextxy(10, 40, s);
			setbkmode(TRANSPARENT);//设置字体背景为透明
			break;

		case WM_LBUTTONDOWN://左键按下
			if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>630 - 20 && m.y < 630 + 20)
			{
				setfillcolor(RGB(65, 65, 65));
				fillrectangle(940 - 50, 630 - 20, 940 + 50, 630 + 20);
				r = { 940 - 50, 630 - 20, 940 + 50, 630 + 20 };//居中字体
				drawtext(_T("游戏设置"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>560 - 20 && m.y < 560 + 20)
			{
				setfillcolor(RGB(65, 65, 65));
				fillrectangle(940 - 50, 560 - 20, 940 + 50, 560 + 20);
				r = { 940 - 50, 560 - 20, 940 + 50, 560 + 20 };//居中字体
				drawtext(_T("重新开始"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>490 - 20 && m.y < 490 + 20)
			{
				setfillcolor(RGB(65, 65, 65));
				fillrectangle(940 - 50, 490 - 20, 940 + 50, 490 + 20);
				r = { 940 - 50, 490 - 20, 940 + 50, 490 + 20 };//居中字体
				drawtext(_T("悔    棋"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			break;

		case WM_LBUTTONUP://左键弹起
			da_yin_qi_pan();
			if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>630 - 20 && m.y < 630 + 20)
			{
				xtcs.jie_mian = 1;
			}
			else if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>560 - 20 && m.y < 560 + 20)
			{
				chong_xin_kai_shi();
			}
			else if (m.x > 940 - 50 && m.x < 940 + 50 && m.y>490 - 20 && m.y < 490 + 20)
			{
				hui_qi(xtcs.mo_shi);
			}
			else if (m.x > 0 + tx && m.x < 660 + tx && m.y + ty>0 && m.y < 660 + ty)//616+44=660 //坐标在棋盘位置则判定落子
			{
				x = (m.x - tx) / w;
				y = (m.y - ty) / w;
				if (qi_pan[y + 1][x + 1] == 0 && xtcs.sheng_fu == 0)
				{
					pan_ding_sheng_fu(xia_zi(xtcs.huan_se, y + 1, x + 1));
					da_yin_qi_pan();
					if (xtcs.mo_shi == 0 && xtcs.sheng_fu == 0)//模式为人机
					{
						if (xtcs.bu_shu < 3 || xtcs.nan_du == 0)
						{
							pan_ding_sheng_fu(ji_suan_zui_you_luo_zi1(fan_qi_se(xtcs.huan_se)));
						}
						else
						{
							pan_ding_sheng_fu(ji_suan_zui_you_luo_zi(fan_qi_se(xtcs.huan_se), fan_qi_se(xtcs.huan_se), 0));
						}
					}
					else if (xtcs.mo_shi == 1 && xtcs.sheng_fu == 0)//模式为双人
					{
						xtcs.huan_se = fan_qi_se(xtcs.huan_se);
					}
				}
			}
			da_yin_qi_pan();//左键弹起时绘图
			break;

		case WM_RBUTTONDOWN://右键按下
	/*		if (m.x > 0 + tx && m.x < 660 + tx && m.y + ty>0 && m.y < 660 + ty)//616+44=660
			{
				x = (m.x - tx) / w;
				y = (m.y - ty) / w;
				if (qi_pan[y + 1][x + 1] == 0)
				{
					xia_zi(1, y + 1, x + 1);
					//ji_suan_zui_you_luo_zi(1);
				}
			}
	*/
			while (xtcs.sheng_fu == 0)
			{
				pan_ding_sheng_fu(ji_suan_zui_you_luo_zi1(xtcs.huan_se));
				xtcs.huan_se = fan_qi_se(xtcs.huan_se);
				aa();
				da_yin_qi_pan();
				if (xtcs.sheng_fu == 0)
				{
					pan_ding_sheng_fu(ji_suan_zui_you_luo_zi(xtcs.huan_se, xtcs.huan_se, 0));
					xtcs.huan_se = fan_qi_se(xtcs.huan_se);
				}
				aa();
				da_yin_qi_pan();


			}

			break;
		case WM_MBUTTONDOWN://中间按下
			x = (m.x - tx) / w;
			y = (m.y - ty) / w;
			if (qi_pan[y + 1][x + 1] == 0)
			{
				//	sprintf(s, "%lf      ", zhu_yao_tong_ji(y + 1, x + 1, xtcs.huan_se));
				//	outtextxy(10, 40, s);
				printf("%d\n", zhu_yao_tong_ji(y + 1, x + 1, xtcs.huan_se));
			}
			break;
		}
	}
	else//界面参数不为0则进入该分支
	{
		switch (m.uMsg)
		{
		case WM_LBUTTONUP://左键弹起
			if (m.x > 772 - 25 && m.x < 772 + 25 && m.y>224 - 25 && m.y < 224 + 25)//单人
			{
				xtcs.jie_mian = 0;
			}
			else if (m.x > 420 - 50 && m.x < 420 + 50 && m.y>300 - 20 && m.y < 300 + 20)
			{
				if (xtcs.mo_shi != 0)
				{
					xtcs.mo_shi = 0;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 550 - 50 && m.x < 550 + 50 && m.y>300 - 20 && m.y < 300 + 20)//双人
			{
				if (xtcs.mo_shi != 1)
				{
					xtcs.mo_shi = 1;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 680 - 50 && m.x < 680 + 50 && m.y>300 - 20 && m.y < 300 + 20)//自定义
			{
				//自定义
			}
			else if (m.x > 420 - 50 && m.x < 420 + 50 && m.y>370 - 20 && m.y < 370 + 20)//落子顺序开启
			{
				if (xtcs.luo_zi_shun_xu != 1)
				{
					xtcs.luo_zi_shun_xu = 1;
				}
			}
			else if (m.x > 550 - 50 && m.x < 550 + 50 && m.y>370 - 20 && m.y < 370 + 20)//落子顺序关闭
			{
				if (xtcs.luo_zi_shun_xu != 0)
				{
					xtcs.luo_zi_shun_xu = 0;
				}
			}
			else if (m.x > 420 - 50 && m.x < 420 + 50 && m.y>440 - 20 && m.y < 440 + 20)//简单
			{
				if (xtcs.nan_du != 0)
				{
					xtcs.nan_du = 0;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 550 - 50 && m.x < 550 + 50 && m.y>440 - 20 && m.y < 440 + 20)//正常
			{
				if (xtcs.nan_du != 7)
				{
					xtcs.nan_du = 7;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 680 - 50 && m.x < 680 + 50 && m.y>440 - 20 && m.y < 440 + 20)//困难
			{
				if (xtcs.nan_du != 11)
				{
					xtcs.nan_du = 11;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 420 - 50 && m.x < 420 + 50 && m.y>510 - 20 && m.y < 510 + 20)//持黑
			{
				if (xtcs.huan_se != 1)
				{
					xtcs.huan_se = 1;
					chong_xin_kai_shi();
				}
			}
			else if (m.x > 550 - 50 && m.x < 550 + 50 && m.y>510 - 20 && m.y < 510 + 20)//持白
			{
				if (xtcs.huan_se != 2)
				{
					xtcs.huan_se = 2;
					chong_xin_kai_shi();
				}
			}
			da_yin_qi_pan();
			break;
		}
	}

	FlushBatchDraw();//批量绘图

	return;
}

int xia_zi(int qi_se, int y, int x)
{
	if (xtcs.sheng_fu == 0)
	{
		qi_pan[y][x] = qi_se;//落子

		//储存落子记录
		bstj[xtcs.bu_shu].y = y;
		bstj[xtcs.bu_shu].x = x;
		bstj[xtcs.bu_shu].bu_shu = xtcs.bu_shu + 1;
		++xtcs.bu_shu;

		tian_jia_luo_zi_dian(y, x);
	}

	return wu_lian_pan_ding(y, x, qi_se);
}

int wu_lian_pan_ding(int y, int x, int qi_se)
{
	int i, j;
	int Ty = y, Tx = x;
	int shu_liang;
	//检测丨排
	while (1)
	{
		if (qi_pan[Ty + 1][Tx] != qi_se)//若不等于则进入循环
		{
			for (shu_liang = 1; qi_pan[Ty][Tx] == qi_se; --Ty, ++shu_liang)
			{
				if (shu_liang == 5)
				{
					return qi_se;//当前棋子胜利;
				}
			}//若不等于当前棋子颜色编号跳出循环
			Ty = y, Tx = x;//初始化临时坐标
			break;//跳出while循环
		}
		++Ty;
	}

	//检测-排
	while (1)
	{
		if (qi_pan[Ty][Tx + 1] != qi_se)//若不等于则进入循环
		{
			for (shu_liang = 1; qi_pan[Ty][Tx] == qi_se; --Tx, ++shu_liang)
			{
				if (shu_liang == 5)
				{
					return qi_se;//当前棋子胜利;
				}
			}//若不等于当前棋子颜色编号跳出循环
			Ty = y, Tx = x;//初始化临时坐标
			break;//跳出while循环
		}
		++Tx;
	}

	//检测\排
	while (1)
	{
		if (qi_pan[Ty + 1][Tx + 1] != qi_se)//若不等于则进入循环
		{
			for (shu_liang = 1; qi_pan[Ty][Tx] == qi_se; --Ty, --Tx, ++shu_liang)
			{
				if (shu_liang == 5)
				{
					return qi_se;//当前棋子胜利;
				}
			}//若不等于当前棋子颜色编号跳出循环
			Ty = y, Tx = x;//初始化临时坐标
			break;//跳出while循环
		}
		++Ty, ++Tx;
	}

	//检测/排
	while (1)
	{
		if (qi_pan[Ty + 1][Tx - 1] != qi_se)//若不等于则进入循环
		{
			for (shu_liang = 1; qi_pan[Ty][Tx] == qi_se; --Ty, ++Tx, ++shu_liang)
			{
				if (shu_liang == 5)
				{
					return qi_se;//当前棋子胜利;
				}
			}//若不等于当前棋子颜色编号跳出循环
			Ty = y, Tx = x;//初始化临时坐标
			break;//跳出while循环
		}
		++Ty, --Tx;
	}

	for (i = 1; i < 16; ++i)
	{
		for (j = 1; j < 16; ++j)
		{
			if (qi_pan[i][j] == 0)
			{
				return 0;//还未分胜负
			}
		}
	}

	return 3;//平局
}

void pan_ding_sheng_fu(int win)
{
	da_yin_qi_pan();
	if (win == 1)
	{
		xtcs.sheng_fu = 1;//白方胜！
		MessageBox(NULL, "               白方胜出！\n请点击重新开始或悔棋继续游戏", "five", MB_SYSTEMMODAL);
	}
	else if (win == 2)
	{
		xtcs.sheng_fu = 2;//黑方胜！
		MessageBox(NULL, "               黑方胜出！\n请点击重新开始或悔棋继续游戏", "five", MB_SYSTEMMODAL);
	}
	else if (win == 3)
	{
		xtcs.sheng_fu = 3;//平局
		MessageBox(NULL, "               平    局！\n请点击重新开始或悔棋继续游戏", "five", MB_SYSTEMMODAL);
	}
	return;
}

void hui_qi(int mo_shi)
{
	if (xtcs.bu_shu > 0)
	{
		if (mo_shi == 1)//双人对战悔棋
		{
			--xtcs.bu_shu;
			qi_pan[bstj[xtcs.bu_shu].y][bstj[xtcs.bu_shu].x] = 0;
			xtcs.huan_se = fan_qi_se(xtcs.huan_se);
			shan_chu_luo_zi_dian(bstj[xtcs.bu_shu].y, bstj[xtcs.bu_shu].x);//删除落子点
		}
		else if (mo_shi == 0 && xtcs.sheng_fu != xtcs.huan_se)//防止人机对战时玩家胜利悔棋出现bug
		{
			for (int i = 0; xtcs.bu_shu >= 1 && i < 2; ++i)
			{
				--xtcs.bu_shu;
				qi_pan[bstj[xtcs.bu_shu].y][bstj[xtcs.bu_shu].x] = 0;
				shan_chu_luo_zi_dian(bstj[xtcs.bu_shu].y, bstj[xtcs.bu_shu].x);//删除落子点
			}
		}
		else
		{
			--xtcs.bu_shu;
			qi_pan[bstj[xtcs.bu_shu].y][bstj[xtcs.bu_shu].x] = 0;
			shan_chu_luo_zi_dian(bstj[xtcs.bu_shu].y, bstj[xtcs.bu_shu].x);//删除落子点
		}
	}
	xtcs.sheng_fu = 0;//将胜负重置

	return;
}

void chong_xin_kai_shi(void)
{
	int i, j;
	for (i = 1; i < 16; ++i)//遍历全局棋盘并清零
	{
		for (j = 1; j < 16; ++j)
		{
			qi_pan[i][j] = 0;
			luo_zi_dian[i][j] = 0;
		}
	}
	xtcs.bu_shu = 0;
	xtcs.sheng_fu = 0;
	if (xtcs.huan_se == 1 && xtcs.mo_shi == 0)//该参数等于1说明电脑持黑
	{
		xia_zi(2, 8, 8);
	}

	return;
}

void qi_zi_huan_se(int qi_se)
{
	if (xtcs.mo_shi == 0)
	{
		xtcs.huan_se = qi_se;
		chong_xin_kai_shi();
		if (fan_qi_se(xtcs.huan_se) == 2)
		{
			xia_zi(2, 8, 8);
		}
	}

	return;
}

void shun_xu(void)
{
	char s[5];
	RECT r;
	settextstyle(12, 0, _T("幼圆"));

	for (int i = 0; i < xtcs.bu_shu; ++i)
	{
		if (qi_pan[bstj[i].y][bstj[i].x] == 2)//若是黑子则显示白色，白子显示黑色
		{
			settextcolor(WHITE);//设置字体颜色
		}
		else
		{
			settextcolor(BLACK);//设置字体颜色
		}
		r = { 204 + (44 * (bstj[i].x - 1)) + 15, 76 + (44 * (bstj[i].y - 1)) + 15, 204 + (44 * (bstj[i].x - 1)) - 15, 76 + (44 * (bstj[i].y - 1)) - 15 };//居中字体
		sprintf(s, "%d", bstj[i].bu_shu);
		drawtext(_T(s), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}



//AI函数
int zhu_yao_tong_ji(int y, int x, int qi_se)
{
	int i, j, s1, s2;
	FZHZ fzhzt1[4] = { 0,0,0,0 };
	FZHZ fzhzt2[4] = { 0,0,0,0 };
	int zui_gao_fent1, zui_gao_fent2, sum;

	shu_pai_ji_suan(&fzhzt1[0], y, x, qi_se, qi_se);
	heng_pai_ji_suan(&fzhzt1[1], y, x, qi_se, qi_se);
	fan_xie_pai_ji_suan(&fzhzt1[2], y, x, qi_se, qi_se);
	zheng_xie_pai_ji_suan(&fzhzt1[3], y, x, qi_se, qi_se);

	//*******************************************************


	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (i == j)//i与j相等不进行计算
			{
				continue;
			}

			s1 = 0, s2 = 0;//初始化s
			if (fzhzt1[i].shu_liang >= 3 && fzhzt1[j].shu_liang >= 3)
			{
				s1 += (fzhzt1[i].shu_liang - fzhzt1[i].zu_dang);
				if (fzhzt1[i].rong_yu_zu_dang >= 1)
				{
					s1 += 1; //有冗余阻挡必然在上方阻挡扣过分所以直接加分
					if (fzhzt1[i].shu_liang >= 3 && fzhzt1[i].rong_yu_zu_dang == 2 && fzhzt1[i].sheng_yu_kong_jian >= 6)
					{
						s1 += 1;
					}
				}
				if (s1 < 3)
				{
					continue;
				}

				s2 += (fzhzt1[j].shu_liang - fzhzt1[j].zu_dang);
				if (fzhzt1[j].rong_yu_zu_dang >= 1)
				{
					s2 += 1; //有冗余阻挡必然在上方阻挡扣过分所以直接加分
					if (fzhzt1[j].shu_liang >= 3 && fzhzt1[j].rong_yu_zu_dang == 2 && fzhzt1[j].sheng_yu_kong_jian >= 6)
					{
						s2 += 1;
					}
				}
				if (s2 < 3)
				{
					continue;
				}

				if ((s1 + s2) < 6)//若s未达到6以上则不执行下方判断语句
				{
					continue;
				}

				if ((s1 + s2) == 6 && fzhzt1[i].shu_liang == 3 && fzhzt1[j].shu_liang == 3)
				{
					if (fzhzt1[i].fen_zhi < 80000)
					{
						fzhzt1[i].fen_zhi = 80000;
					}
				}
				else
				{
					if (fzhzt1[i].fen_zhi < 130000)
					{
						fzhzt1[i].fen_zhi = 130000;
					}
				}
			}
		}
	}






	//*******************************************************

	for (i = 1, j = 0; i < 4; ++i)
	{
		if (fzhzt1[j].fen_zhi < fzhzt1[i].fen_zhi)
		{
			j = i;
		}
	}

	zui_gao_fent1 = (5000 + fzhzt1[j].fen_zhi);//进攻额外提升部分优先级






	shu_pai_ji_suan(&fzhzt2[0], y, x, fan_qi_se(qi_se), qi_se);
	heng_pai_ji_suan(&fzhzt2[1], y, x, fan_qi_se(qi_se), qi_se);
	fan_xie_pai_ji_suan(&fzhzt2[2], y, x, fan_qi_se(qi_se), qi_se);
	zheng_xie_pai_ji_suan(&fzhzt2[3], y, x, fan_qi_se(qi_se), qi_se);

	//*******************************************************


	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (i == j)//i与j相等不进行计算
			{
				continue;
			}

			s1 = 0, s2 = 0;//初始化s
			if (fzhzt2[i].shu_liang >= 3 && fzhzt2[j].shu_liang >= 3)
			{
				s1 += (fzhzt2[i].shu_liang - fzhzt2[i].zu_dang);
				if (fzhzt2[i].rong_yu_zu_dang >= 1)
				{
					s1 += 1; //有冗余阻挡必然在上方阻挡扣过分所以直接加分
					if (fzhzt2[i].shu_liang >= 3 && fzhzt2[i].rong_yu_zu_dang == 2 && fzhzt2[i].sheng_yu_kong_jian >= 6)
					{
						s1 += 1;
					}
				}
				if (s1 < 3)
				{
					continue;
				}

				s2 += (fzhzt2[j].shu_liang - fzhzt2[j].zu_dang);
				if (fzhzt2[j].rong_yu_zu_dang >= 1)
				{
					s2 += 1; //有冗余阻挡必然在上方阻挡扣过分所以直接加分
					if (fzhzt2[j].shu_liang >= 3 && fzhzt2[j].rong_yu_zu_dang == 2 && fzhzt2[j].sheng_yu_kong_jian >= 6)
					{
						s2 += 1;
					}
				}
				if (s2 < 3)
				{
					continue;
				}

				if ((s1 + s2) < 6)//若s未达到6以上则不执行下方判断语句
				{
					continue;
				}

				if ((s1 + s2) == 6 && fzhzt2[i].shu_liang == 3 && fzhzt2[j].shu_liang == 3)
				{
					if (fzhzt2[i].fen_zhi < 80000)
					{
						fzhzt2[i].fen_zhi = 80000;
					}
				}
				else
				{
					if (fzhzt2[i].fen_zhi < 100000)
					{
						fzhzt2[i].fen_zhi = 100000;
					}
				}
			}
		}
	}




	//*******************************************************

	for (i = 1, j = 0; i < 4; ++i)
	{
		if (fzhzt2[j].fen_zhi < fzhzt2[i].fen_zhi)
		{
			j = i;
		}
	}

	zui_gao_fent2 = fzhzt2[j].fen_zhi;

	sum = zui_gao_fent1 > zui_gao_fent2 ? zui_gao_fent1 : zui_gao_fent2;

	for (i = 0; i < 4; ++i)
	{
		sum += (0.1 * (fzhzt1[i].fen_zhi > fzhzt2[i].fen_zhi ? fzhzt1[i].fen_zhi : fzhzt2[i].fen_zhi));
	}




	return sum;
}

int ji_suan_zui_you_luo_zi(int qi_se, int yuan_qi_se, int ci_shu)//di_gui_kong_zhi用于控制递归
{
	if (ci_shu == xtcs.nan_du)
	{
		return 0;
	}
	ZBTJ tzbtj[10];
	ZBTJ zbtj[200];
	ZBTJ t;
	zbtj[0].fen_zhi = 0;
	tzbtj[0].jie_guo = -1000000000;
	int y, x, i, j;
	int top = 1, bottom = 0;
	int len = 0;
	int tsheng_fu = 0;//临时胜负
	int zui_you_jie_guo = fan_qi_se(yuan_qi_se), lin_shi_jie_guo = 0;

	for (y = 1; y < 16; ++y)
	{
		for (x = 1; x < 16; ++x)
		{
			if (luo_zi_dian[y][x] > 0 && qi_pan[y][x] == 0)
			{
				t.fen_zhi = zhu_yao_tong_ji(y, x, qi_se);
				t.y = y;
				t.x = x;
				len = cha_ru(zbtj, len, t);
			}
		}
	}


	if (ci_shu != 0)
	{
		if (qi_se != yuan_qi_se)
		{
			while (zbtj[bottom].fen_zhi == zbtj[top].fen_zhi)
			{
				++top;
			}

			if (top != 1)
			{
				for (i = bottom, j = top; i <= j - 1; ++i)
				{
					t.jie_guo = you_shi_zhi_tong_ji(zbtj[i].y, zbtj[i].x, qi_se);
					if (tzbtj[bottom].jie_guo < t.jie_guo)
					{
						top = bottom;
					}
					if (tzbtj[bottom].jie_guo <= t.jie_guo)
					{
						zbtj[i].jie_guo = t.jie_guo;
						tzbtj[top] = zbtj[i];
						if (top != 9)
						{
							++top;
						}
					}
				}
			}
			else
			{
				tzbtj[0] = zbtj[0];
			}
			srand((int)time(0));
			bottom = rand() % (top);

			tsheng_fu = xia_zi(qi_se, tzbtj[bottom].y, tzbtj[bottom].x);
			if (tsheng_fu == 0)
			{
				lin_shi_jie_guo = ji_suan_zui_you_luo_zi(fan_qi_se(qi_se), yuan_qi_se, ci_shu + 1);
				hui_qi(3);
				return lin_shi_jie_guo;
			}
			else//若不为0那一定有结果了，则递回结果
			{
				hui_qi(3);
				return tsheng_fu;
			}
		}
		else
		{
			int xian_zhi = zbtj[0].fen_zhi * 0.6;
			while (zbtj[bottom].fen_zhi > xian_zhi && bottom < 15)//若删除到整体长度为0则跳出循环
			{
				tsheng_fu = xia_zi(qi_se, zbtj[bottom].y, zbtj[bottom].x);
				if (tsheng_fu == 0)
				{
					lin_shi_jie_guo = ji_suan_zui_you_luo_zi(fan_qi_se(qi_se), yuan_qi_se, ci_shu + 1);
					if (yuan_qi_se == lin_shi_jie_guo)
					{
						hui_qi(3);
						return lin_shi_jie_guo;
					}
					else if (lin_shi_jie_guo == 3)//结果优先级为qi_se,3,0,fan_qi_se   最高优先级会在上边的if语句里就递回，最低优先级若接下来的else if不成立最终也会被递回
					{
						zui_you_jie_guo = lin_shi_jie_guo;
					}
					else if (lin_shi_jie_guo == 0 && zui_you_jie_guo != 3)//执行到该语句最优结果不为3的可能性只有两个fan_qi_se与0
					{
						zui_you_jie_guo = lin_shi_jie_guo;
					}
					hui_qi(3);
					++bottom;
				}
				else//若不为0那一定有结果了，则递回结果
				{
					hui_qi(3);
					return tsheng_fu;
				}
			}

			return zui_you_jie_guo;//递回最优结果
		}
	}
	else if (ci_shu == 0)//首次调用进入该分支
	{
		while (1)
		{
			bottom = 0;
			int xian_zhi = zbtj[0].fen_zhi * 0.6;
			while (bottom < len && zbtj[bottom].fen_zhi > xian_zhi && bottom < 15)//若删除到整体长度为0则跳出循环
			{
				tsheng_fu = xia_zi(qi_se, zbtj[bottom].y, zbtj[bottom].x);

				if (tsheng_fu == 0)
				{
					lin_shi_jie_guo = ji_suan_zui_you_luo_zi(fan_qi_se(qi_se), yuan_qi_se, ci_shu + 1);
					if (lin_shi_jie_guo == yuan_qi_se)//未来结果为胜利直接落子
					{
						hui_qi(3);
						return xia_zi(qi_se, zbtj[bottom].y, zbtj[bottom].x);
					}
					else if (lin_shi_jie_guo == 0 || lin_shi_jie_guo == 3)//未来结果为平局或未结果则储存当前结果
					{
						zbtj[bottom].jie_guo = lin_shi_jie_guo;
					}
					else//未来结果为失败删除
					{
						len = shan_chu(zbtj, len, bottom);
						hui_qi(3);
						continue;//删除后后面的数据会到当前的位置来，不需要加一所以直接跳过，否则会漏检测
					}
				}
				else if (tsheng_fu == yuan_qi_se)//结果为胜利直接落子
				{
					hui_qi(3);
					return xia_zi(qi_se, zbtj[bottom].y, zbtj[bottom].x);
				}
				else if (tsheng_fu == 0 || tsheng_fu == 3)//未来结果为平局或未结果则储存当前结果
				{
					zbtj[bottom].jie_guo = tsheng_fu;
				}
				else//结果为失败删除
				{
					len = shan_chu(zbtj, len, bottom);
					hui_qi(3);
					continue;//删除后后面的数据会到当前的位置来，不需要加一所以直接跳过，否则会漏检测
				}

				hui_qi(3);
				++bottom;
			}
			if (len != 0)
			{
				for (i = 0; i < len && (zbtj[0].fen_zhi * 0.8) < zbtj[i].fen_zhi; ++i)
				{
					if (zbtj[i].jie_guo == 3)
					{
						return xia_zi(qi_se, zbtj[i].y, zbtj[i].x);
					}
				}
				for (i = 0; i < len && (zbtj[0].fen_zhi * 0.8) < zbtj[i].fen_zhi; ++i)
				{
					if (zbtj[i].jie_guo == 0)
					{
						return xia_zi(qi_se, zbtj[i].y, zbtj[i].x);
					}
				}
			}
			return ji_suan_zui_you_luo_zi1(yuan_qi_se);
		}
	}
	exit(-1);//临时
}

int cha_ru(ZBTJ* zbtj, int len, ZBTJ newzbtj)
{
	int low = 0, high = len, mid;
	while (high >= low)
	{
		mid = (high + low) / 2;
		if (newzbtj.fen_zhi > zbtj[mid].fen_zhi)
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	for (int i = len; i >= low; --i)
	{
		zbtj[i + 1] = zbtj[i];
	}
	zbtj[low] = newzbtj;

	return ++len;
}

int shan_chu(ZBTJ* zbtj, int len, int d)//d为删除的下标(d为0就是删除数组内第一个元素)
{
	for (; d < len; ++d)
	{
		zbtj[d] = zbtj[d + 1];
	}
	--len;
	return len;
}

void kou_fen(int zu_dang, int y, int x, int* sum)//扣分
{
	if (*sum >= 100000 && zu_dang == 0)//提高四连无阻挡时的优先级
	{
		*sum += 20000;
	}
	if (zu_dang == 1)//一边有阻挡分数除以二
	{
		*sum = *sum / 2;
	}
	else if (zu_dang >= 2)//两边有阻挡不记数
	{
		*sum = 0;
	}

	return;
}

int you_shi_zhi_tong_ji(int y, int x, int qi_se)//优势值
{
	int i, j;
	int sum1 = 0, sum2 = 0;
	qi_pan[y][x] = qi_se;

	for (i = 1; i < 16; ++i)
	{
		for (j = 1; j < 16; ++j)
		{
			if (qi_pan[i][j] == 0)
			{
				if (qi_pan[i + 1][j] == qi_se || qi_pan[i][j + 1] == qi_se || qi_pan[i + 1][j + 1] == qi_se || qi_pan[i - 1][j] == qi_se || qi_pan[i][j - 1] == qi_se || qi_pan[i - 1][j - 1] == qi_se || qi_pan[i - 1][j + 1] == qi_se || qi_pan[i + 1][j - 1] == qi_se)
				{
					sum1 += you_shi_zhi_ji_suan(i, j, 1);
				}
				if (qi_pan[i + 1][j] == fan_qi_se(qi_se) || qi_pan[i][j + 1] == fan_qi_se(qi_se) || qi_pan[i + 1][j + 1] == fan_qi_se(qi_se) || qi_pan[i - 1][j] == fan_qi_se(qi_se) || qi_pan[i][j - 1] == fan_qi_se(qi_se) || qi_pan[i - 1][j - 1] == fan_qi_se(qi_se) || qi_pan[i - 1][j + 1] == fan_qi_se(qi_se) || qi_pan[i + 1][j - 1] == fan_qi_se(qi_se))
				{
					sum2 += you_shi_zhi_ji_suan(i, j, 2);
				}
			}
		}
	}
	qi_pan[y][x] = 0;

	if (qi_se == 1)
	{
		return sum1 -= sum2;
	}
	else
	{
		return sum2 -= sum1;
	}
}

int you_shi_zhi_ji_suan(int y, int x, int qi_se)
{
	int i, j, s1, s2;
	FZHZ fzhzt1[4] = { 0,0,0,0 };
	FZHZ fzhzt2[4] = { 0,0,0,0 };
	int sum;

	shu_pai_ji_suan(&fzhzt1[0], y, x, qi_se, qi_se);
	heng_pai_ji_suan(&fzhzt1[1], y, x, qi_se, qi_se);
	fan_xie_pai_ji_suan(&fzhzt1[2], y, x, qi_se, qi_se);
	zheng_xie_pai_ji_suan(&fzhzt1[3], y, x, qi_se, qi_se);

	shu_pai_ji_suan(&fzhzt2[0], y, x, fan_qi_se(qi_se), qi_se);
	heng_pai_ji_suan(&fzhzt2[1], y, x, fan_qi_se(qi_se), qi_se);
	fan_xie_pai_ji_suan(&fzhzt2[2], y, x, fan_qi_se(qi_se), qi_se);
	zheng_xie_pai_ji_suan(&fzhzt2[3], y, x, fan_qi_se(qi_se), qi_se);

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			if (i == j)//i与j相等不进行计算
			{
				continue;
			}

			s1 = 0, s2 = 0;//初始化s
			if (fzhzt1[i].shu_liang >= 3 && fzhzt1[j].shu_liang >= 3)
			{
				s1 += (fzhzt1[i].shu_liang - fzhzt1[i].zu_dang);
				if (fzhzt1[i].rong_yu_zu_dang >= 1)
				{
					s1 += 1; //有冗余阻挡必然在上方阻挡扣过分所以直接加分
					if (fzhzt1[i].shu_liang >= 3 && fzhzt1[i].rong_yu_zu_dang == 2 && fzhzt1[i].sheng_yu_kong_jian >= 6)
					{
						s1 += 1;
					}
				}
				if (s1 < 3)
				{
					continue;
				}

				s2 += (fzhzt1[j].shu_liang - fzhzt1[j].zu_dang);
				if (fzhzt1[j].rong_yu_zu_dang >= 1)
				{
					s2 += 1; //有冗余阻挡必然在上方阻挡扣过分所以直接加分
					if (fzhzt1[j].shu_liang >= 3 && fzhzt1[j].rong_yu_zu_dang == 2 && fzhzt1[j].sheng_yu_kong_jian >= 6)
					{
						s2 += 1;
					}
				}
				if (s2 < 3)
				{
					continue;
				}

				if ((s1 + s2) < 6)//若s未达到6以上则不执行下方判断语句
				{
					continue;
				}

				if ((s1 + s2) == 6)
				{
					if (fzhzt1[i].fen_zhi < 80000)
					{
						fzhzt1[i].fen_zhi = 80000;
					}
				}
				else
				{
					if (fzhzt1[i].fen_zhi < 130000)
					{
						fzhzt1[i].fen_zhi = 130000;
					}
				}
			}
		}
	}





	for (i = 1, j = 0; i < 4; ++i)
	{
		if (fzhzt1[j].fen_zhi < fzhzt1[i].fen_zhi)
		{
			j = i;
		}
	}

	sum = fzhzt1[j].fen_zhi;//进攻额外提升一些优先级

	for (i = 0; i < 4; ++i)
	{
		sum += (0.1 * (fzhzt1[i].fen_zhi > fzhzt2[i].fen_zhi ? fzhzt1[i].fen_zhi : fzhzt2[i].fen_zhi));
	}




	return sum;
}


void yu_ce_shu_pai_wu_lian(int y, int x, int qi_se, FZHZ* fzhz)//检测|排是否可以达到预期五连条件
{
	while (qi_pan[y + 1][x] == qi_se || qi_pan[y + 1][x] == 0)
	{
		++y;
	}

	for (fzhz->sheng_yu_kong_jian = 0; qi_pan[y][x] == qi_se || qi_pan[y][x] == 0 && fzhz->sheng_yu_kong_jian < 6; --y)
	{
		++fzhz->sheng_yu_kong_jian;
	}

	if (fzhz->sheng_yu_kong_jian < 5)
	{
		++fzhz->zu_dang;
	}

	return;
}

void yu_ce_heng_pai_wu_lian(int y, int x, int qi_se, FZHZ* fzhz)//检测-排是否可以达到预期五连条件
{
	while (qi_pan[y][x + 1] == qi_se || qi_pan[y][x + 1] == 0)
	{
		++x;
	}

	for (fzhz->sheng_yu_kong_jian = 0; qi_pan[y][x] == qi_se || qi_pan[y][x] == 0 && fzhz->sheng_yu_kong_jian < 6; --x)
	{
		++fzhz->sheng_yu_kong_jian;
	}

	if (fzhz->sheng_yu_kong_jian < 5)
	{
		++fzhz->zu_dang;
	}

	return;
}

void yu_ce_fan_xie_pai_wu_lian(int y, int x, int qi_se, FZHZ* fzhz)//检测\排是否可以达到预期五连条件
{
	while (qi_pan[y + 1][x + 1] == qi_se || qi_pan[y + 1][x + 1] == 0)
	{
		++y, ++x;
	}

	for (fzhz->sheng_yu_kong_jian = 0; qi_pan[y][x] == qi_se || qi_pan[y][x] == 0 && fzhz->sheng_yu_kong_jian < 6; --y, --x)
	{
		++fzhz->sheng_yu_kong_jian;
	}

	if (fzhz->sheng_yu_kong_jian < 5)
	{
		++fzhz->zu_dang;
	}

	return;
}

void yu_ce_zheng_xie_pai_wu_lian(int y, int x, int qi_se, FZHZ* fzhz)//检测/排是否可以达到预期五连条件
{
	while (qi_pan[y + 1][x - 1] == qi_se || qi_pan[y + 1][x - 1] == 0)
	{
		++y, --x;
	}

	for (fzhz->sheng_yu_kong_jian = 0; qi_pan[y][x] == qi_se || qi_pan[y][x] == 0 && fzhz->sheng_yu_kong_jian < 6; --y, ++x)
	{
		++fzhz->sheng_yu_kong_jian;
	}

	if (fzhz->sheng_yu_kong_jian < 5)
	{
		++fzhz->zu_dang;
	}

	return;
}


void shu_pai_ji_suan(FZHZ* max, int y, int x, int qi_se, int yuan_qi_se)//检测丨排
{
	qi_pan[y][x] = qi_se;
	FZHZ fzhz[2] = { 0 };
	int Ty = y, Tx = x, Ty1, Tx1;

	//向上
	yu_ce_shu_pai_wu_lian(y, x, qi_se, &fzhz[0]);
	while (1)
	{
		if (qi_pan[Ty + 1][Tx] != qi_se)
		{
			if (qi_pan[Ty + 1][Tx] == 0)
			{
				break;
			}
			else
			{
				++fzhz[0].zu_dang;
				break;
			}
		}
		++Ty;
	}
	Ty1 = Ty, Tx1 = Tx; //保存一边尽头的坐标
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[0].shu_liang;
		if (fzhz[0].shu_liang >= 5 && fzhz[0].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[0].fen_zhi = 25;
			}
			else
			{
				fzhz[0].fen_zhi = 20;
			}
			fzhz[0].zu_dang = -1;
			break;
		}
		fzhz[0].fen_zhi += fzhz[0].shu_liang;
		--Ty;
		if (qi_pan[Ty][Tx] == 0 && fzhz[0].rong_yu == 0 && qi_pan[Ty - 1][Tx] == qi_se)
		{
			++fzhz[0].rong_yu;
			--Ty;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[0].zu_dang;
	}
	else if (fzhz[0].zu_dang == 0 && fzhz[0].shu_liang < 4)//上方if如果成立就没必要执行该语句所以改成else if
	{
		if (qi_pan[Ty - 1][Tx] != 0)
		{
			if (qi_pan[Ty - 1][Tx] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 + 2][Tx1] != 0)
		{
			if (qi_pan[Ty1 + 2][Tx1] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
	}
	fzhz[0].fen_zhi *= 10000;//使整型处理后不会进入小数位
	kou_fen(fzhz[0].zu_dang, Ty, Tx, &fzhz[0].fen_zhi);
	if (fzhz[0].rong_yu == 1)
	{
		fzhz[0].fen_zhi *= 0.9;
	}

	Ty = y, Tx = x;//初始化临时坐标

	//向下
	yu_ce_shu_pai_wu_lian(y, x, qi_se, &fzhz[1]);
	while (1)
	{
		if (qi_pan[Ty - 1][Tx] != qi_se)
		{
			if (qi_pan[Ty - 1][Tx] == 0)
			{
				break;
			}
			else
			{
				++fzhz[1].zu_dang;
				break;
			}
		}
		--Ty;
	}
	Ty1 = Ty, Tx1 = Tx; //保存一边尽头的坐标
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[1].shu_liang;
		if (fzhz[1].shu_liang >= 5 && fzhz[1].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[1].fen_zhi = 25;
			}
			else
			{
				fzhz[1].fen_zhi = 20;
			}
			fzhz[1].zu_dang = -1;
			break;
		}
		fzhz[1].fen_zhi += fzhz[1].shu_liang;
		++Ty;
		if (qi_pan[Ty][Tx] == 0 && fzhz[1].rong_yu == 0 && qi_pan[Ty + 1][Tx] == qi_se)
		{
			++fzhz[1].rong_yu;
			++Ty;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[1].zu_dang;
	}
	else if (fzhz[1].zu_dang == 0 && fzhz[1].shu_liang < 4)//上方if如果成立就没必要执行该语句所以改成else if
	{
		if (qi_pan[Ty + 1][Tx] != 0)
		{
			if (qi_pan[Ty + 1][Tx] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 - 2][Tx1] != 0)
		{
			if (qi_pan[Ty1 - 2][Tx1] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
	}
	fzhz[1].fen_zhi *= 10000;//使整型处理后不会进入小数位
	kou_fen(fzhz[1].zu_dang, Ty, Tx, &fzhz[1].fen_zhi);
	if (fzhz[1].rong_yu == 1)
	{
		fzhz[1].fen_zhi *= 0.9;
	}

	qi_pan[y][x] = 0;//位置归零

	max->fen_zhi = (fzhz[0].fen_zhi > fzhz[1].fen_zhi ? fzhz[0].fen_zhi : fzhz[1].fen_zhi);
	max->shu_liang = (fzhz[0].shu_liang > fzhz[1].shu_liang ? fzhz[0].shu_liang : fzhz[1].shu_liang);
	max->zu_dang = (fzhz[0].zu_dang > fzhz[1].zu_dang ? fzhz[0].zu_dang : fzhz[1].zu_dang);
	max->rong_yu = (fzhz[0].rong_yu > fzhz[1].rong_yu ? fzhz[0].rong_yu : fzhz[1].rong_yu);
	max->rong_yu_zu_dang = (fzhz[0].rong_yu_zu_dang > fzhz[1].rong_yu_zu_dang ? fzhz[0].rong_yu_zu_dang : fzhz[1].rong_yu_zu_dang);
	max->sheng_yu_kong_jian = (fzhz[0].sheng_yu_kong_jian > fzhz[1].sheng_yu_kong_jian ? fzhz[0].sheng_yu_kong_jian : fzhz[1].sheng_yu_kong_jian);

	return;
}

void heng_pai_ji_suan(FZHZ* max, int y, int x, int qi_se, int yuan_qi_se)//检测-排
{
	qi_pan[y][x] = qi_se;
	FZHZ fzhz[2] = { 0 };
	int Ty = y, Tx = x, Ty1, Tx1;

	//向左
	yu_ce_heng_pai_wu_lian(y, x, qi_se, &fzhz[0]);
	while (1)
	{
		if (qi_pan[Ty][Tx + 1] != qi_se)
		{
			if (qi_pan[Ty][Tx + 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[0].zu_dang;
				break;
			}
		}
		++Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //保存一边尽头的坐标
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[0].shu_liang;
		if (fzhz[0].shu_liang >= 5 && fzhz[0].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[0].fen_zhi = 25;
			}
			else
			{
				fzhz[0].fen_zhi = 20;
			}
			fzhz[0].zu_dang = -1;
			break;
		}
		fzhz[0].fen_zhi += fzhz[0].shu_liang;
		--Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[0].rong_yu == 0 && qi_pan[Ty][Tx - 1] == qi_se)
		{
			++fzhz[0].rong_yu;
			--Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[0].zu_dang;
	}
	else if (fzhz[0].zu_dang == 0 && fzhz[0].shu_liang < 4)//上方if如果成立就没必要执行该语句所以改成else if
	{
		if (qi_pan[Ty][Tx - 1] != 0)
		{
			if (qi_pan[Ty][Tx - 1] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1][Tx1 + 2] != 0)
		{
			if (qi_pan[Ty1][Tx1 + 2] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
	}
	fzhz[0].fen_zhi *= 10000;//使整型处理后不会进入小数位
	kou_fen(fzhz[0].zu_dang, Ty, Tx, &fzhz[0].fen_zhi);
	if (fzhz[0].rong_yu == 1)
	{
		fzhz[0].fen_zhi *= 0.9;
	}


	Ty = y, Tx = x;//初始化临时坐标


	//向右
	yu_ce_heng_pai_wu_lian(y, x, qi_se, &fzhz[1]);
	while (1)
	{
		if (qi_pan[Ty][Tx - 1] != qi_se)
		{
			if (qi_pan[Ty][Tx - 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[1].zu_dang;
				break;
			}
		}
		--Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //保存一边尽头的坐标
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[1].shu_liang;
		if (fzhz[1].shu_liang >= 5 && fzhz[1].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[1].fen_zhi = 25;
			}
			else
			{
				fzhz[1].fen_zhi = 20;
			}
			fzhz[1].zu_dang = -1;
			break;
		}
		fzhz[1].fen_zhi += fzhz[1].shu_liang;
		++Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[1].rong_yu == 0 && qi_pan[Ty][Tx + 1] == qi_se)
		{
			++fzhz[1].rong_yu;
			++Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[1].zu_dang;
	}
	else if (fzhz[1].zu_dang == 0 && fzhz[1].shu_liang < 4)//上方if如果成立就没必要执行该语句所以改成else if
	{
		if (qi_pan[Ty][Tx + 1] != 0)
		{
			if (qi_pan[Ty][Tx + 1] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1][Tx1 - 2] != 0)
		{
			if (qi_pan[Ty1][Tx1 - 2] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
	}
	fzhz[1].fen_zhi *= 10000;//使整型处理后不会进入小数位
	kou_fen(fzhz[1].zu_dang, Ty, Tx, &fzhz[1].fen_zhi);
	if (fzhz[1].rong_yu == 1)
	{
		fzhz[1].fen_zhi *= 0.9;
	}

	qi_pan[y][x] = 0;//位置归零

	max->fen_zhi = (fzhz[0].fen_zhi > fzhz[1].fen_zhi ? fzhz[0].fen_zhi : fzhz[1].fen_zhi);
	max->shu_liang = (fzhz[0].shu_liang > fzhz[1].shu_liang ? fzhz[0].shu_liang : fzhz[1].shu_liang);
	max->zu_dang = (fzhz[0].zu_dang > fzhz[1].zu_dang ? fzhz[0].zu_dang : fzhz[1].zu_dang);
	max->rong_yu = (fzhz[0].rong_yu > fzhz[1].rong_yu ? fzhz[0].rong_yu : fzhz[1].rong_yu);
	max->rong_yu_zu_dang = (fzhz[0].rong_yu_zu_dang > fzhz[1].rong_yu_zu_dang ? fzhz[0].rong_yu_zu_dang : fzhz[1].rong_yu_zu_dang);
	max->sheng_yu_kong_jian = (fzhz[0].sheng_yu_kong_jian > fzhz[1].sheng_yu_kong_jian ? fzhz[0].sheng_yu_kong_jian : fzhz[1].sheng_yu_kong_jian);

	return;
}

void fan_xie_pai_ji_suan(FZHZ* max, int y, int x, int qi_se, int yuan_qi_se)//检测\排
{
	qi_pan[y][x] = qi_se;
	FZHZ fzhz[2] = { 0 };
	int Ty = y, Tx = x, Ty1, Tx1;

	//向左上
	yu_ce_fan_xie_pai_wu_lian(y, x, qi_se, &fzhz[0]);
	while (1)
	{
		if (qi_pan[Ty + 1][Tx + 1] != qi_se)
		{
			if (qi_pan[Ty + 1][Tx + 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[0].zu_dang;
				break;
			}
		}
		++Ty, ++Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //保存一边尽头的坐标
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[0].shu_liang;
		if (fzhz[0].shu_liang >= 5 && fzhz[0].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[0].fen_zhi = 25;
			}
			else
			{
				fzhz[0].fen_zhi = 20;
			}
			fzhz[0].zu_dang = -1;
			break;
		}
		fzhz[0].fen_zhi += fzhz[0].shu_liang;
		--Ty, --Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[0].rong_yu == 0 && qi_pan[Ty - 1][Tx - 1] == qi_se)
		{
			++fzhz[0].rong_yu;
			--Ty, --Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[0].zu_dang;
	}
	else if (fzhz[0].zu_dang == 0 && fzhz[0].shu_liang < 4)//上方if如果成立就没必要执行该语句所以改成else if
	{
		if (qi_pan[Ty - 1][Tx - 1] != 0)
		{
			if (qi_pan[Ty - 1][Tx - 1] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 + 2][Tx1 + 2] != 0)
		{
			if (qi_pan[Ty1 + 2][Tx1 + 2] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
	}
	fzhz[0].fen_zhi *= 10000;//使整型处理后不会进入小数位
	kou_fen(fzhz[0].zu_dang, Ty, Tx, &fzhz[0].fen_zhi);
	if (fzhz[0].rong_yu == 1)
	{
		fzhz[0].fen_zhi *= 0.9;
	}

	Ty = y, Tx = x;//初始化临时坐标


	//向右下
	yu_ce_fan_xie_pai_wu_lian(y, x, qi_se, &fzhz[1]);
	while (1)
	{
		if (qi_pan[Ty - 1][Tx - 1] != qi_se)
		{
			if (qi_pan[Ty - 1][Tx - 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[1].zu_dang;
				break;
			}
		}
		--Ty, --Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //保存一边尽头的坐标
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[1].shu_liang;
		if (fzhz[1].shu_liang >= 5 && fzhz[1].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[1].fen_zhi = 25;
			}
			else
			{
				fzhz[1].fen_zhi = 20;
			}
			fzhz[1].zu_dang = -1;
			break;
		}
		fzhz[1].fen_zhi += fzhz[1].shu_liang;
		++Ty, ++Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[1].rong_yu == 0 && qi_pan[Ty + 1][Tx + 1] == qi_se)
		{
			++fzhz[1].rong_yu;
			++Ty, ++Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[1].zu_dang;
	}
	else if (fzhz[1].zu_dang == 0 && fzhz[1].shu_liang < 4)//上方if如果成立就没必要执行该语句所以改成else if
	{
		if (qi_pan[Ty + 1][Tx + 1] != 0)
		{
			if (qi_pan[Ty + 1][Tx + 1] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 - 2][Tx1 - 2] != 0)
		{
			if (qi_pan[Ty1 - 2][Tx1 - 2] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
	}
	fzhz[1].fen_zhi *= 10000;//使整型处理后不会进入小数位
	kou_fen(fzhz[1].zu_dang, Ty, Tx, &fzhz[1].fen_zhi);
	if (fzhz[1].rong_yu == 1)
	{
		fzhz[1].fen_zhi *= 0.9;
	}

	qi_pan[y][x] = 0;//位置归零

	max->fen_zhi = (fzhz[0].fen_zhi > fzhz[1].fen_zhi ? fzhz[0].fen_zhi : fzhz[1].fen_zhi);
	max->shu_liang = (fzhz[0].shu_liang > fzhz[1].shu_liang ? fzhz[0].shu_liang : fzhz[1].shu_liang);
	max->zu_dang = (fzhz[0].zu_dang > fzhz[1].zu_dang ? fzhz[0].zu_dang : fzhz[1].zu_dang);
	max->rong_yu = (fzhz[0].rong_yu > fzhz[1].rong_yu ? fzhz[0].rong_yu : fzhz[1].rong_yu);
	max->rong_yu_zu_dang = (fzhz[0].rong_yu_zu_dang > fzhz[1].rong_yu_zu_dang ? fzhz[0].rong_yu_zu_dang : fzhz[1].rong_yu_zu_dang);
	max->sheng_yu_kong_jian = (fzhz[0].sheng_yu_kong_jian > fzhz[1].sheng_yu_kong_jian ? fzhz[0].sheng_yu_kong_jian : fzhz[1].sheng_yu_kong_jian);

	return;
}

void zheng_xie_pai_ji_suan(FZHZ* max, int y, int x, int qi_se, int yuan_qi_se)//检测/排
{
	qi_pan[y][x] = qi_se;
	FZHZ fzhz[2] = { 0 };
	int Ty = y, Tx = x, Ty1, Tx1;

	//向右上
	yu_ce_zheng_xie_pai_wu_lian(y, x, qi_se, &fzhz[0]);
	while (1)
	{
		if (qi_pan[Ty + 1][Tx - 1] != qi_se)
		{
			if (qi_pan[Ty + 1][Tx - 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[0].zu_dang;
				break;
			}
		}
		++Ty, --Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //保存一边尽头的坐标
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[0].shu_liang;
		if (fzhz[0].shu_liang >= 5 && fzhz[0].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[0].fen_zhi = 25;
			}
			else
			{
				fzhz[0].fen_zhi = 20;
			}
			fzhz[0].zu_dang = -1;
			break;
		}
		fzhz[0].fen_zhi += fzhz[0].shu_liang;
		--Ty, ++Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[0].rong_yu == 0 && qi_pan[Ty - 1][Tx + 1] == qi_se)
		{
			++fzhz[0].rong_yu;
			--Ty, ++Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[0].zu_dang;
	}
	else if (fzhz[0].zu_dang == 0 && fzhz[0].shu_liang < 4)//上方if如果成立就没必要执行该语句所以改成else if
	{
		if (qi_pan[Ty - 1][Tx + 1] != 0)
		{
			if (qi_pan[Ty - 1][Tx + 1] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 + 2][Tx1 - 2] != 0)
		{
			if (qi_pan[Ty1 + 2][Tx1 - 2] != qi_se)
			{
				++fzhz[0].zu_dang;
				++fzhz[0].rong_yu_zu_dang;
			}
		}
	}
	fzhz[0].fen_zhi *= 10000;//使整型处理后不会进入小数位
	kou_fen(fzhz[0].zu_dang, Ty, Tx, &fzhz[0].fen_zhi);
	if (fzhz[0].rong_yu == 1)
	{
		fzhz[0].fen_zhi *= 0.9;
	}

	Ty = y, Tx = x;//初始化临时坐标


	//向左下
	yu_ce_zheng_xie_pai_wu_lian(y, x, qi_se, &fzhz[1]);
	while (1)
	{
		if (qi_pan[Ty - 1][Tx + 1] != qi_se)
		{
			if (qi_pan[Ty - 1][Tx + 1] == 0)
			{
				break;
			}
			else
			{
				++fzhz[1].zu_dang;
				break;
			}
		}
		--Ty, ++Tx;
	}
	Ty1 = Ty, Tx1 = Tx; //保存一边尽头的坐标
	while (qi_pan[Ty][Tx] == qi_se)
	{
		++fzhz[1].shu_liang;
		if (fzhz[1].shu_liang >= 5 && fzhz[1].rong_yu == 0)
		{
			if (yuan_qi_se == qi_se)
			{
				fzhz[1].fen_zhi = 25;
			}
			else
			{
				fzhz[1].fen_zhi = 20;
			}
			fzhz[1].zu_dang = -1;
			break;
		}
		fzhz[1].fen_zhi += fzhz[1].shu_liang;
		++Ty, --Tx;
		if (qi_pan[Ty][Tx] == 0 && fzhz[1].rong_yu == 0 && qi_pan[Ty + 1][Tx - 1] == qi_se)
		{
			++fzhz[1].rong_yu;
			++Ty, --Tx;
		}
	}
	if (qi_pan[Ty][Tx] != 0)
	{
		++fzhz[1].zu_dang;
	}
	else if (fzhz[1].zu_dang == 0 && fzhz[1].shu_liang < 4)//上方if如果成立就没必要执行该语句所以改成else if
	{
		if (qi_pan[Ty + 1][Tx - 1] != 0)
		{
			if (qi_pan[Ty + 1][Tx - 1] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
		if (qi_pan[Ty1 - 2][Tx1 + 2] != 0)
		{
			if (qi_pan[Ty1 - 2][Tx1 + 2] != qi_se)
			{
				++fzhz[1].zu_dang;
				++fzhz[1].rong_yu_zu_dang;
			}
		}
	}
	fzhz[1].fen_zhi *= 10000;//使整型处理后不会进入小数位
	kou_fen(fzhz[1].zu_dang, Ty, Tx, &fzhz[1].fen_zhi);
	if (fzhz[1].rong_yu == 1)
	{
		fzhz[1].fen_zhi *= 0.9;
	}

	qi_pan[y][x] = 0;//位置归零

	max->fen_zhi = (fzhz[0].fen_zhi > fzhz[1].fen_zhi ? fzhz[0].fen_zhi : fzhz[1].fen_zhi);
	max->shu_liang = (fzhz[0].shu_liang > fzhz[1].shu_liang ? fzhz[0].shu_liang : fzhz[1].shu_liang);
	max->zu_dang = (fzhz[0].zu_dang > fzhz[1].zu_dang ? fzhz[0].zu_dang : fzhz[1].zu_dang);
	max->rong_yu = (fzhz[0].rong_yu > fzhz[1].rong_yu ? fzhz[0].rong_yu : fzhz[1].rong_yu);
	max->rong_yu_zu_dang = (fzhz[0].rong_yu_zu_dang > fzhz[1].rong_yu_zu_dang ? fzhz[0].rong_yu_zu_dang : fzhz[1].rong_yu_zu_dang);
	max->sheng_yu_kong_jian = (fzhz[0].sheng_yu_kong_jian > fzhz[1].sheng_yu_kong_jian ? fzhz[0].sheng_yu_kong_jian : fzhz[1].sheng_yu_kong_jian);

	return;
}



int ji_suan_zui_you_luo_zi1(int qi_se)
{
	ZBTJ tzbtj[10];
	ZBTJ zbtj[226];


	zbtj[0].fen_zhi = 0;
	int y, x, i, j;
	int top = 0, bottom = 0;
	int t;


	for (y = 1; y < 16; ++y)
	{
		for (x = 1; x < 16; ++x)
		{
			if (qi_pan[y][x] == 0)
			{
				t = zhu_yao_tong_ji(y, x, qi_se);
				if (zbtj[bottom].fen_zhi < t)
				{
					top = bottom;
					zbtj[top].fen_zhi = t;
				}
				if (zbtj[bottom].fen_zhi == t)
				{
					zbtj[top].fen_zhi = t;
					zbtj[top].x = x;
					zbtj[top].y = y;
					if (top != 255)
					{
						++top;
					}
				}
			}
		}
	}

	tzbtj[0].jie_guo = -1000000000;
	if (top != 1)
	{
		for (i = bottom, j = top; i <= j - 1; ++i)
		{
			t = you_shi_zhi_tong_ji(zbtj[i].y, zbtj[i].x, qi_se);
			if (tzbtj[bottom].jie_guo < t)
			{
				top = bottom;
			}
			if (tzbtj[bottom].jie_guo <= t)
			{
				zbtj[i].jie_guo = t;
				tzbtj[top] = zbtj[i];
				if (top != 9)
				{
					++top;
				}
			}
		}
	}
	else
	{
		tzbtj[0] = zbtj[0];
	}

	srand((int)time(0));
	bottom = rand() % (top);



	return xia_zi(qi_se, tzbtj[bottom].y, tzbtj[bottom].x);
}

int aa(void)//优势值
{
	int i, j;
	int sum1 = 0, sum2 = 0;

	for (i = 1; i < 16; ++i)
	{
		for (j = 1; j < 16; ++j)
		{
			if (qi_pan[i][j] == 0)
			{
				if (qi_pan[i + 1][j] == 1 || qi_pan[i][j + 1] == 1 || qi_pan[i + 1][j + 1] == 1 || qi_pan[i - 1][j] == 1 || qi_pan[i][j - 1] == 1 || qi_pan[i - 1][j - 1] == 1 || qi_pan[i - 1][j + 1] == 1 || qi_pan[i + 1][j - 1] == 1)
				{
					sum1 += you_shi_zhi_ji_suan(i, j, 1);

				}
				if (qi_pan[i + 1][j] == 2 || qi_pan[i][j + 1] == 2 || qi_pan[i + 1][j + 1] == 2 || qi_pan[i - 1][j] == 2 || qi_pan[i][j - 1] == 2 || qi_pan[i - 1][j - 1] == 2 || qi_pan[i - 1][j + 1] == 2 || qi_pan[i + 1][j - 1] == 2)
				{
					sum2 += you_shi_zhi_ji_suan(i, j, 2);

				}
			}
		}
	}
	printf("%d\n", xtcs.bu_shu);
	printf("2 = %d\n", sum2);
	printf("1 = %d\n\n", sum1);

	return sum1;
}


void gou_zao_lin_jie_biao(void)
{
	int x, y;

	for (y = 1; y < 16; ++y)
	{
		for (x = 1; x < 16; ++x)
		{
			lian_jie_guan_xi(y, x);
		}		
	}

	return;
}

void lian_jie_guan_xi(int y, int x)//AI落子判断连接关系
{
	if (y - 1 < 16 && y - 1 > 0)//上1
	{
		gou_zao_jie_dian(&ljb[y][x], y - 1, x);
		if (y - 2 < 16 && y - 2 > 0)//上2
		{
			gou_zao_jie_dian(&ljb[y][x], y - 2, x);
		}
	}

	if (y + 1 < 16 && y + 1 > 0)//下1
	{
		gou_zao_jie_dian(&ljb[y][x], y + 1, x);
		if (y + 2 < 16 && y + 2 > 0)//下2
		{
			gou_zao_jie_dian(&ljb[y][x], y + 2, x);
		}
	}

	if (x - 1 < 16 && x - 1 > 0)//左1
	{
		gou_zao_jie_dian(&ljb[y][x], y, x - 1);
		if (x - 2 < 16 && x - 2 > 0)//左2
		{
			gou_zao_jie_dian(&ljb[y][x], y, x - 2);
		}
	}

	if (x + 1 < 16 && x + 1 > 0)//右1
	{
		gou_zao_jie_dian(&ljb[y][x], y, x + 1);
		if (x + 2 < 16 && x + 2 > 0)//右2
		{
			gou_zao_jie_dian(&ljb[y][x], y, x + 2);
		}
	}

	if (y - 1 < 16 && y - 1 > 0 && x - 1 < 16 && x - 1 > 0)//左上1
	{
		gou_zao_jie_dian(&ljb[y][x], y - 1, x - 1);
		if (y - 2 < 16 && y - 2 > 0 && x - 2 < 16 && x - 2 > 0)//左上2
		{
			gou_zao_jie_dian(&ljb[y][x], y - 2, x - 2);
		}
	}

	if (y + 1 < 16 && y + 1 > 0 && x + 1 < 16 && x + 1 > 0)//右下1
	{
		gou_zao_jie_dian(&ljb[y][x], y + 1, x + 1);
		if (y + 2 < 16 && y + 2 > 0 && x + 2 < 16 && x + 2 > 0)//右下2
		{
			gou_zao_jie_dian(&ljb[y][x], y + 2, x + 2);
		}
	}

	if (y - 1 < 16 && y - 1 > 0 && x + 1 < 16 && x + 1 > 0)//右上1
	{
		gou_zao_jie_dian(&ljb[y][x], y - 1, x + 1);
		if (y - 2 < 16 && y - 2 > 0 && x + 2 < 16 && x + 2 > 0)//右上2
		{
			gou_zao_jie_dian(&ljb[y][x], y - 2, x + 2);
		}
	}

	if (y + 1 < 16 && y + 1 > 0 && x - 1 < 16 && x - 1 > 0)//左下1
	{
		gou_zao_jie_dian(&ljb[y][x], y + 1, x - 1);
		if (y + 2 < 16 && y + 2 > 0 && x - 2 < 16 && x - 2 > 0)//左下2
		{
			gou_zao_jie_dian(&ljb[y][x], y + 2, x - 2);
		}
	}

	return;
}

void gou_zao_jie_dian(LJB* ljb, int y, int x)//构造节点
{
	while (ljb->next)
	{
		ljb = ljb->next;
	}
	LJB* p = (LJB*)malloc(sizeof(LJB));
	if (!p)
	{
		exit(-1);
	}
	p->x = x;
	p->y = y;
	p->next = NULL;
	ljb->next = p;

	return;
}

void tian_jia_luo_zi_dian(int y, int x)//添加落子点
{
	LJB* p = ljb[y][x].next;

	while (p)
	{
		luo_zi_dian[p->y][p->x]++;
		p = p->next;
	}

	return;
}

void shan_chu_luo_zi_dian(int y, int x)//删除落子点
{
	LJB* p = ljb[y][x].next;

	while (p)
	{
		luo_zi_dian[p->y][p->x]--;
		p = p->next;
	}

	return;
}